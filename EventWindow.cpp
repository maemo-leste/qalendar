#include "EventWindow.h"

#include <QMaemo5Style>

#include <QDateTime>
#include <QShortcut>
#include <QTimer>

#include <CEvent.h>
#include <CRecurrenceRule.h>
#include "CWrapper.h"

#include "AlarmPickSelector.h"

#include "ChangeManager.h"

EventWindow::EventWindow(ComponentInstance *instance, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EventWindow),
    instance(*instance)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setAttribute(Qt::WA_Maemo5StackedWindow);

    this->setAttribute(Qt::WA_Maemo5AutoOrientation, true);
    this->setAttribute(Qt::WA_Maemo5LandscapeOrientation, false);
    this->setAttribute(Qt::WA_Maemo5PortraitOrientation, false);

    this->instance.event = CWrapper::details(instance->event);

    QPalette palette;
    palette.setColor(QPalette::WindowText, QMaemo5Style::standardColor("SecondaryTextColor"));
    ui->locationInfo->setPalette(palette);
    ui->detailsFrame->setPalette(palette);
    ui->fromLabel->setPalette(palette);
    ui->toLabel->setPalette(palette);
    ui->repeatLabel->setPalette(palette);
    ui->alarmLabel->setPalette(palette);
    ui->calendarInfo->setPalette(palette);
    ui->descriptionFrame->setPalette(palette);
    ui->descriptionInfo->setPalette(palette);

    connect(ui->prevAction, SIGNAL(triggered()), this, SLOT(gotoPrevEvent()));
    connect(ui->nextAction, SIGNAL(triggered()), this, SLOT(gotoNextEvent()));

    connect(ui->editAction, SIGNAL(triggered()), this, SLOT(editEvent()));
    connect(ui->deleteAction, SIGNAL(triggered()), this, SLOT(deleteEvent()));

    connect(new QShortcut(QKeySequence(Qt::Key_Left), this), SIGNAL(activated()), this, SLOT(gotoPrevEvent()));
    connect(new QShortcut(QKeySequence(Qt::Key_Right), this), SIGNAL(activated()), this, SLOT(gotoNextEvent()));

    connect(new QShortcut(QKeySequence(Qt::Key_Backspace), this), SIGNAL(activated()), this, SLOT(close()));

    reload();
}

EventWindow::~EventWindow()
{
    delete instance.event;
}

void EventWindow::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::ActivationChange
    &&  this->isActiveWindow()
    &&  this->isOutdated())
    {
        CEvent *outdatedEvent = instance.event;
        instance.event = CWrapper::details(instance.event);
        delete outdatedEvent;

        if (instance.event) {
            // If the event was edited, it could have moved elsewhere, so a fresh
            // list of instances is needed
            vector<ComponentInstance*> instances;
            CWrapper::expand(instance.component, instances, instance.stamp, instance.stamp);

            // Go through the list and try to find the current instance
            bool instanceLost = true;
            for (unsigned int i = 0; i < instances.size(); i++) {
                if (instance.stamp == instances[i]->stamp)
                    instanceLost = false;
                delete instances[i];
            }

            // If the current instance is no more, fall back to the base instance
            if (instanceLost)
                instance.stamp = instance.event->getDateStart();

            // NOTE: A more refined heuristic to find the edited instance might be
            // a useful addition, because this one is as basic and ineffective as it
            // can get.
        }

        reload();
    }

    QMainWindow::changeEvent(e);
}

void EventWindow::reload()
{
    // Gracefully recover from an event that was deleted behind the scenes
    if (!instance.event) {
        QTimer::singleShot(0, this, SLOT(close()));
        return;
    }

    this->sync();

    CEvent *event = instance.event;

    ui->summaryInfo->setText(QString::fromUtf8(event->getSummary().c_str()));
    ui->locationInfo->setText(QString::fromUtf8(event->getLocation().c_str()));

    QString description = QString::fromUtf8(event->getDescription().c_str());

    ui->descriptionInfo->setText(description);

    ui->descriptionFrame->setHidden(description.isEmpty());

    QDateTime from = QDateTime::fromTime_t(instance.stamp);
    QDateTime   to = QDateTime::fromTime_t(instance.end());

    const bool allDay = event->getAllDay();
    const bool multiDay = from.date() != to.date();

    ui->fromLabel->setText(multiDay ? tr("From") : tr("On"));
    ui->toLabel->setVisible(multiDay);
    ui->toInfo->setVisible(multiDay);

    if (multiDay) {
        ui->fromInfo->setText(from.toString(allDay ? "dddd d MMMM yyyy" : "dddd d MMMM yyyy, hh:mm"));
        ui->  toInfo->setText(  to.toString(allDay ? "dddd d MMMM yyyy" : "dddd d MMMM yyyy, hh:mm"));
    } else {
        ui->fromInfo->setText(allDay ? from.toString("dddd d MMMM yyyy")
                                     : from.toString("dddd d MMMM yyyy, hh:mm")
                              + (from.time() != to.time() ? to.toString("-hh:mm") : QString()));
    }

    vector<CRecurrenceRule*> recurrenceRules = event->retrieveRecurreceRuleObject();
    if (recurrenceRules.empty()) {
        ui->repeatLabel->hide();
        ui->repeatInfo->hide();
    } else {
        ui->repeatLabel->show();
        ui->repeatInfo->show();

        QString repeatText;
        for (unsigned int i = 0; i < recurrenceRules.size(); i++) {
            if (!repeatText.isEmpty()) repeatText += "\n\n";
            repeatText += QString(recurrenceRules[i]->getRrule().c_str()).replace(";", "\n");
        }

        ui->repeatInfo->setText(repeatText);
    }

    const int secondsBeforeAlarm = event->getAlarmBefore();
    if (secondsBeforeAlarm < 0) {
        ui->alarmLabel->hide();
        ui->alarmInfo->hide();
    } else {
        ui->alarmLabel->show();
        ui->alarmInfo->show();
        ui->alarmInfo->setText(AlarmPickSelector::textForSeconds(secondsBeforeAlarm));
    }

    int error;
    CCalendar *calendar = CMulticalendar::MCInstance()->getCalendarById(event->getCalendarId(), error);
    QIcon calendarIcon = QIcon::fromTheme(CWrapper::colorIcon(calendar->getCalendarColor()));
    QString calendarName = QString::fromUtf8(calendar->getCalendarName().c_str());
    delete calendar;

    ui->calendarIcon->setPixmap(calendarIcon.pixmap(calendarIcon.availableSizes().first()));
    ui->calendarInfo->setText(calendarName);
}

void EventWindow::gotoPrevEvent()
{
    int error;
    int calendarId;

    CComponent *component = CMulticalendar::MCInstance()->getPrevNextComponent(instance.event->getId(),
                                                                               instance.stamp,
                                                                               true,
                                                                               calendarId, instance.stamp, error);

    if (component) {
        delete instance.component;
        instance.component = component;
        reload();
    }
}

void EventWindow::gotoNextEvent()
{
    int error;
    int calendarId;

    CComponent *component = CMulticalendar::MCInstance()->getPrevNextComponent(instance.event->getId(),
                                                                               instance.stamp,
                                                                               false,
                                                                               calendarId, instance.stamp, error);

    if (component) {
        delete instance.component;
        instance.component = component;
        reload();
    }
}

void EventWindow::editEvent()
{
    ChangeManager::edit(this, instance.event);
}

void EventWindow::deleteEvent()
{
    ChangeManager::drop(this, instance.event);
}
