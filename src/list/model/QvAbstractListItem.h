#ifndef QvAbstractListItem_h__
#define QvAbstractListItem_h__
#include <QString>
#include <QImage>
#include <QDate>
#include <QObject>
#include <QPointer>

class QvAbstractListItem: public QObject
{
	Q_OBJECT

public:	
	enum { Downloading = Qt::ItemIsTristate << 1,
		DownloadedNeedRestart = Qt::ItemIsTristate << 2,
		RemovedNeedRestart = Qt::ItemIsTristate << 3,
		UpdatedNeedRestart = Qt::ItemIsTristate << 4,
		DisabledNeedRestart = Qt::ItemIsTristate << 5,
		EnabledNeedRestart = Qt::ItemIsTristate << 6,
		hasUpdate = Qt::ItemIsTristate << 7,
		canBeToggled = Qt::ItemIsTristate << 8
	};

	virtual ~QvAbstractListItem(void){};
	virtual QString name() const = 0;
	virtual QString description() const = 0;
	virtual QString detail() const = 0;
	virtual quint32 version() const = 0;
	virtual bool installed() const = 0;
	virtual bool enabled() const = 0;
	virtual bool buildIn() const = 0;
	virtual QString source() const = 0;
	virtual QImage icon() const = 0;
	virtual QDate date() const = 0;
	virtual QString guid() const = 0;
	virtual int progress() const {return 0;};

	virtual void setDescription(const QString&) = 0;
	virtual void setDetail(const QString&) = 0;
	virtual void setVersion(quint32) = 0;
	virtual void setSource(const QString&)= 0;
	virtual void setDate(const QDate&) = 0;
	
	virtual void setHasUpdate(bool) = 0;

	virtual Qt::ItemFlags flags() const { return Qt::NoItemFlags;};

signals:
	void needUpdate();

};
typedef QPointer< QvAbstractListItem > QvAbstractListItemPtr;

Q_DECLARE_INTERFACE(QvAbstractListItem, "ru.vmp.QvAbstractListItem")
#endif // QvAbstractListItem_h__
