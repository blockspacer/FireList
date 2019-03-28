#include <QApplication>
#include "list/QvObjectListView.h"
#include "list/model/qvobjectmodel.h"
#include "list/QvRemoteObjectItem.h"
#include "list/QvLocalObjectItem.h"
#include "QNetworkAccessManager"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QvObjectModel model(0);
	QvObjectListView w(0);
	QNetworkAccessManager manager;

	// Пример с кнопкой загрузки
	QvRemoteObjectItem * r_item = new QvRemoteObjectItem();

	r_item->setNetworkAccessManager(&manager);
	r_item->setName("Item 1");
	r_item->setDescription("Download small image file as on icon");
	r_item->setDate(QDate::currentDate());
  r_item->setSource("https://ddgobkiprc33d.cloudfront.net/bb9f67d1-3dc4-46a8-a767-fc0da3035553.png");
  r_item->setIconUrl(QString("https://ddgobkiprc33d.cloudfront.net/bb9f67d1-3dc4-46a8-a767-fc0da3035553.png"));
	w.setModel(&model);
	model.addSection("Remote item");
	model.addInSection(0, r_item);

	// Пример с кнопками обновления, выключения/включения

	model.addSection("Locale item");

	{
		QvLocalObjectItem * l_item = new QvLocalObjectItem();
		l_item->setInstalled(true);
		l_item->setBuildIn(true);
		l_item->setNetworkAccessManager(&manager);
		l_item->setName("Item 1");
		l_item->setDescription("Local item 1");
		l_item->setDate(QDate::currentDate());
		l_item->setDetail("Test");
		
		model.addInSection(1, l_item);
	}

	{
		QvLocalObjectItem * l_item = new QvLocalObjectItem();
		l_item->setInstalled(true);
		l_item->setNetworkAccessManager(&manager);
		l_item->setName("Item 2");
		l_item->setDescription("Local item 2");
		l_item->setDate(QDate::currentDate());
		l_item->setEnabled(true);
		model.addInSection(1, l_item);
	}

	{
		QvLocalObjectItem * l_item = new QvLocalObjectItem();
		l_item->setInstalled(true);
		l_item->setNetworkAccessManager(&manager);
		l_item->setName("Item 3");
		l_item->setDescription("Local item 3. Download big file (FreeBSD-8.2-RELEASE-i386-bootonly.iso)");
		l_item->setDate(QDate());
		l_item->setVersion(1020300045);
		l_item->setEnabled(true);
		l_item->setHasUpdate(true);
    l_item->setSource("https://ddgobkiprc33d.cloudfront.net/bb9f67d1-3dc4-46a8-a767-fc0da3035553.png");
		model.addInSection(1, l_item);
	}

	w.setDefaultIcon(QImage(":/Resources/plug.png"));
	w.expandAll();
	w.show();
	return a.exec();
}
