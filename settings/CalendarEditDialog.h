#ifndef CALENDAREDITDIALOG_H
#define CALENDAREDITDIALOG_H

#include <glib.h>
#include <gio/gio.h>

#include "RotatingDialog.h"
#include "ui_CalendarEditDialog.h"

#include "CCalendar.h"

class CalendarEditDialog : public RotatingDialog
{
    Q_OBJECT

public:
    CalendarEditDialog(int calendarId, QWidget *parent);
    ~CalendarEditDialog();

    int calendarId();

    void setCalendarName(QString name);

private:
    Ui::CalendarEditDialog *ui;

    CCalendar *calendar;

private slots:
    void onTypeButtonClicked(int type);

    void saveCalendar();
    void exportCalendar();
    void deleteCalendar();
};

#endif // CALENDAREDITDIALOG_H
