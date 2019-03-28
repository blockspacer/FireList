#include "QvSortFilterModel.h"

QvSortFilterModel::QvSortFilterModel(QObject * parent):QSortFilterProxyModel(parent)
{
}

QvSortFilterModel::~QvSortFilterModel(void)
{
}

bool QvSortFilterModel::lessThan ( const QModelIndex & left, const QModelIndex & right ) const
{
	if(left.parent().isValid())
	{
		return QSortFilterProxyModel::lessThan(left, right);
	} else {
		return false;
	}
}

void QvSortFilterModel::install( const QModelIndex & index )
{
	QModelIndex srcIndex = mapToSource(index);
	QMetaObject::invokeMethod(sourceModel(), "install", Q_ARG(QModelIndex, srcIndex));
}

void QvSortFilterModel::remove( const QModelIndex & index )
{
	QModelIndex srcIndex = mapToSource(index);
	QMetaObject::invokeMethod(sourceModel(), "remove", Q_ARG(QModelIndex, srcIndex));
}

void QvSortFilterModel::updateItem( const QModelIndex & index )
{
	QModelIndex srcIndex = mapToSource(index);
	QMetaObject::invokeMethod(sourceModel(), "update", Q_ARG(QModelIndex, srcIndex));
}

void QvSortFilterModel::enable( const QModelIndex & index )
{
	QModelIndex srcIndex = mapToSource(index);
	QMetaObject::invokeMethod(sourceModel(), "enable", Q_ARG(QModelIndex, srcIndex));
}

void QvSortFilterModel::disable( const QModelIndex & index )
{
	QModelIndex srcIndex = mapToSource(index);
	QMetaObject::invokeMethod(sourceModel(), "disable", Q_ARG(QModelIndex, srcIndex));
}

void QvSortFilterModel::detail( const QModelIndex & index )
{
	QModelIndex srcIndex = mapToSource(index);
	QMetaObject::invokeMethod(sourceModel(), "detail", Q_ARG(QModelIndex, srcIndex));
}

void QvSortFilterModel::needUpdate_src( const QModelIndex & index )
{
	emit needUpdate(mapFromSource(index));
}

void QvSortFilterModel::setSourceModel( QAbstractItemModel * src )
{
	if(sourceModel())
	{
		disconnect(sourceModel(), SIGNAL(needUpdate(QModelIndex)),
			this, SLOT(needUpdate_src(QModelIndex)));
	}
	
	QSortFilterProxyModel::setSourceModel(src);

	connect(sourceModel(), SIGNAL(needUpdate(QModelIndex)),
		this, SLOT(needUpdate_src(QModelIndex)));
}

void QvSortFilterModel::cancel( const QModelIndex & index )
{
	QModelIndex srcIndex = mapToSource(index);
	QMetaObject::invokeMethod(sourceModel(), "cancel", Q_ARG(QModelIndex, srcIndex));
}