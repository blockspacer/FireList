#ifndef QVOBJECTMODEL_H
#define QVOBJECTMODEL_H

#include "QvAbstractListItem.h"
#include <QAbstractItemModel>
#include <QUrl>
#include <QVector>
#include <QStringList>
#include <QImage>


class QvObjectModel : public QAbstractItemModel
{
	Q_OBJECT

public:

	enum ObjectDataRole
	{
		DescriptionRole = Qt::UserRole + 1,
		DetailRole,
		VersionRole,
		InstalledRole,
		EnabledRole,
		BuildInRole,
		SourceRole,
		IconRole,
		DateRole,
		GuidRole,
		ProgressRole,
		NeedUpdateRole
	};

	QvObjectModel(QObject *parent);
	~QvObjectModel();
	int	columnCount ( const QModelIndex & parent = QModelIndex() ) const;
	QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
	QModelIndex	index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
	QModelIndex	parent ( const QModelIndex & index ) const;
	int	rowCount ( const QModelIndex & parent = QModelIndex() ) const;
	int	rowWUpdateCount ( const QModelIndex & parent = QModelIndex() ) const;

	bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );

	int addSection(const QString & text);
	void removeSection(int section);
	bool addInSection(int section, QvAbstractListItem * item);

	Qt::ItemFlags flags ( const QModelIndex & index ) const;


signals:
	void needUpdate(QModelIndex);

public slots:
	void install(const QModelIndex & index );
	void remove(const QModelIndex & index );
	void updateItem(const QModelIndex & index );
	void enable(const QModelIndex & index );
	void disable(const QModelIndex & index );
	void detail( const QModelIndex & index );
	void cancel( const QModelIndex & index );

private slots:
	void itemNeedUpdate();
	void itemDeleted(QObject * item);

private:
	typedef QList< QvAbstractListItemPtr > ItemVector;
	
	QVector< ItemVector > items;
	QStringList headers_;

	QvAbstractListItem* getItem( const QModelIndex & index ) const;

	inline int magickNum() const
	{
		return items.count() % 2 + items.count() / 2;
	};

	inline int mask() const
	{
		int m = 1;
		int c = magickNum();
		for (int i = 1; i < c; ++i)
		{
			m = (m << 1) | 1;
		}
		return m;
	};

	QVariant itemData(int section, int row, int role) const;
};

#endif // QVOBJECTMODEL_H
