QT += core gui maemo5 dbus

TARGET = qalendar
TEMPLATE = app

INCLUDEPATH += month week recurrence settings

SOURCES += main.cpp \
    MainWindow.cpp \
    \
    TemporalPlug.cpp \
    AgendaPlug.cpp \
    JournalsPlug.cpp \
    TodosPlug.cpp \
    \
    month/MonthPlug.cpp \
    month/WeekButton.cpp \
    month/MonthWidget.cpp \
    \
    week/WeekPlug.cpp \
    week/WeekdayButton.cpp \
    week/WeekArea.cpp \
    week/WeekHintWidget.cpp \
    week/WeekWidget.cpp \
    week/ComponentWidget.cpp \
    \
    recurrence/RecurrencePickSelector.cpp \
    recurrence/RecurrencePickDialog.cpp \
    recurrence/RulePickSelector.cpp \
    recurrence/RulePickDialog.cpp \
    recurrence/ByDayWidget.cpp \
    recurrence/ByDayCoreWidget.cpp \
    recurrence/ByWeekdayWidget.cpp \
    recurrence/ByWeekdayCoreWidget.cpp \
    recurrence/ByWeekdayColumnWidget.cpp \
    recurrence/ByMonthWidget.cpp \
    \
    settings/SettingsDialog.cpp \
    settings/TunePickSelector.cpp \
    settings/TunePickDialog.cpp \
    settings/DateFormatWidget.cpp \
    settings/DateFormatDialog.cpp \
    settings/CalendarsConfigSelector.cpp \
    settings/CalendarsConfigDialog.cpp \
    settings/CalendarEditDialog.cpp \
    \
    Theme.cpp \
    \
    Date.cpp \
    ChangeManager.cpp \
    BirthdayCalendar.cpp \
    DayWindow.cpp \
    ImportDialog.cpp \
    TodoEditDialog.cpp \
    EventEditDialog.cpp \
    JournalEditDialog.cpp \
    BirthdayEditDialog.cpp \
    AlarmPickSelector.cpp \
    AlarmPickDialog.cpp \
    CalendarPickSelector.cpp \
    CalendarPickDialog.cpp \
    ColorPickSelector.cpp \
    ColorPickDialog.cpp \
    DateTimePickDialog.cpp \
    DatePickSelector.cpp \
    DatePickDialog.cpp \
    TimePickSelector.cpp \
    TimePickDialog.cpp \
    ZonePickSelector.cpp \
    ZonePickDialog.cpp \
    EventWindow.cpp \
    TodoWindow.cpp \
    CalendarListWidget.cpp \
    ComponentListWidget.cpp \
    LinkLabel.cpp \
    ExpandingTextEdit.cpp \
    Rotator.cpp

HEADERS += \
    MainWindow.h \
    \
    Plug.h \
    TemporalPlug.h \
    AgendaPlug.h \
    TodosPlug.h \
    JournalsPlug.h \
    \
    month/MonthPlug.h \
    month/WeekButton.h \
    month/MonthWidget.h \
    month/MonthLayoutWindow.h \
    \
    week/WeekPlug.h \
    week/WeekdayButton.h \
    week/WeekArea.h \
    week/WeekHintWidget.h \
    week/WeekHintProfile.h \
    week/WeekWidget.h \
    week/WeekProfile.h \
    week/WeekLayoutWindow.h \
    week/ComponentWidget.h \
    \
    recurrence/RecurrencePickSelector.h \
    recurrence/RecurrencePickDialog.h \
    recurrence/RulePickSelector.h \
    recurrence/RulePickDialog.h \
    recurrence/ByDayWidget.h \
    recurrence/ByDayCoreWidget.h \
    recurrence/ByWeekdayWidget.h \
    recurrence/ByWeekdayCoreWidget.h \
    recurrence/ByWeekdayColumnWidget.h \
    recurrence/ByMonthWidget.h \
    recurrence/DayNumberDialog.h \
    recurrence/RecurrenceSectionLabel.h \
    \
    settings/SettingsDialog.h \
    settings/TunePickSelector.h \
    settings/TunePickDialog.h \
    settings/DateFormatWidget.h \
    settings/DateFormatDialog.h \
    settings/CalendarsConfigSelector.h \
    settings/CalendarsConfigDialog.h \
    settings/CalendarEditDialog.h \
    settings/DetailedCalendarDelegate.h \
    \
    Metrics.h \
    Roles.h \
    Theme.h \
    \
    Date.h \
    ChangeManager.h \
    ChangeClient.h \
    BirthdayCalendar.h \
    DayWindow.h \
    ColorCheckDelegate.h \
    ImportDialog.h \
    TodoEditDialog.h \
    TodoDelegate.h \
    EventEditDialog.h \
    JournalEditDialog.h \
    BirthdayEditDialog.h \
    AlarmPickSelector.h \
    AlarmPickDialog.h \
    CalendarPickSelector.h \
    CalendarPickDialog.h \
    ColorPickSelector.h \
    ColorPickDialog.h \
    CalendarDelegate.h \
    DateDayDelegate.h \
    DateTimePickDialog.h \
    DatePickSelector.h \
    DatePickDialog.h \
    TimePickSelector.h \
    TimePickDialog.h \
    ZonePickSelector.h \
    ZonePickDialog.h \
    ZoneDelegate.h \
    ComponentListDelegate.h \
    DateHeadingWidget.h \
    NewComponentWidget.h \
    CWrapper.h \
    JournalDelegate.h \
    EventWindow.h \
    TodoWindow.h \
    CalendarListWidget.h \
    ComponentListWidget.h \
    ChainedSpinBox.h \
    LinkLabel.h \
    ExpandingTextEdit.h \
    ComponentInstance.h \
    VerticalScrollArea.h \
    ExpandingListWidget.h \
    GestureWidget.h \
    RotatingDialog.h \
    ComponentEditDialog.h \
    Rotator.h \
    Painter.h \
    Scroller.h

