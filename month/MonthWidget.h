#ifndef MONTHWIDGET_H
#define MONTHWIDGET_H

#include "GestureWidget.h"

#include <QDate>
#include <QDateTime>
#include <QMouseEvent>
#include <QPixmap>

#include <CComponent.h>

#include <vector>
#include <map>

#include "ComponentInstance.h"

namespace Metrics
{
    namespace MonthWidget
    {
        const int NumWeeks = 6;
        const int NumWeekdays = 7;
    }
}

class MonthWidget : public GestureWidget
{
    Q_OBJECT

public:
    MonthWidget(QDate date, QWidget *parent);

    void setDate(QDate date);
    QDate firstDate();
    QDate lastDate();

    int CellWidth = 102;
    int CellHeight = 66;

private:
    QDate date;

    void reload();

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    void forgetPress();

    QDate pressedDate;

    QDate mapToDate(const QPoint &pos);

    QPixmap render();

    vector<CComponent*> components;
    vector<ComponentInstance*> instances;
    map<int, int> palette;
};

#endif // MONTHWIDGET_H
