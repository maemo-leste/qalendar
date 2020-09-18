#ifndef WEEKBUTTON_H
#define WEEKBUTTON_H

#include <glib.h>
#include <gio/gio.h>

#include <QAbstractButton>

#include <QDate>

class WeekButton : public QAbstractButton
{
    Q_OBJECT

public:
    WeekButton(QWidget *parent);

    QSize sizeHint() const;

    void setDate(QDate date);

signals:
    void clicked(QDate date);

private:
    QDate date;

    void paintEvent(QPaintEvent *);

private slots:
    void onClicked();
};

#endif // WEEKBUTTON_H