FORMS += \
    AgendaPlug.ui \
    JournalsPlug.ui \
    TodosPlug.ui \
    \
    recurrence/RecurrencePickDialog.ui \
    recurrence/RulePickDialog.ui \
    \
    settings/SettingsDialog.ui \
    settings/TunePickDialog.ui \
    settings/DateFormatDialog.ui \
    settings/CalendarsConfigDialog.ui \
    settings/CalendarEditDialog.ui \
    \
    DayWindow.ui \
    ImportDialog.ui \
    TodoEditDialog.ui \
    EventEditDialog.ui \
    JournalEditDialog.ui \
    BirthdayEditDialog.ui \
    AlarmPickDialog.ui \
    CalendarPickDialog.ui \
    ColorPickDialog.ui \
    DateTimePickDialog.ui \
    ZonePickDialog.ui \
    EventWindow.ui \
    TodoWindow.ui

TRANSLATIONS = \
    translations/qalendar_de.ts \
    translations/qalendar_en.ts \
    translations/qalendar_es.ts \
    translations/qalendar_fi.ts \
    translations/qalendar_fr.ts \
    translations/qalendar_it.ts \
    translations/qalendar_nl.ts \
    translations/qalendar_pl.ts \
    translations/qalendar_ru.ts \
    translations/qalendar_sl_SI.ts \
    translations/qalendar_uk.ts \
    translations/qalendar_vi.ts

isEmpty(QMAKE_LRELEASE) {
    QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
}
lrelease.input = TRANSLATIONS
lrelease.output = ${QMAKE_FILE_BASE}.qm
lrelease.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_BASE}.qm
lrelease.CONFIG += no_link target_predeps
QMAKE_EXTRA_COMPILERS += lrelease

isEmpty(PREFIX) {
    PREFIX = /usr
}
BINDIR = $$PREFIX/bin
DATADIR = $$PREFIX/share
PKGDATADIR = $$DATADIR/$$TARGET

DEFINES += PKGDATADIR=\\\"$$PKGDATADIR\\\"

INSTALLS += target desktop translations

target.path = $$BINDIR

desktop.path = $$DATADIR/applications/hildon
desktop.files += extra/$${TARGET}.desktop

for(TSFILE, TRANSLATIONS) {
    QMFILE = $$replace(TSFILE, ".ts", ".qm")
    translations.files += $$OUT_PWD/$$basename(QMFILE)
}
translations.CONFIG += no_check_exist
translations.path = $$PKGDATADIR/translations

INSTALLS += service
service.path = $$PREFIX/share/dbus-1/services
service.files += extra/com.nokia.calendar.service


CONFIG += link_pkgconfig
PKGCONFIG += calendar-backend gq-gconf libebook-1.2 libcityinfo0-0

LIBS += -ltime
