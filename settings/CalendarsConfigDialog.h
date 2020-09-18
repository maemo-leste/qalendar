#ifndef CALENDARSCONFIGDIALOG_H
#define CALENDARSCONFIGDIALOG_H

#include <glib.h>
#include <gio/gio.h>

#include "RotatingDialog.h"
#include "ui_CalendarsConfigDialog.h"

class CalendarsConfigDialog : public RotatingDialog
{
    Q_OBJECT

public:
    CalendarsConfigDialog(QWidget *parent);
    ~CalendarsConfigDialog();

private:
    Ui::CalendarsConfigDialog *ui;

    void reload();

private slots:
    void onCalendarActivated(QListWidgetItem *item);
    void onCalendarChanged(QListWidgetItem *item);

    void newCalendar();
};

#endif // CALENDARSCONFIGDIALOG_H
