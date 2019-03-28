#include "QvRemoteObjectItem.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include "QvDownloadDecorator.h"

QvRemoteObjectItem::QvRemoteObjectItem()
{
	downloader_ = new QvDownloadDecorator("", 0, this);
}

QvRemoteObjectItem::~QvRemoteObjectItem()
{
	
}

void QvRemoteObjectItem::setIconUrl( const QUrl & url )
{
	if( !url.isEmpty())
	{
		QNetworkRequest req(url);
		QNetworkReply * repl = manager_->get(req);
		connect(repl, SIGNAL(finished()),
			this, SLOT(iconDownloaded()));
		connect(repl, SIGNAL(finished()),
			repl, SLOT(deleteLater()));
	}
}

void QvRemoteObjectItem::iconDownloaded()
{
	QNetworkReply * repl = qobject_cast<QNetworkReply *>(sender());
	if( repl && repl->error() == QNetworkReply::NoError )
	{
		setIcon(QImage::fromData(repl->readAll()));
		repl->close();
		emit needUpdate();
	}
}

void QvRemoteObjectItem::install()
{
	if( !source().isEmpty())
	{
		downloader_->download(source());
	} else {
		qDebug() << "SOURCE empty";
	}
}


int QvRemoteObjectItem::progress() const
{
	return downloader_->progress();
}

Qt::ItemFlags QvRemoteObjectItem::flags() const
{
	Qt::ItemFlags f;
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

void QvRemoteObjectItem::cancel()
{
	downloader_->abort();
	emit needUpdate();
}

void QvRemoteObjectItem::setNetworkAccessManager( QNetworkAccessManager * manager )
{
	manager_ = manager;
	if(downloader_)
	{
		downloader_->setNetworkAccessManager(manager);
	}
}

QNetworkAccessManager * QvRemoteObjectItem::networkAccessManager() const
{
	return manager_;
}

