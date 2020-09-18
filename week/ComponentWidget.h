#ifndef COMPONENTWIDGET_H
#define COMPONENTWIDGET_H

#include <glib.h>
#include <gio/gio.h>

#include <QAbstractButton>
#include <QContextMenuEvent>

#include "ComponentInstance.h"

namespace Metrics
{
    namespace ComponentWidget
    {
        const int AllDayHeight = 48;
    }
}

class ComponentWidget : public QAbstractButton
{
    Q_OBJECT

public:
    ComponentWidget(ComponentInstance *instance, int color, int x, int y, int w, int h, QWidget *parent);

    void addSlave(ComponentWidget *slave);

    const int color;

private:
    ComponentInstance *instance;

    ComponentWidget *master;
    QList<ComponentWidget*> slaves;

    void paintEvent(QPaintEvent *);

    void contextMenuEvent(QContextMenuEvent *e);

    void mousePressEvent(QMouseEvent *e = NULL);
    void mouseReleaseEvent(QMouseEvent *e = NULL);
    void mouseMoveEvent(QMouseEvent *e);

private slots:
    void onClicked();
    void onPressed(bool real = true);
    void onReleased(bool real = true);

    void editComponent();
    void cloneComponent();
    void deleteComponent();
};

#endif // COMPONENTWIDGET_H
