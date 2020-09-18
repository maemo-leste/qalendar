#include "MainWindow.h"

#include <QVBoxLayout>
#include <QMenuBar>

#include <QDBusConnection>
#include <QSettings>
#include <QUrl>

#include "EventWindow.h"
#include "TodoWindow.h"
#include "ImportDialog.h"
#include "SettingsDialog.h"

#include "ChangeManager.h"
#include "Rotator.h"

MainWindow::MainWindow(bool runInBackground) :
    runInBackground(runInBackground)
{
    //this->setAttribute(Qt::WA_Maemo5StackedWindow);
    this->setProperty("X-Maemo-StackedWindow", 1);

    deleteOldComponents();

    if (runInBackground)
        ChangeManager::enableBirthdayCalendar();

    // Layout
    QWidget *centralWidget = new QWidget();
    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setCentralWidget(centralWidget);

    QDate date = QDate::currentDate();

    // Create plugs
    monthPlug = new MonthPlug(date, this);
    weekPlug = new WeekPlug(date, this);
    agendaPlug = new AgendaPlug(date, this);
    todosPlug = new TodosPlug(this);
    journalsPlug = new JournalsPlug(this);

    // Handle week button presses from the month plug
    connect(monthPlug, SIGNAL(weekActivated(QDate)), this, SLOT(showWeek(QDate)));

    // Add plugs
    mainLayout->addWidget(monthPlug);
    mainLayout->addWidget(weekPlug);
    mainLayout->addWidget(agendaPlug);
    mainLayout->addWidget(todosPlug);
    mainLayout->addWidget(journalsPlug);

    // Create the menu
    QMenuBar *menubar = new QMenuBar(this);
    windowMenu = new QMenu(menubar);
    this->setMenuBar(menubar);

    // Create the group for plug actions
    plugGroup = new QActionGroup(this);
    plugGroup->setExclusive(true);

    // Create plug actions in the group
    monthAction = new QAction(tr("Month"), plugGroup);
    weekAction = new QAction(tr("Week"), plugGroup);
    agendaAction = new QAction(tr("Agenda"), plugGroup);
    todosAction = new QAction(tr("Tasks"), plugGroup);
    journalsAction = new QAction(tr("Notes"), plugGroup);
    settingsAction = new QAction(tr("Settings"), this);

    // Make plug actions checkable
    monthAction->setCheckable(true);
    weekAction->setCheckable(true);
    agendaAction->setCheckable(true);
    todosAction->setCheckable(true);
    journalsAction->setCheckable(true);

    // Connect the actions to their slots
    connect(monthAction, SIGNAL(triggered()), this, SLOT(showMonth()));
    connect(weekAction, SIGNAL(triggered()), this, SLOT(showWeek()));
    connect(agendaAction, SIGNAL(triggered()), this, SLOT(showAgenda()));
    connect(todosAction, SIGNAL(triggered()), this, SLOT(showTodos()));
    connect(journalsAction, SIGNAL(triggered()), this, SLOT(showJournals()));

    connect(settingsAction, SIGNAL(triggered()), this, SLOT(openSettings()));

    menubar->addAction(windowMenu->menuAction());

    Rotator::acquire()->setSlave(this);

    // Select the last used plug
    QString view = QSettings().value("View", "month").toString();
    if (view == "month")
        showMonth();
    else if (view == "week")
        showWeek();
    else if (view == "agenda")
        showAgenda();
    else if (view == "todos")
        showTodos();
    else if (view == "journals")
        showJournals();

    if (runInBackground) {
        QDBusConnection::sessionBus().registerService(DBUS_SERVICE);
        QDBusConnection::sessionBus().registerObject(DBUS_PATH, this, QDBusConnection::ExportScriptableSlots);
        // An extra path to cater to the contacts app
        QDBusConnection::sessionBus().registerObject("/", this, QDBusConnection::ExportScriptableSlots);
    } else {
        show();
    }
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if (runInBackground) {
        this->hide();
        e->ignore();
    } else {
        QMainWindow::closeEvent(e);
    }
}

// Forward activation events to the current plug
void MainWindow::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::ActivationChange) {
        if (this->isActiveWindow()) {
            currentPlug->onActivated();
        } else {
            currentPlug->onDeactivated();
        }
    }

    QMainWindow::changeEvent(e);
}

// Change current plug to the provided one
void MainWindow::setPlug(Plug *plug)
{
    // Disconnect all plugs
    for (int i = 0; i < mainLayout->count(); i++) {
        mainLayout->itemAt(i)->widget()->hide();
        disconnect(mainLayout->itemAt(i)->widget(), SIGNAL(titleChanged(QString)), this, SLOT(setWindowTitle(QString)));
    }

    // Connect the chosen plug
    Rotator::acquire()->setPolicy(plug->isRotatable() ? Rotator::Automatic : Rotator::Landscape);
    connect(plug, SIGNAL(titleChanged(QString)), this, SLOT(setWindowTitle(QString)));
    this->setWindowTitle(plug->title());
    plug->show();

    // Repopulate the menu
    windowMenu->clear();
    windowMenu->addActions(plugGroup->actions());
    for (int i = 0; i < plug->actions.size(); i++)
        windowMenu->addAction(plug->actions.at(i));
    windowMenu->addAction(settingsAction);

    currentPlug = plug;
    currentPlug->onActivated();
}

