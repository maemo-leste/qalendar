#include "SettingsDialog.h"

#include <QStandardItemModel>
#include <QSettings>
#include <QScroller>

#include <GConfItem>

#include "DateFormatDialog.h"
#include "CalendarsConfigSelector.h"
#include "TunePickSelector.h"

#include "Date.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    RotatingDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    ui->buttonBox->addButton(new QPushButton(tr("Save")), QDialogButtonBox::AcceptRole);

    this->setAttribute(Qt::WA_DeleteOnClose);

    // List of calendars
    ui->calendarsButton->setPickSelector(new CalendarsConfigSelector());

    // Alarm tone
    ui->tuneButton->setPickSelector(new TunePickSelector(GConfItem("/apps/calendar/calendar-alarm-tone").value().toString()));

    // Automatic cleanups
    QSettings settings;
    ui->deleteEventsButton->setPickSelector(buildDeleteSelector(settings.value("DeleteEventsAfter", 0).toInt()));
    ui->deleteTodosButton->setPickSelector(buildDeleteSelector(settings.value("DeleteTodosAfter", 0).toInt()));

    // First day of week
    QMaemo5ListPickSelector *daySelector = new QMaemo5ListPickSelector();
    QStandardItemModel *dayModel = new QStandardItemModel(0, 1, daySelector);
    for (int i = 1; i <= 7; i++) {
        QStandardItem *dayItem = new QStandardItem(QLocale().standaloneDayName(i, QLocale::LongFormat));
        dayItem->setData(i, Qt::UserRole);
        dayModel->appendRow(dayItem);
    }
    daySelector->setModel(dayModel);
    daySelector->setCurrentIndex(Date::firstDayOfWeek() - 1);
    ui->firstDayButton->setPickSelector(daySelector);

    // Editable time zones
    ui->zonesBox->setChecked(settings.value("TimeZones", false).toBool());

    connect(ui->dateFormatButton, SIGNAL(clicked()), this, SLOT(openDateFormatDialog()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(saveSettings()));

    QScroller::grabGesture(ui->mainArea, QScroller::LeftMouseButtonGesture);
    this->setFeatures(ui->dialogLayout, ui->buttonBox);
}

SettingsDialog::~SettingsDialog()
{
    QScroller::ungrabGesture(ui->mainArea);
    delete ui;
}

QMaemo5ListPickSelector* SettingsDialog::buildDeleteSelector(int selectValue)
{
    QMaemo5ListPickSelector *selector = new QMaemo5ListPickSelector();
    QStandardItemModel *model = new QStandardItemModel(0, 1, selector);

    QStandardItem *item;
    item = new QStandardItem(tr("Never")); item->setData(0, Qt::UserRole);
    model->appendRow(item);
    item = new QStandardItem(tr("Older than %n week(s)", "", 1)); item->setData(60*60*24*7, Qt::UserRole);
    model->appendRow(item);
    item = new QStandardItem(tr("Older than %n week(s)", "", 2)); item->setData(60*60*24*7*2, Qt::UserRole);
    model->appendRow(item);
    item = new QStandardItem(tr("Older than %n month(s)", "", 1)); item->setData(60*60*24*31, Qt::UserRole);
    model->appendRow(item);
    item = new QStandardItem(tr("Older than %n year(s)", "", 1)); item->setData(60*60*24*366, Qt::UserRole);
    model->appendRow(item);

    selector->setModel(model);

    for (int i = 0; i < model->rowCount(); i++) {
        if (selectValue == model->item(i)->data(Qt::UserRole).toInt()) {
            selector->setCurrentIndex(i);
            break;
        }
    }

    return selector;
}

void SettingsDialog::openDateFormatDialog()
{
    (new DateFormatDialog(this))->exec();
}

void SettingsDialog::saveSettings()
{
    TunePickSelector *tps = static_cast<TunePickSelector*>(ui->tuneButton->pickSelector());
    QMaemo5ListPickSelector *des = static_cast<QMaemo5ListPickSelector*>(ui->deleteEventsButton->pickSelector());
    QMaemo5ListPickSelector *dts = static_cast<QMaemo5ListPickSelector*>(ui->deleteTodosButton->pickSelector());
    QMaemo5ListPickSelector *fds = static_cast<QMaemo5ListPickSelector*>(ui->firstDayButton->pickSelector());

    GConfItem("/apps/calendar/calendar-alarm-tone").set(tps->currentPath());

    QSettings settings;
    settings.setValue("DeleteEventsAfter", des->model()->index(des->currentIndex(), 0).data(Qt::UserRole).toInt());
    settings.setValue("DeleteTodosAfter", dts->model()->index(dts->currentIndex(), 0).data(Qt::UserRole).toInt());
    settings.setValue("TimeZones", ui->zonesBox->isChecked());

    Date::setFirstDayOfWeek(fds->model()->index(fds->currentIndex(), 0).data(Qt::UserRole).toInt());

    this->accept();
}
