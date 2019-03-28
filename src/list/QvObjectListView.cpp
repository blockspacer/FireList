#include "QvObjectListView.h"
#include "model/qvobjectmodel.h"
#include "model/QvObjectDelegate.h"

#include <QCoreApplication>


QvObjectListView::QvObjectListView(QWidget *parent)
	: QTreeView(parent)
{

	delegate_ = new QvObjectDelegate(this);
	setItemDelegate(delegate_);
	
	setIndentation(0);
	setItemsExpandable(false);
	setRootIsDecorated(false);
	setHeaderHidden(true);
	setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	setMouseTracking(true);
	setDefaultIcon(QImage(":/Resources/plugin.png"));

	setMinimumWidth(450);


}

QvObjectListView::~QvObjectListView()
{
	//delete proxy_;
	delete delegate_;
}

QImage QvObjectListView::defautIcon() const
{
	if(delegate_)
	{
		return delegate_->defautIcon();
	}
	return QImage();
}

void QvObjectListView::setDefaultIcon( const QImage & img )
{
	if(delegate_)
	{
		delegate_->setDefaultIcon(img);
	}
}

void QvObjectListView::setModel( QAbstractItemModel *m )
{
	// Включение сортировки
	/*
	model_ = m;
	proxy_ = new QvSortFilterModel(this);
	proxy_->setSourceModel(model_);
	
	proxy_->setSortCaseSensitivity(Qt::CaseInsensitive);
	proxy_->setDynamicSortFilter(true);
	proxy_->sort(0);
	setModel(proxy_);*/
	QTreeView::setModel(m);

	connect(delegate_, SIGNAL(needUpdate(QModelIndex)), this, SLOT(update(QModelIndex)));

	connect(delegate_, SIGNAL(install(QModelIndex)), this, SIGNAL(install(QModelIndex)));
	connect(delegate_, SIGNAL(remove(QModelIndex)), this, SIGNAL(remove(QModelIndex)));
	connect(delegate_, SIGNAL(update(QModelIndex)), this, SIGNAL(updateItem(QModelIndex)));
	connect(delegate_, SIGNAL(enable(QModelIndex)), this, SIGNAL(enable(QModelIndex)));
	connect(delegate_, SIGNAL(disable(QModelIndex)), this, SIGNAL(disable(QModelIndex)));
	connect(delegate_, SIGNAL(detail(QModelIndex)), this, SIGNAL(detail(QModelIndex)));
	connect(delegate_, SIGNAL(cancel(QModelIndex)), this, SIGNAL(cancel(QModelIndex)));
	connect(delegate_, SIGNAL(restart(QModelIndex)), this, SIGNAL(restart()));

	connect(delegate_, SIGNAL(install(QModelIndex)), model(), SLOT(install(QModelIndex)));
	connect(delegate_, SIGNAL(remove(QModelIndex)), model(), SLOT(remove(QModelIndex)));
	connect(delegate_, SIGNAL(update(QModelIndex)), model(), SLOT(updateItem(QModelIndex)));
	connect(delegate_, SIGNAL(enable(QModelIndex)), model(), SLOT(enable(QModelIndex)));
	connect(delegate_, SIGNAL(disable(QModelIndex)), model(), SLOT(disable(QModelIndex)));
	connect(delegate_, SIGNAL(detail(QModelIndex)), model(), SLOT(detail(QModelIndex)));
	connect(delegate_, SIGNAL(cancel(QModelIndex)), model(), SLOT(cancel(QModelIndex)));

	connect(model(), SIGNAL(needUpdate(QModelIndex)), this, SLOT(update(const QModelIndex &)));
	connect(model(), SIGNAL(needUpdate(QModelIndex)), itemDelegate(), SIGNAL(sizeHintChanged(QModelIndex)));
}