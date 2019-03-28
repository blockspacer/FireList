#ifndef QVREMOTEOBJECTITEM_H
#define QVREMOTEOBJECTITEM_H

#include <QObject>
#include "model/QvListItem.h"
#include <QPointer>
#include <QUrl>

class QvDownloadDecorator;
class QNetworkAccessManager;

class QvRemoteObjectItem : public QvListItem
{
	Q_OBJECT
	
public:

	enum ObjectType {
		otNone = 0,
		otCity = 1,
		otPlugin,
		otUser = 100
	};

	QvRemoteObjectItem();
	~QvRemoteObjectItem();

	void setIconUrl(const QUrl & url);

	int progress() const;
	Qt::ItemFlags flags() const;

	void setNetworkAccessManager(QNetworkAccessManager * manager);
	QNetworkAccessManager *networkAccessManager() const;

public slots:
	void install();
	void cancel();


private slots:

	void iconDownloaded();

private:
	QPointer<QvDownloadDecorator> downloader_;
	QPointer<QNetworkAccessManager> manager_;
	
};

#endif // QVREMOTEOBJECTITEM_H
