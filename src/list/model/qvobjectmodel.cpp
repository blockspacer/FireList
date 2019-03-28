#include "qvobjectmodel.h"
#include "QvAbstractListItem.h"
#include <QCoreApplication>
#include <QDebug>

QvObjectModel::QvObjectModel(QObject *parent)
	: QAbstractItemModel(parent)
{
}

QvObjectModel::~QvObjectModel()
{
	QVector< ItemVector >::iterator i;

	
	items.clear();
}

int QvObjectModel::rowCount( const QModelIndex & parent /*= QModelIndex() */ ) const
{
	if (parent.isValid())
	{
		if(parent.internalId() < (1 << magickNum()))
		{
			return items[parent.internalId()].size();
		}
	} else {
		return items.size();
	}
	return 0;
}

QModelIndex QvObjectModel::parent( const QModelIndex & index ) const
{
	if(index.internalId() >= (1 << magickNum()) )
	{
		return createIndex(index.row(), index.column(), int(index.internalId() & mask()));
	}
	return QModelIndex();

}

int QvObjectModel::columnCount( const QModelIndex & parent /*= QModelIndex() */ ) const
{
	return 1;
}

QVariant QvObjectModel::data( const QModelIndex & index, int role /*= Qt::DisplayRole */ ) const
{
	if (index.isValid())
	{
		if(index.parent().isValid())
		{
			// Элементы списка
			return itemData( index.internalId() & mask(), index.row(), role);
		} else {
			// Заголовок разделов
			switch(role)
			{
			case Qt::DisplayRole:
				return headers_[index.row()];
			case DetailRole:
				{
					int itemsCount_ = rowCount(index),
						itemsUCount_ = rowWUpdateCount(index);
					QStringList items_;
          //items_ << QCoreApplication::translate("update", "%1 items", "total count of items", QCoreApplication::CodecForTr, itemsCount_ ).arg(itemsCount_);
          items_ << tr("update");
          items_ << tr("%1 items").arg(itemsCount_);
          items_ << tr("total count of items");
          items_ << tr("update");
          if(itemsUCount_ > 0) {
            //items_ << QCoreApplication::translate("update", "(need update %1)", "total count of items", QCoreApplication::CodecForTr, itemsUCount_ ).arg(itemsUCount_);
            items_ << tr("update");
            items_ << tr("need update %1").arg(itemsUCount_);
            items_ << tr("total count of items");
            items_ << tr("update");
          }
          return items_.join(" ");
				}
			}
		}
	}
	return QVariant();
}

QVariant QvObjectModel::itemData(int section, int row, int role ) const
{
	const QvAbstractListItem * item = items[section][row];

	if(!item)
	{
		return QVariant();
	}

	switch(role)
	{
	case DetailRole:
		return item->detail();
	case Qt::DecorationRole:
		return item->icon();
	case Qt::DisplayRole:
		return item->name();
	case DescriptionRole:
		return item->description();
	case VersionRole:
		return item->version();
	case InstalledRole:
		return item->installed();
	case EnabledRole:
		return item->enabled();
	case BuildInRole:
		return item->buildIn();
	case SourceRole:
		return item->source();
	case DateRole:
		return item->date();
	case ProgressRole:
		return item->progress();
	case GuidRole:
		return item->guid();

	default:
		return QVariant();
	}
}

QModelIndex QvObjectModel::index( int row, int column, const QModelIndex & parent /*= QModelIndex() */ ) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	int id = -1;
	if( parent.isValid() )
	{
		id = (row + 1) << magickNum();
		id = id | parent.internalId();		
	} else {
		id = row;
	}
	if(id != -1)
		return createIndex(row, column, id);
	else
		return QModelIndex();
}

int QvObjectModel::addSection( const QString & text )
{
	//beginInsertRows(QModelIndex(), items.count(), items.count());
	items << ItemVector();
	headers_ << text;
	int newRow = items.size() -1;
	beginInsertRows(QModelIndex(), newRow, newRow);	
	endInsertRows();
	return newRow;
}

bool QvObjectModel::addInSection( int section, QvAbstractListItem * item )
{
	if (item)
	{
		item->setParent(this);
		connect(item, SIGNAL(destroyed(QObject *)), this, SLOT(itemDeleted(QObject *)));
		beginInsertRows(index(section, 0), items[section].count(), items[section].count());
		items[section] << item;

		bool ret = connect(item, SIGNAL(needUpdate()), this, SLOT(itemNeedUpdate()));
		Q_ASSERT(ret);
	
		endInsertRows();
		return true;
	} else {
		return false;
	}
}

void QvObjectModel::removeSection( int section )
{
	if(section < items.size())
	{
		beginRemoveRows(QModelIndex(), section, section);
		qDeleteAll(items[section]);
		items.remove(section);
		headers_.removeAt(section);
		endRemoveRows();
	}
}

