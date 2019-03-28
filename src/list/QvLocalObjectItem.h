#ifndef QvLocalObjectItem_h__
#define QvLocalObjectItem_h__

#include "model/QvListItem.h"
#include <QString>
#include <QPointer>
#include <QImage>

class QvDownloadDecorator;
class QNetworkAccessManager;

class QvLocalObjectItem : public QvListItem
{
	Q_OBJECT;
	Q_INTERFACES(QvAbstractListItem);
	QPointer<QvDownloadDecorator> downloader_;
	bool needRestart_, previosValue_, updated_, needRemove_;	
public:
	QvLocalObjectItem();
	~QvLocalObjectItem();

	QString guid() const;
	int progress() const;

	Qt::ItemFlags flags() const;

	void setHasUpdate(bool v);

	void setNetworkAccessManager(QNetworkAccessManager * manager);
	QNetworkAccessManager *networkAccessManager() const;

public slots:
	void enable();
	void disable();
	void cancel();
	void update();
	void remove();

};
#endif // QvLocalObjectItem_h__