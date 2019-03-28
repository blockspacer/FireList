#ifndef QvSortFilterModel_h__
#define QvSortFilterModel_h__

#include <QSortFilterProxyModel>

class QvSortFilterModel :
	public QSortFilterProxyModel
{
	Q_OBJECT
public:
	QvSortFilterModel(QObject * parent);
	~QvSortFilterModel(void);

	void setSourceModel ( QAbstractItemModel * sourceModel );

signals:
	void needUpdate(const QModelIndex & index);

public slots:
	void install(const QModelIndex & index );
	void remove(const QModelIndex & index );
	void updateItem(const QModelIndex & index );
	void enable(const QModelIndex & index );
	void disable(const QModelIndex & index );
	void detail( const QModelIndex & index );
	void cancel( const QModelIndex & index );

private slots:
	void needUpdate_src(const QModelIndex & index);

protected:
	bool lessThan ( const QModelIndex & left, const QModelIndex & right ) const;
};
#endif // QvSortFilterModel_h__