// Delete events and todos older than specified in the settings
void MainWindow::deleteOldComponents()
{
    int eventsSecondsBack = QSettings().value("DeleteEventsAfter", 0).toInt();
    int  todosSecondsBack = QSettings().value("DeleteTodosAfter",  0).toInt();

    if (eventsSecondsBack == 0 && todosSecondsBack == 0) return;

    const time_t eventsCutOffStamp = QDateTime::currentDateTime().toTime_t() - eventsSecondsBack;
    const time_t  todosCutOffStamp = QDateTime::currentDateTime().toTime_t() - todosSecondsBack;

    CMulticalendar *mc = CMulticalendar::MCInstance();
    vector<CCalendar*> calendars = mc->getListCalFromMc();

    for (unsigned int i = 0; i < calendars.size(); i++) {
        if (calendars[i]->getCalendarType() == BIRTHDAY_CALENDAR) continue;

        int error;

        if (eventsSecondsBack > 0)
            mc->deleteEventBeyond(calendars[i]->getCalendarId(), eventsCutOffStamp, error);
        if (todosSecondsBack > 0)
            mc->deleteTodosBeyond(calendars[i]->getCalendarId(), todosCutOffStamp, error);
    }

    mc->releaseListCalendars(calendars);
}

// Switch to month view
void MainWindow::showMonth(QDate date)
{
    monthAction->setChecked(true);

    setPlug(monthPlug);

    if (date.isValid())
        monthPlug->setDate(date);

    QSettings().setValue("View", "month");
}

// Switch to week view
void MainWindow::showWeek(QDate date)
{
    weekAction->setChecked(true);

    setPlug(weekPlug);

    if (date.isValid())
        weekPlug->setDate(date);

    QSettings().setValue("View", "week");
}

// Switch to agenda view
void MainWindow::showAgenda(QDate date)
{
    agendaAction->setChecked(true);

    setPlug(agendaPlug);

    if (date.isValid())
        agendaPlug->setDate(date);

    QSettings().setValue("View", "agenda");
}

// Switch to tasks view
void MainWindow::showTodos()
{
    todosAction->setChecked(true);

    setPlug(todosPlug);

    QSettings().setValue("View", "todos");
}

// Switch to notes view
void MainWindow::showJournals()
{
    journalsAction->setChecked(true);

    setPlug(journalsPlug);

    QSettings().setValue("View", "journals");
}

// Open the settings dialog
void MainWindow::openSettings()
{
    (new SettingsDialog(this))->exec();
}

// Try to find the topmost visible window
QMainWindow* MainWindow::topWindow()
{
    // Assume that dialogs are always at the top
    foreach (QDialog *dialog, this->findChildren<QDialog*>())
        if (dialog->isVisible())
            return NULL;

    // Assume that windows are properly stacked, so there can be only one window
    // at the top and it can be recognized by the lack of children.
    foreach (QMainWindow *window, this->findChildren<QMainWindow*>())
        if (window->isVisible()
        &&  window->findChildren<QMainWindow*>().isEmpty())
            return window;

    return this;
}

void MainWindow::top_application()
{
    // As long as checking for external updates to the database is not
    // implemented, scheduling a refresh each time the window is shown is
    // probably a good idea.
    if (this->isHidden())
        ChangeManager::bump();

    this->show();
    this->raise();
    this->activateWindow();
}

void MainWindow::launch_view(uint type, int stamp, QString componentId, int calendarId)
{
    // Some tests show that the second argument can have following values:
    // * launching from desktop widget -> current timestamp
    // * viewing an event from alarm dialog -> -1
    // * viewing a todo from alarm dialog -> -2

    top_application();

    QDate date = stamp < 0 ? QDate() : QDateTime::fromTime_t(stamp).date();

    switch (type) {
        case 1: if (topWindow() == this) showMonth(date); break;
        case 2: if (topWindow() == this) showWeek(date); break;
        case 3: if (topWindow() == this) showAgenda(date); break;
        case 4: showComponent<CEvent, &CCalendar::getEvent>(calendarId, componentId); break;
        case 5: showComponent<CTodo, &CCalendar::getTodo>(calendarId, componentId); break;
    }
}

void MainWindow::open_bday_event(uint, QString contactId)
{
    // The first argument seems to always equal 4 (event view?)

    if (CCalendar *calendar = CMulticalendar::MCInstance()->getBirthdayCalendar()) {
        int error;
        launch_view(4, -1,
                    calendar->getExternalToLocalId(string(contactId.toLatin1()), true, error).c_str(),
                    calendar->getCalendarId());
        delete calendar;
    }
}

void MainWindow::mime_open(QString uri)
{
    top_application();

    if (QMainWindow *topWindow = this->topWindow())
        (new ImportDialog(topWindow, QUrl(uri).toLocalFile()))->show();
}

// Close the application on the next appropriate occasion
void MainWindow::deferred_close()
{
    runInBackground = false;

    if (this->isHidden())
        this->close();
}

// Helper for the main showComponent()
void MainWindow::showComponent(CEvent *event, QMainWindow *parent)
{
    ComponentInstance *instance = new ComponentInstance(event, event->getDateStart());
    (new EventWindow(instance, parent))->show();
    delete instance;
}

// Helper for the main showComponent()
void MainWindow::showComponent(CTodo *todo, QMainWindow *parent)
{
    (new TodoWindow(todo, parent))->show();
}
