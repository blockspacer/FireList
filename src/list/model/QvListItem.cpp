#include "QvListItem.h"

QvListItem::QvListItem( const QString & name /*= QString()*/, const QString & desc /*= QString()*/ )
{
	name_ = name;
	description_ = desc;
	installed_ = false;
	enabled_ = false;
	buildIn_ = false;
	updated_ = false;
	version_ = 0;
}

QString QvListItem::name() const
{
	return name_;
}

void QvListItem::setName( const QString &v )
{
	name_ = v;
}

QString QvListItem::description() const
{
	return description_;
}

void QvListItem::setDescription( const QString &v )
{
	description_ = v;
}

QString QvListItem::detail() const
{
	return detail_;
}

void QvListItem::setDetail( const QString &v )
{
	detail_ = v;
}

quint32 QvListItem::version() const
{
	return version_;
}

void QvListItem::setVersion( quint32 v )
{	
	version_ = v;
}

bool QvListItem::installed() const
{
	return installed_;
}

void QvListItem::setInstalled( bool v )
{
	installed_ = v;
}

bool QvListItem::enabled() const
{
	return enabled_;
}

void QvListItem::setEnabled( bool v )
{
	enabled_ = v;
}

bool QvListItem::buildIn() const
{
	return buildIn_;
}

void QvListItem::setBuildIn( bool v )
{
	buildIn_ = v;
}

QString QvListItem::source() const
{
	return source_;
}

void QvListItem::setSource( const QString &v )
{
	source_ = v;
}

QImage QvListItem::icon() const
{
	return icon_;
}

void QvListItem::setIcon( const QImage &v )
{
	icon_ = v;
}

QDate QvListItem::date() const
{
	return date_;
}

void QvListItem::setDate( const QDate &v )
{
	date_ = v;
}

int QvListItem::progress() const
{
	return -1;
}

QString QvListItem::guid() const
{
	return guid_;
}

void QvListItem::setGUid( const QString & guid )
{
	guid_ = guid;
}


void QvListItem::setHasUpdate( bool v )
{
	updated_ = v;
}