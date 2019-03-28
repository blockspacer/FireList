#ifndef CUSTOMITEMWIDGET_H
#define CUSTOMITEMWIDGET_H

#include <QWidget>

namespace Ui {
class CustomItemWidget;
}

class CustomItemWidget : public QWidget
{
Q_OBJECT

public:
explicit CustomItemWidget(QWidget *parent = nullptr);
~CustomItemWidget();

//private:
public:
Ui::CustomItemWidget *ui;
};

#endif // CUSTOMITEMWIDGET_H
