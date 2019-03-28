#include "customitemwidget.h"
#include "ui_customitemwidget.h"

CustomItemWidget::CustomItemWidget(QWidget *parent) :
QWidget(parent),
ui(new Ui::CustomItemWidget)
{
ui->setupUi(this);

  ui->pushButton_3->installEventFilter(this);
}

CustomItemWidget::~CustomItemWidget()
{
delete ui;
}
