#include "QvLocalObjectItem.h"
#include <QDebug>
#include "QvDownloadDecorator.h"
#include <QUuid>

QvLocalObjectItem::QvLocalObjectItem(  )
{	
	previosValue_ = false;
	needRestart_ = false;
	needRemove_ = false;
	updated_ = false;
	downloader_ = new QvDownloadDecorator("", 0, this);
}


int QvLocalObjectItem::progress() const
{
	return downloader_->progress();
}

void QvLocalObjectItem::enable()
{
	qDebug() << "ENABLE TOOL";
	
	previosValue_ = true;
	needRestart_ = true;
	emit needUpdate();
}

void QvLocalObjectItem::disable()
{
	previosValue_ = false;
	needRestart_ = true;
	emit needUpdate();
}

Qt::ItemFlags QvLocalObjectItem::flags() const
{
	Qt::ItemFlags f;

	f|= Qt::ItemFlag(canBeToggled);

	if(needRestart_)
	{
		if(needRemove_)
		{
			f |= Qt::ItemFlag(RemovedNeedRestart);
		} else {
			f |= enabled() ? Qt::ItemFlag(DisabledNeedRestart) : Qt::ItemFlag(EnabledNeedRestart);
		}		
	}

	if(updated_)
	{
		f|= Qt::ItemFlag(hasUpdate);
	}



	if(downloader_->downloading())
	{
		f |= Qt::ItemFlag(Downloading);
	}

	if(downloader_->ready())
	{
		f |= Qt::ItemFlag(DownloadedNeedRestart);
	}

	return f;
}

void QvLocalObjectItem::cancel()
{
	needRestart_ = false;
	previosValue_ = false;
	needRemove_ = false;
	downloader_->abort();
	emit needUpdate();

}

QvLocalObjectItem::~QvLocalObjectItem()
{
	
}

QString QvLocalObjectItem::guid() const
{
  return QUuid::createUuid().toString();
}

void QvLocalObjectItem::update()
{
	downloader_->download(source());
}

void QvLocalObjectItem::setHasUpdate( bool v )
{
	updated_ = v;
}

void QvLocalObjectItem::setNetworkAccessManager( QNetworkAccessManager * manager )
{
	downloader_->setNetworkAccessManager(manager);
}

QNetworkAccessManager * QvLocalObjectItem::networkAccessManager() const
{
	return downloader_->networkAccessManager();
}

void QvLocalObjectItem::remove()
{
	needRemove_ = true;
	needRestart_ = true;
	emit needUpdate();
}