Qt::ItemFlags QvObjectModel::flags( const QModelIndex & index ) const
{
	QvAbstractListItem * item = getItem(index);
	Qt::ItemFlags f = QAbstractItemModel::flags(index);
	if(item)
	{
		f |= item->flags();
	}
	return f;
}

void QvObjectModel::install( const QModelIndex & index )
{
	QObject * obj = dynamic_cast<QObject*>(getItem(index));
	if(obj)
	{
		QMetaObject::invokeMethod(obj, "install");
	}
}

void QvObjectModel::remove( const QModelIndex & index )
{
	QObject * obj = dynamic_cast<QObject*>(getItem(index));
	if(obj)
	{
		QMetaObject::invokeMethod(obj, "remove");
	}
}

void QvObjectModel::updateItem( const QModelIndex & index )
{
	QObject * obj = dynamic_cast<QObject*>(getItem(index));
	if(obj)
	{
		QMetaObject::invokeMethod(obj, "update");
	}
}

void QvObjectModel::enable( const QModelIndex & index )
{
	QObject * obj = dynamic_cast<QObject*>(getItem(index));
	if(obj)
	{
		QMetaObject::invokeMethod(obj, "enable");
	}
}

void QvObjectModel::disable( const QModelIndex & index )
{
	QObject * obj = dynamic_cast<QObject*>(getItem(index));
	if(obj)
	{
		QMetaObject::invokeMethod(obj, "disable");
	}
}

void QvObjectModel::detail( const QModelIndex & index )
{
	QObject * obj = dynamic_cast<QObject*>(getItem(index));
	if(obj)
	{
		QMetaObject::invokeMethod(obj, "detail");
	}
}

QvAbstractListItem* QvObjectModel::getItem( const QModelIndex & index ) const
{
	QvAbstractListItem * item = 0;
	if(index.isValid() && index.parent().isValid())
	{
		int section = index.parent().internalId();
		int row = index.row();
		item = items[section][row];
	}
	return item;
}

void QvObjectModel::itemNeedUpdate()
{
	QvAbstractListItem * item = dynamic_cast<QvAbstractListItem*>(sender());
	int section = 0, row = 0;
	for (int section = 0; section < items.count(); ++section)
	{
		row = items[section].indexOf(item);
		if(row != -1)
			break;
	}
	if(section != items.count() && row != -1)
	{
		QModelIndex parent = index(section, 0, QModelIndex());
		qDebug() << "Need update item";
		emit needUpdate(index(row, 0, parent));
	}
}

void QvObjectModel::cancel( const QModelIndex & index )
{
	QObject * obj = dynamic_cast<QObject*>(getItem(index));
	if(obj)
	{
		QMetaObject::invokeMethod(obj, "cancel");
	}
}

void QvObjectModel::itemDeleted( QObject * obj )
{
	QvAbstractListItem * item = qobject_cast<QvAbstractListItem*>(obj);
	int section = 0, row = 0;
	for (int section = 0; section < items.count(); ++section)
	{
		if( (row = items[section].indexOf(item)) != -1 )
		{
			beginRemoveRows(index(section, 0), row, row);
			items[section].removeAll(item);
			qDebug() << "Remove item: " << item->name();
			endRemoveRows();
		}		
	}
}

bool QvObjectModel::setData( const QModelIndex & index, const QVariant & value, int role /*= Qt::EditRole */ )
{

	if ( !(index.isValid() && index.parent().isValid()) )
	{
		// Заголовок разделов
		return QAbstractItemModel::setData(index, value, role);
	}

	QvAbstractListItem * item = items[index.internalId() & mask()][index.row()];

	if(!item)
	{
		return QAbstractItemModel::setData(index, value, role);
	}

	switch(role)
	{
	case DetailRole:
		item->setDetail(value.toString()); break;
	case DescriptionRole:
		item->setDescription(value.toString()); break;
	case VersionRole:
		item->setVersion(value.toInt()); break;
	case SourceRole:
		item->setSource(value.toString()); break;
	case DateRole:
		item->setDate(value.toDate()); break;
	case NeedUpdateRole:
		item->setHasUpdate(value.toBool()); break;
	default:
		return QAbstractItemModel::setData(index, value, role);
	}
	return true;
}

int QvObjectModel::rowWUpdateCount( const QModelIndex & parent /*= QModelIndex() */ ) const
{
	int c = rowCount(parent),
		ret = 0;
	for(int i = 0; i < c; ++i)
	{
		if(parent.child(i, 0).flags() & Qt::ItemFlag(QvAbstractListItem::hasUpdate))
			++ret;
	}
	return ret;
}
