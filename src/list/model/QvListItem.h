#ifndef QvListItem_h__
#define QvListItem_h__

#include "QvAbstractListItem.h"
#include <QString>
#include <QImage>

class QvListItem : public QvAbstractListItem
{
	QString name_, description_, detail_, source_, guid_;
	bool installed_, enabled_, buildIn_;
	QImage icon_;
	quint32 version_;
	QDate date_;
protected:
	bool updated_;
public:
	QvListItem(const QString & name = QString(), const QString & desc = QString());
	virtual ~QvListItem(){};

	QString name() const;
	void setName(const QString &v);

	QString description() const;
	void setDescription(const QString &v);

	QString detail() const;
	void setDetail(const QString &v);

	quint32 version() const;
	void setVersion(quint32 v);

	bool installed() const;
	void setInstalled(bool v);

	bool enabled() const;
	void setEnabled(bool v);

	bool buildIn() const;
	void setBuildIn(bool v);

	QString source() const;
	void setSource(const QString &v);

	QImage icon() const;
	void setIcon(const QImage &v);

	QDate date() const;
	void setDate(const QDate &v);

	QString guid() const;
	void setGUid(const QString & guid);

	int progress() const;

	void setHasUpdate(bool v);

};
#endif // QvListItem_h__