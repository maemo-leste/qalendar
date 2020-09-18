#ifndef GESTUREWIDGET_H
#define GESTUREWIDGET_H

#include <glib.h>
#include <gio/gio.h>

#include <QWidget>

#include <QMouseEvent>

class GestureWidget : public QWidget
{
    Q_OBJECT

public:
    GestureWidget(QWidget *parent = NULL) :
        QWidget(parent)
    {
    }

    // Mouse event handlers are public to make forwarding possible

    void mousePressEvent(QMouseEvent *e)
    {
        pressedPoint = e->pos();
    }

    void mouseReleaseEvent(QMouseEvent *)
    {
        forgetPress();
    }

    void mouseMoveEvent(QMouseEvent *e)
    {
        if (pressedPoint.isNull()) return;

        const int swipeThold = 70;
        const int dx = e->pos().x() - pressedPoint.x();
        const int dy = e->pos().y() - pressedPoint.y();

        if (qAbs(dx) > qAbs(dy*2)) { // Horizontal motion
            if (dx > swipeThold) {
                forgetPress();
                emit swipedRight();
            } else if (dx < -swipeThold) {
                forgetPress();
                emit swipedLeft();
            }
        } else if (qAbs(dy) > qAbs(dx*2)){ // Vertical motion
            if (dy > swipeThold) {
                forgetPress();
                emit swipedDown();
            } else if (dy < -swipeThold) {
                forgetPress();
                emit swipedUp();
            }
        }
    }

signals:
    void swipedUp();
    void swipedDown();
    void swipedLeft();
    void swipedRight();

protected:
    QPoint pressedPoint;

    virtual void forgetPress()
    {
        pressedPoint = QPoint();
    }
};

#endif // GESTUREWIDGET_H
