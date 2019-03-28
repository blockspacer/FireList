#ifndef QVOBJECTLISTVIEW_H
#define QVOBJECTLISTVIEW_H

#include <QTreeView>
#include <QPointer>
#include <QSet>

class QvObjectModel;
class QvObjectDelegate;
class QvAbstractListItem;
class QSortFilterProxyModel;
class QvObjectLoader;

class QvObjectListView : public QTreeView
{
	Q_OBJECT

public:
	QvObjectListView(QWidget *parent);
	~QvObjectListView();
	
	QImage defautIcon() const;
	void setDefaultIcon(const QImage & img);

	void setModel(QAbstractItemModel *model);


signals:
	void install(const QModelIndex & index );
	void remove(const QModelIndex & index );
	void updateItem(const QModelIndex & index );
	void enable(const QModelIndex & index );
	void disable(const QModelIndex & index );
	void detail( const QModelIndex & index );
	void cancel( const QModelIndex & index );
	void restart( );

private:

	QPointer<QvObjectModel> model_;
	QPointer<QvObjectDelegate> delegate_;
	QPointer<QSortFilterProxyModel> proxy_;
};

#endif // QVOBJECTLISTVIEW_H
