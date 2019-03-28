#ifndef QVOBJECTDELEGATE_H
#define QVOBJECTDELEGATE_H

#include <QObject>
#include <QItemDelegate>
#include <QStaticText>
#include "qvobjectmodel.h"

class QvObjectDelegate : public QItemDelegate
{
	Q_OBJECT

public:

	QvObjectDelegate(QObject *parent);
	~QvObjectDelegate();

	void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
	QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
  bool editorEvent( QEvent *event, QAbstractItemModel *model,
    const QStyleOptionViewItem &option, const QModelIndex &index );
	QImage defautIcon() const;
	void setDefaultIcon(const QImage & img);

signals:
	void install(const QModelIndex & index );
	void remove(const QModelIndex & index );
	void update(const QModelIndex & index );
	void enable(const QModelIndex & index );
	void disable(const QModelIndex & index );
	void cancel(const QModelIndex & index );
	void restart(const QModelIndex & index );
	void needUpdate(const QModelIndex & index );

	void detail( const QModelIndex & index );
private:

	enum ButtonAction
	{
		baNone = 0,
		baInstall = 1,
		baRemove,
		baUpdate,
		baEnable,
		baDisable,
		baCancel,
		baRestart
	};

	void paintObject( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
	void paintObjectBtn( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
	void paintObjectProgress( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
	void paintHeader( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
	
	void paintObjectHeader( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

	void paintDetail(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

	void drawButton(QStyleOptionButton &opt, const QPoint &p, QPainter * painter) const;
  bool validateButton( const QModelIndex &index, const QStyleOptionViewItem &option, QEvent * event ) ;

	QVector<ButtonAction> getButtons( const QModelIndex &index ) const;
	bool validateLabel( const QModelIndex &index, const QStyleOptionViewItem &option, QEvent * event ) ;

	int widthButtonGroup(const QModelIndex &index) const;

	void initStyleProgressOption( QStyleOptionProgressBar *option, const QModelIndex & index ) const;

	bool needRestart(const QModelIndex &index) const;

	void drawItemBackground(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;

	QRect buttonRect;
	QPoint buttonPressedPoint_;
	mutable bool labelHover_;
	QImage defaultIcon_;
};

#endif // QVOBJECTDELEGATE_H
