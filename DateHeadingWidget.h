#ifndef DATEHEADINGWIDGET_H
#define DATEHEADINGWIDGET_H

#include <glib.h>
#include <gio/gio.h>


#include <QWidget>
#include <QMaemo5Style>
#include <QPainter>
#include <QDate>

#include "Date.h"

class DateHeadingWidget : public QWidget
{
    Q_OBJECT

public:
    DateHeadingWidget(QWidget *parent) :
        QWidget(parent)
    {
    }

    void setDate(QDate date)
    {
        this->date = date;
    }

    static void paint(QPainter &painter, const QRect &rect, const QDate &date)
    {
        QFont font = painter.font();
        font.setBold(true);
        font.setPointSize(13);
        painter.setFont(font);

        painter.setPen(QMaemo5Style::standardColor(date == QDate::currentDate() ? "ActiveTextColor"
                                                                                : "SecondaryTextColor"));

        painter.fillRect(rect, QPalette().color(QPalette::Window));

        painter.drawText(rect, Qt::AlignHCenter|Qt::AlignVCenter,
                         Date::toString(date, Date::Full));
    }

private:
    QDate date;

    void paintEvent(QPaintEvent *)
    {
        QPainter painter(this);

        paint(painter, this->rect(), date);
    }
};

#endif // DATEHEADINGWIDGET_H
