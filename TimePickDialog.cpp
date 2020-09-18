#include "TimePickDialog.h"

#include <QScrollBar>
//#include <QAbstractKineticScroller>

#include "ExpandingListWidget.h"

// TODO: Support for 12-hour format, also in other places

TimePickDialog::TimePickDialog(QTime time, QWidget *parent) :
    DateTimePickDialog(tr("Now"), parent)
{
    setWindowTitle(tr("Select time"));

    hList = new ExpandingListWidget(this);
    mList = new ExpandingListWidget(this);

    for (int h = 0; h <= 23; h++) {
        QListWidgetItem *item = new QListWidgetItem(QString("%1").arg(h, 2, 10, QChar('0')), hList);
        item->setTextAlignment(Qt::AlignCenter);
    }
    for (int m = 0; m <= 59; m++) {
        QListWidgetItem *item = new QListWidgetItem(QString("%1").arg(m, 2, 10, QChar('0')), mList);
        item->setTextAlignment(Qt::AlignCenter);
    }

    ui->listLayout->addWidget(hList);
    ui->listLayout->addWidget(mList);

    scrollers.append(Scroller(hList, 2, 0));
    scrollers.append(Scroller(mList, 2, 0));

    connect(hList, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(centerView()));
    connect(mList, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(centerView()));

    setTime(time);
}

QTime TimePickDialog::time()
{
    return QTime(row(hList), row(mList));
}

void TimePickDialog::setTime(const QTime &time)
{
    hList->setCurrentRow(time.hour());
    mList->setCurrentRow(time.minute());
}

void TimePickDialog::centerView()
{
    centerView(hList);
    centerView(mList);
}

void TimePickDialog::reset()
{
    setTime(QTime::currentTime());

    centerView();
}

void TimePickDialog::accept()
{
    emit selected(time());

    DateTimePickDialog::accept();
}
