#ifndef QvDownloadDecorator_h__
#define QvDownloadDecorator_h__

#include <QObject>
#include <QPointer>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>

class QvDownloadDecorator: public QObject
{
	Q_OBJECT
	QString dstPath_;
public:
	QvDownloadDecorator(const QString& dstPath, QNetworkAccessManager * manager, QObject * parent);
	~QvDownloadDecorator();

	bool downloading() const;
	bool ready() const;

	void download(const QUrl & url);
	void abort();

	
	int progress() const;
	QString lastDownloaded() const;

	void setNetworkAccessManager(QNetworkAccessManager * manager);
	QNetworkAccessManager *networkAccessManager() const;

signals:
	void needUpdate();

private slots:
	void downloadProgress ( qint64 bytesSent, qint64 bytesTotal );
	void downloadReady();

private:
	void reset();
	QPointer<QNetworkReply> downloader_;
	QPointer<QNetworkAccessManager> manager_;
	QPointer<QFile> dstFile_;
	QString lastDownloaded_;
	bool dowloading_;
	int progress_;
};
#endif // QvDownloadDecorator_h__