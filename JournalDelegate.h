#ifndef JOURNALDELEGATE_H
#define JOURNALDELEGATE_H

#include <glib.h>
#include <gio/gio.h>


#include <CJournal.h>

#include <QStyledItemDelegate>
#include <QPainter>

#include <QMaemo5Style>
#include <QFontMetrics>
#include <QDateTime>

#include "CWrapper.h"
#include "Date.h"
#include "Roles.h"
#include "Metrics.h"

class JournalDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    JournalDelegate(QObject *parent) : QStyledItemDelegate(parent) { }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        using namespace Metrics::Item;
        using namespace Metrics::Pixmap;

        QStyledItemDelegate::paint(painter, option, QModelIndex());

        CJournal *journal = qvariant_cast<CJournal*>(index.data(JournalRole));

        if (!journal) return;

        QString summary = CWrapper::simplify(journal->getSummary(), TextMaxChars);
        QDateTime date = QDateTime::QDateTime::fromTime_t(journal->getDateStart());
        QIcon icon = QIcon::fromTheme(CWrapper::colorIcon(index.data(ColorRole).toInt()));

        QRect r = option.rect.adjusted(Margin, Margin, -Margin, -Margin);
        QFont f = painter->font();

        painter->save();

        painter->drawPixmap(r.left(),
                            r.top() + (r.height() - BulletSize) / 2,
                            icon.pixmap(IconSize, IconSize).scaled(BulletSize, BulletSize));

        r.adjust(BulletSize+Margin*3, TextMargin, 0, -ValueMargin);

        // Draw summary
        painter->drawText(r, Qt::AlignTop|Qt::AlignLeft,
                          QFontMetrics(f).elidedText(summary, Qt::ElideRight, r.width()));

        // Draw modification date
        f.setPointSize(13);
        painter->setFont(f);
        painter->setPen(QMaemo5Style::standardColor("SecondaryTextColor"));
        painter->drawText(r, Qt::AlignBottom|Qt::AlignLeft,
                          QFontMetrics(f).elidedText(Date::toString(date, Date::Full, true), Qt::ElideRight, r.width()));

        painter->restore();
    }

    QSize sizeHint(const QStyleOptionViewItem&, const QModelIndex&) const
    {
        using namespace Metrics::Item;

        return QSize(Width, Height);
    }
};

#endif // JOURNALDELEGATE_H
