#include "QvDownloadDecorator.h"
#include <QUrl>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

QvDownloadDecorator::QvDownloadDecorator( const QString& dstPath, QNetworkAccessManager * manager, QObject * parent ):QObject(parent)
{
	dowloading_ = false;
	manager_ = manager;
	progress_ = 0;
	dstPath_ = dstPath;
	connect(this, SIGNAL(needUpdate()), parent, SIGNAL(needUpdate()));
}

QvDownloadDecorator::~QvDownloadDecorator()
{
	reset();
}

bool QvDownloadDecorator::downloading() const
{
	return dowloading_;
}

bool QvDownloadDecorator::ready() const
{
	return !dowloading_ && progress_ == 100;
}

void QvDownloadDecorator::download( const QUrl & url )
{

	QDir dstDir(dstPath_);	
	QFileInfo info_(url.path());
	QString fileName_ = QString("%1.part").arg(dstDir.absoluteFilePath(info_.fileName()));

	if(ready() && fileName_.indexOf(lastDownloaded_) != -1 ) // Если уже загружено, то не грузим
		return;

	abort();

	dstFile_ = new QFile(fileName_);
	if(dstFile_->open(QIODevice::WriteOnly) && manager_)
	{
		QNetworkRequest req(url);
		downloader_ = manager_->get(req);
		dowloading_ = !downloader_.isNull();

		connect(downloader_, SIGNAL(finished()),
			this, SLOT(downloadReady()));
		connect(downloader_, SIGNAL(downloadProgress(qint64, qint64)),
			this, SLOT(downloadProgress(qint64, qint64)));
	} else {
		qDebug() << dstFile_->errorString();
	}
}

void QvDownloadDecorator::downloadProgress( qint64 bytesSent, qint64 bytesTotal )
{
	QNetworkReply * repl = qobject_cast<QNetworkReply *>(sender());

	if(repl)
	{
		if(dstFile_)
		{
			dstFile_->write( repl->readAll() );
			dstFile_->flush();
		} else {
			repl->readAll();
		}
		int new_progress = ( bytesSent == bytesTotal ) ? 100 : (int(float(bytesSent) / float(bytesTotal) * 100));
		if(new_progress != progress_ && dowloading_)
		{
			progress_ = new_progress;
			emit needUpdate();
		}		
	}
}

void QvDownloadDecorator::downloadReady()
{
	dowloading_ = false;
	if(dstFile_)
	{
		dstFile_->close();
		QString newName = dstFile_->fileName();
		newName	= newName.left(newName.length() - 5);
		lastDownloaded_ = newName;
		dstFile_->rename(newName);
		delete dstFile_;
	}
	emit needUpdate();
}

void QvDownloadDecorator::abort()
{
	reset();
	if(!lastDownloaded_.isEmpty())
	{
		QFile::remove(lastDownloaded_);
	}
}

int QvDownloadDecorator::progress() const
{
	return progress_;
}

QString QvDownloadDecorator::lastDownloaded() const
{
	return lastDownloaded_;
}

void QvDownloadDecorator::reset()
{
	dowloading_ = false;
	progress_ = 0;

	if( downloader_ )
	{
		downloader_->abort();
	}

	if( dstFile_ )
	{
		dstFile_->close();
		dstFile_->remove();
		delete dstFile_;
	}
}

void QvDownloadDecorator::setNetworkAccessManager( QNetworkAccessManager * manager )
{
	manager_ = manager;
}

QNetworkAccessManager * QvDownloadDecorator::networkAccessManager() const
{
	return manager_;
}