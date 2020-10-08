#include "WeekButton.h"

#include <cmath>

#include <QPixmap>
#include <QPainter>

#include <QGuiApplication>
#include <QScreen>

#include "Date.h"

#include "Theme.h"
#include "MonthWidget.h"

using namespace Metrics::WeekButton;

WeekButton::WeekButton(QWidget *parent) :
    QAbstractButton(parent)
{
    // Set font size
    QFont font = this->font();
    font.setPointSize(13);
    this->setFont(font);

    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    connect(this, SIGNAL(clicked()), this, SLOT(onClicked()));
}

QSize WeekButton::sizeHint() const
{
    return QSize(Width, -1);
}

void WeekButton::setDate(QDate date)
{
    this->date = date;

    this->setText(QString(tr("w%1")).arg(QString::number(Date::relWeekNumber(date)), 2, '0'));
}

void WeekButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    QPixmap pix = this->isDown() ? QPixmap(PIX_MONTH_WEEK_PRESSED) : QPixmap(PIX_MONTH_WEEK);
    pix = pix.scaled(QSize(this->width(), this->height()));
    painter.drawPixmap(0, 0, pix);

    painter.drawText(0, 0, this->width(), this->height(), Qt::AlignHCenter|Qt::AlignVCenter, this->text());
}

void WeekButton::onClicked()
{
    emit clicked(date);
}
