#include "QvObjectDelegate.h"
#include "customitemwidget.h"
#include "ui_customitemwidget.h"
#include <QPainter>
#include <QApplication>
#include <QStyleOptionButton>
#include <QStyleOptionProgressBarV2>
#include <QSortFilterProxyModel>
#include <QMouseEvent>
#include <QStyle>
#include <QDate>
#include <QDebug>
#include <QLinearGradient>

#include "QvAbstractListItem.h"

#define ICON_SIZE 32
#define OFFSET_H 16
#define CHECK_WIDTH 50
#define PROGRESS_HEIGHT 21
#define OFFSET_BUTTON 5
#define DETAIL_OFFSET 30

#define ITEM_HEIGHT 64
#define HEADER_HEIGHT 24

#define STRIPE_WIDTH 30

QvObjectDelegate::QvObjectDelegate(QObject *parent)
	: QItemDelegate(parent)
{
	QFontMetrics fm(QApplication::fontMetrics());
	QRect installRect = fm.boundingRect(QCoreApplication::translate("list", "Install")).adjusted(-20, -5, 20, 5),
		deleteRect = fm.boundingRect(QCoreApplication::translate("list", "Remove")).adjusted(-20, -5, 20, 5),
		updateRect = fm.boundingRect(QCoreApplication::translate("list", "Update")).adjusted(-20, -5, 20, 5),
		enableRect = fm.boundingRect(QCoreApplication::translate("list", "Enable")).adjusted(-20, -5, 20, 5),
		disableRect = fm.boundingRect(QCoreApplication::translate("list", "Disable")).adjusted(-20, -5, 20, 5);
	
	buttonRect = installRect.width() > deleteRect.width() ? installRect : deleteRect;
	buttonRect = buttonRect.width() > updateRect.width() ? buttonRect : updateRect;
	buttonRect = buttonRect.width() > enableRect.width() ? buttonRect : enableRect;
	buttonRect = buttonRect.width() > disableRect.width() ? buttonRect : disableRect;
	buttonRect.moveTo(0, 0);

	labelHover_ = false;

}

QvObjectDelegate::~QvObjectDelegate()
{

}

void QvObjectDelegate::paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	painter->save();
	if(index.parent().isValid())
	{
		if(needRestart(index))
		{

			drawItemBackground(painter, option, index);
			
			paintObjectHeader(painter, option, index);
		}
		paintObject(painter, option, index);
	} else 
	{
		paintHeader(painter, option, index);
	}	
	painter->restore();
	painter->save();
	painter->setPen(QColor(0xD7, 0xD7, 0xD7));
	painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());

	painter->restore();
}

QSize QvObjectDelegate::sizeHint( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	if(!index.parent().isValid())
	{
		return QSize(-1, HEADER_HEIGHT);
	} else {
		int height_ = ITEM_HEIGHT;
		if(needRestart(index))
		{
			height_ += HEADER_HEIGHT;
		}

		return QSize(-1, height_);
	}
}

void QvObjectDelegate::paintObject(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{	
	QRect tr;
	QString name = index.data(Qt::DisplayRole).toString(),
		description = index.data(QvObjectModel::DescriptionRole).toString();
	QPainter &p = *painter;
	p.setClipRect(option.rect);

	p.setPen(QColor(210, 210, 210));
	p.setBrush(QColor(240, 240, 240));

	p.setPen(QColor(77, 77, 77));	
	p.translate(option.rect.topLeft());

	p.translate(0, sizeHint(option, index).height() - ITEM_HEIGHT);

	p.translate(OFFSET_H, OFFSET_H);

	QImage img = index.data(Qt::DecorationRole).value<QImage>();
	if(!img.isNull())
	{
		p.drawImage(0,0, img);
	} else {
		p.drawImage(0,0, defaultIcon_);
	}	

	p.translate(ICON_SIZE + OFFSET_H, 0); // отступили от иконки на 10px

	// рисуем текст
	QFont f = option.font;
	f.setPointSize(10);
	f.setWeight(QFont::Bold);
	QFontMetrics fm(f);
	tr = fm.boundingRect(name);
	p.setFont(f);
	p.drawText(0, tr.height()-5, name);

	// рисуем описание

	p.setFont(option.font);
	fm = QFontMetrics(option.font);

	QDate date_ = index.data(QvObjectModel::DateRole).toDate();
	int version_ = index.data(QvObjectModel::VersionRole).toInt();
	QString versionStr_;
	if(!date_.isNull())
	{
		versionStr_ = date_.toString("dd MMMM yyyy");

	} else if(version_ > 1000000000){
		int ver_min = 0;
		int ver = version_ / 1000000000;
		ver_min = version_ % 1000000000;

		int major = ver_min / 10000000;
		ver_min = ver_min % 10000000;

		int minor = ver_min / 100000;
		ver_min = ver_min % 100000;

		versionStr_ = QCoreApplication::translate("list", "%1.%2.%3.%4", "Version in list")
			.arg(ver).arg(major, 2, 10, QLatin1Char('0') )
			.arg(minor, 2, 10, QLatin1Char('0')).arg(ver_min);
	}

	if(!versionStr_.isEmpty())
	{
		tr = fm.boundingRect(versionStr_);
		tr.moveTo(option.rect.width() - ICON_SIZE - 2*OFFSET_H - tr.width() - OFFSET_BUTTON, 0 );
		painter->drawText(tr, Qt::TextSingleLine, versionStr_);
	}

	int maxWidth(option.rect.width() - widthButtonGroup(index) - ICON_SIZE - OFFSET_H - DETAIL_OFFSET );
	if(!index.data(QvObjectModel::DetailRole).toString().isEmpty())
	{
		maxWidth -= fm.boundingRect(QCoreApplication::translate("list", "Detail")).width();
	}

	description = fm.elidedText(description, Qt::ElideRight, maxWidth);
	p.translate(0, ICON_SIZE / 2);
	tr = fm.boundingRect(description);
	p.drawText(0, tr.height(), description);
	if(!index.data(QvObjectModel::DetailRole).toString().isEmpty())
	{
		paintDetail(painter, option, index);
	}
	
	if(index.flags().testFlag(Qt::ItemFlag(QvAbstractListItem::Downloading)))
	{	
		paintObjectProgress(painter, option, index);
	} //else {
		paintObjectBtn(painter, option, index);
  //}


/*

  QRect item_widget_rect(0, 0, 200, 200);
  QPoint item_pos(0, 0);
  QTransform trans = painter->combinedTransform();
  QRect transformed_item_widget_rect = trans.mapRect(item_widget_rect);
  QPoint transformed_item_widget_pos = trans.map(item_pos);
  // we have to save paint device of the provided painter to restore the painter
  // after drawing QWidget
  QPaintDevice* original_pdev_ptr = painter->device();
  CustomItemWidget item_widget;
  item_widget.setGeometry(o.rect);
  //item_widget.setGeometry(QRect(option.rect.x(),option.rect.y(),option.rect.x()+800,option.rect.y()+800));
  // example of simple painting after widget
  // painter->drawEllipse(0, 0, 100, 100);
  // here we have to finish the painting of provided painter, cause
  //     1) QWidget::render(QPainter *,...) doesn't work with provided external painter
  //          and we have to use QWidget::render(QPaintDevice *,...)
  //          which creates its own painter
  //     2) two painters can't work with the same QPaintDevice at the same time
  QPoint mappedp = painter->deviceTransform().map(QPoint(p.x(), p.y()));
  QPoint mappedorigin = painter->deviceTransform().map(QPoint(o.rect.x(), o.rect.y()));
  QRect mappedorigin_rect = painter->deviceTransform().mapRect(o.rect);
  //item_widget.render(painter->device(), mappedorigin, QRegion(0, 0, o.rect.width(), o.rect.height()), QWidget::RenderFlag::DrawChildren);
  item_widget.render(painter->device(), mappedp, QRegion(0, 0, 100, 100), QWidget::RenderFlag::DrawChildren);
  painter->end();
  // rendering of QWidget itself
  //item_widget.render(painter->device(), QPoint(transformed_item_widget_rect.x(), transformed_item_widget_rect.y()), QRegion(0, 0, option.rect.width(), option.rect.height()), QWidget::RenderFlag::DrawChildren);
  //item_widget.render(painter->device(), mappedorigin, QRegion(0, 0, o.rect.width(), o.rect.height()), QWidget::RenderFlag::DrawChildren);
  // starting (in fact just continuing) painting with external painter, provided
  // by caller
  painter->begin(original_pdev_ptr);
  //o.rect.translate(0, 200);
*/


}

void QvObjectDelegate::paintHeader( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	
	QPainter &p = *painter;
	p.save();
	p.setClipRect(option.rect);
	p.setPen(QColor(77, 77, 77));
	// рисуем текст
	QRect tr;
	QString name = index.data(Qt::DisplayRole).toString(),
		desc = index.data(QvObjectModel::DetailRole).toString();

	QFont f = option.font;	
	f.setPointSize(12);
	f.setWeight(QFont::Bold);
	QFontMetrics fm(f);
	tr = fm.boundingRect(name);
	p.setFont(f);
	p.drawText(option.rect, Qt::AlignVCenter | Qt::AlignLeft, name);

	f = option.font;	
	f.setWeight(QFont::DemiBold);
	p.setFont(f);
	p.drawText(option.rect, Qt::AlignVCenter | Qt::AlignRight, desc);

	p.restore();
}

bool QvObjectDelegate::editorEvent( QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index )
{
	Q_ASSERT(event);
	Q_ASSERT(model);
	// make sure that we have the right event type
	if ((event->type() == QEvent::MouseButtonRelease)
		|| (event->type() == QEvent::MouseMove)
		|| (event->type() == QEvent::MouseButtonPress))
	{
		return validateLabel(index, option, event) || validateButton(index, option, event);
	} else if (event->type() == QEvent::KeyPress) {
		if (static_cast<QKeyEvent*>(event)->key() != Qt::Key_Space
			&& static_cast<QKeyEvent*>(event)->key() != Qt::Key_Select)
			return false;
	} else {
		return false;
	}	
	return false; 
}

void QvObjectDelegate::paintObjectBtn( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	painter->restore();
	painter->save();

	QVector<ButtonAction> tags_ =  getButtons(index);
	int width_ = 0;

	if(index.flags().testFlag(Qt::ItemFlag(QvAbstractListItem::Downloading)))
	{
		width_ = buttonRect.width()  + 4;
	} else {
		width_ = widthButtonGroup(index);
	}

	int x = option.rect.right() - width_,
		y = option.rect.bottom() - buttonRect.height() - 5;
	painter->translate(x, y);

	QPoint pPoint(buttonPressedPoint_.x() - x, buttonPressedPoint_.y() -y );

  qDebug() << pPoint;

	QStyle * style = QApplication::style();
	QStyleOptionButton opt;
	opt.rect  = buttonRect;
	opt.state |= QStyle::State_Active;
	opt.state |= QStyle::State_Enabled;

	foreach (const ButtonAction & act, tags_)
	{
		switch(act)
		{
		case baInstall:
			opt.text = QCoreApplication::translate("list", "Install");
			break;
		case baRemove:
			opt.text = QCoreApplication::translate("list", "Remove");
			break;
		case baUpdate:
			opt.text = QCoreApplication::translate("list", "Update");
			break;
		case baEnable:
			opt.text = QCoreApplication::translate("list", "Enable");
			break;
		case baDisable:	
			opt.text =  QCoreApplication::translate("list", "Disable");
			break;
		case baCancel:
			opt.text =  QCoreApplication::translate("list", "Cancel");
			break;
		case baRestart:
			opt.text =  QCoreApplication::translate("list", "Restart");
			break;
		default:
			opt.text =  QCoreApplication::translate("list", "Unknown");
			break;
		}
		drawButton(opt, pPoint, painter );
	}
}

void QvObjectDelegate::drawButton( QStyleOptionButton &o, const QPoint &p, QPainter * painter ) const
{
  if(o.rect.contains(p))
    o.state |= QStyle::State_Sunken;
	
	QStyle * style = QApplication::style();
  /*if(style)
    style->drawControl(QStyle::CE_PushButton, &o, painter );*/




  //QPoint item_pos(0, 0);
  QPoint item_pos(o.rect.x(), o.rect.y());
  QTransform trans = painter->combinedTransform();
  QPoint transformed_item_widget_pos = trans.map(item_pos);
  // we have to save paint device of the provided painter to restore the painter
  // after drawing QWidget
  QPaintDevice* original_pdev_ptr = painter->device();
  CustomItemWidget item_widget;
  //item_widget.setGeometry(o.rect);
  QRect item_widget_rect(item_widget.geometry());
  QRect transformed_item_widget_rect = trans.mapRect(item_widget_rect);
  QRegion tra(0, 0, o.rect.width(), o.rect.height());
  //item_widget.setGeometry(QRect(0,0,10,10));
  item_widget.setGeometry(QRect(0,0,o.rect.width(), o.rect.height()));

  //item_widget.ui->pushButton_3->installEventFilter(&item_widget);



  //item_widget.ui->pushButton_3->setDown(o.state.testFlag(QStyle::State_MouseOver));
  if(o.rect.contains(p)) {
    item_widget.ui->pushButton_3->setDown(true);
  }
/*
  if(o.state.testFlag(QStyle::State_MouseOver)){
    qDebug() << "o.state & QStyle::State_MouseOver";
  }
*/
  /*
  QMouseEvent *e = static_cast<QMouseEvent *> (event);
    mouse_position_ = e->pos();
    if (o.rect.contains(mouse_position_)) {
        repaint_cell  = true;

        // Check if the user has clicked in this area
        if (e->button() == Qt::LeftButton) {

            switch(event->type()) {
                case QEvent::MouseButtonRelease:
                    // Update model data in here
                    break;
                default:
                    break;
            }
        }
    }*/

  /*item_widget.ui->pushButton_3->setFlat(o.state & QStyle::State_FocusAtBorder);
  item_widget.ui->pushButton_3->initStyleOption(o);*/

  //item_widget.setGeometry(QRect(option.rect.x(),option.rect.y(),option.rect.x()+800,option.rect.y()+800));
  // example of simple painting after widget
  // painter->drawEllipse(0, 0, 100, 100);
  // here we have to finish the painting of provided painter, cause
  //     1) QWidget::render(QPainter *,...) doesn't work with provided external painter
  //          and we have to use QWidget::render(QPaintDevice *,...)
  //          which creates its own painter
  //     2) two painters can't work with the same QPaintDevice at the same time
  //painter->end();

  painter->save();
  // rendering of QWidget itself
  //QPoint mappedorigin = painter->deviceTransform().map(QPoint(o.rect.x(),o.rect.y()));
  //item_widget.render(painter->device(), QPoint(transformed_item_widget_rect.x(), transformed_item_widget_rect.y()), QRegion(0, 0, option.rect.width(), option.rect.height()), QWidget::RenderFlag::DrawChildren);
  //item_widget.render(painter->device(), mappedorigin, item_widget.geometry(), QWidget::RenderFlag::DrawChildren);

  //item_widget.render(painter->device(), transformed_item_widget_pos, item_widget.geometry(), QWidget::RenderFlag::DrawChildren);
 item_widget.ui->pushButton_3->setText(o.text);

 item_widget.render(painter->device(), transformed_item_widget_pos, tra, QWidget::RenderFlag::DrawChildren);
   // starting (in fact just continuing) painting with external painter, provided
  // by caller
  //painter->begin(original_pdev_ptr);

  painter->restore();

  //p.translate(0, 200);


  o.state &= ~QStyle::State_Sunken;
  o.rect.translate(buttonRect.width() + OFFSET_BUTTON, 0);
}

void QvObjectDelegate::paintDetail( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	QString detail_ = QCoreApplication::translate("list", "Detail");
	QRect r = painter->fontMetrics().boundingRect(detail_);
	if(!labelHover_)	
	{
		painter->setPen( QColor(0x00, 0x66, 0xCC) );
	} else {
		//painter->setPen( QColor(Qt::red) );
		painter->setPen( QColor(0x3D, 0xA1, 0xFF) );
	}

	QString description = index.data(QvObjectModel::DescriptionRole).toString();
	int maxWidth(option.rect.width() - widthButtonGroup(index) - ICON_SIZE - OFFSET_H - DETAIL_OFFSET );
	maxWidth -= option.fontMetrics.boundingRect(QvObjectDelegate::tr("Detail")).width();

	description = option.fontMetrics.elidedText(description, Qt::ElideRight, maxWidth);
	QRect tr = option.fontMetrics.boundingRect(description);
	r.moveTo(tr.width() + 10, 3);
	painter->drawText(r, Qt::TextSingleLine, detail_);

	r.translate(1, -1);
	painter->drawLine(r.bottomLeft(), r.bottomRight() );
}

bool QvObjectDelegate::validateButton( const QModelIndex &index, const QStyleOptionViewItem &option, QEvent * event )
{
	QStyle *style = QApplication::style();
	QRect checkRect(buttonRect);
	QSize checkSize = checkRect.size();

	QVector<ButtonAction> tags_ = getButtons(index);

	int width_ = tags_.count() * buttonRect.width() + (tags_.count() - 1) * OFFSET_BUTTON + 4;
	checkRect.translate(option.rect.right() - width_,
		option.rect.bottom() - buttonRect.height() - 5);
	tags_ << baNone;
	QMouseEvent *me = static_cast<QMouseEvent*>(event);
	if(event->type() == QEvent::MouseButtonRelease)			
	{
		buttonPressedPoint_ = QPoint();
	}

	if (me->button() == Qt::LeftButton)
	{
		int pos_;			
		for (pos_ = 0; pos_ < (tags_.count() - 1); ++pos_)
		{
			if(checkRect.contains(me->pos())) 
			{

				break;
			}
			checkRect.translate( checkRect.width() + OFFSET_BUTTON, 0 );
		}
		if(event->type() == QEvent::MouseButtonPress)
		{
			if(tags_[pos_] != baNone)
				buttonPressedPoint_ = me->pos();
      //  option.state.setFlag(QStyle::State_MouseOver);
			return true;
		}		

		switch(tags_[pos_])
		{
		case baInstall:
			emit install(index);
			qDebug() << "INSTALL"; break;
		case baRemove:
			emit remove(index);
			qDebug() << "REMOVE"; break;
		case baUpdate:
			emit update(index);
			qDebug() << "UPDATE"; break;
		case baEnable:
			emit enable(index);
			qDebug() << "ENABLE"; break;
		case baDisable:
			emit disable(index);
			qDebug() << "DISABLE"; break;
		case baCancel:
			emit cancel(index);
			qDebug() << "CANCEL"; break;
		case baRestart:
			emit restart(index);
			qDebug() << "RESTART"; break;
		default:
			buttonPressedPoint_ = QPoint();
      //  option.state.setFlag(QStyle::State_MouseOver);
			qDebug() << "MISS";	
			return false;
		}
		return true;
	} else {
		return false;
	}
}

bool QvObjectDelegate::validateLabel( const QModelIndex &index, const QStyleOptionViewItem &option, QEvent * event ) 
{
	if(index.data(QvObjectModel::DetailRole).toString().isEmpty())
	{
		return false;
	}


	QString description = index.data(QvObjectModel::DescriptionRole).toString();
	QString detail_ = QCoreApplication::translate("list", "Detail");
	int maxWidth(option.rect.width() - widthButtonGroup(index) - ICON_SIZE - OFFSET_H - DETAIL_OFFSET );
	maxWidth -= option.fontMetrics.boundingRect(detail_).width();
	
	description = option.fontMetrics.elidedText(description, Qt::ElideRight, maxWidth);

	QRect tr = option.fontMetrics.boundingRect(description),
		r = option.fontMetrics.boundingRect(detail_);

	r.moveTo(option.rect.topLeft()); //Сместились в начало элемента
	r.translate(OFFSET_H, OFFSET_H);
	r.translate(ICON_SIZE + OFFSET_H, 0); // отступили от иконки на 10px
	r.translate(0, ICON_SIZE / 2 );

	if( needRestart(index) )
	{
		r.translate(0, HEADER_HEIGHT);
	}

	r.translate(tr.width() + 10, 3);

	QMouseEvent *me = static_cast<QMouseEvent*>(event);	
	if(r.contains(me->pos()))
	{		
		if(!labelHover_)
		{
			labelHover_ = true;
			QApplication::setOverrideCursor(Qt::PointingHandCursor);
			emit needUpdate(index);
		}
		if(me->type() == QEvent::MouseButtonRelease)
		{
			emit detail(index);
			//qDebug() << "DETAIL";
		}
	} else {
		if(labelHover_)
		{
			labelHover_ = false;
			QApplication::restoreOverrideCursor();
			emit needUpdate(index);
		}
	}
	
	return labelHover_;
}

int QvObjectDelegate::widthButtonGroup( const QModelIndex &index ) const
{
	if(!index.flags().testFlag(Qt::ItemFlag(QvAbstractListItem::Downloading)))
	{
		int count_ = getButtons(index).count();
		if(count_)
		{
			return count_ * buttonRect.width() + (count_ - 1) * OFFSET_BUTTON + 4;
		} else {
			return 4;
		}
		
	} else {
		return CHECK_WIDTH * 2 + 4 + buttonRect.width() + OFFSET_BUTTON;
	}
}

QImage QvObjectDelegate::defautIcon() const 
{
	return defaultIcon_;
}

void QvObjectDelegate::setDefaultIcon( const QImage & img )
{
	defaultIcon_ = img;
}

void QvObjectDelegate::paintObjectProgress( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	QStyleOptionProgressBarV2 opt;
	initStyleProgressOption(&opt, index);
	QStyle *style = QApplication::style();
	opt.rect.setLeft(option.rect.width() - (CHECK_WIDTH + OFFSET_H) * 2 - ICON_SIZE - OFFSET_BUTTON - buttonRect.width() - OFFSET_BUTTON );
	opt.rect.setTop( 4 );
	opt.rect.setWidth(CHECK_WIDTH * 2);
	opt.rect.setHeight(PROGRESS_HEIGHT);
	painter->setPen(QColor(77, 77, 77));	
	style->drawControl(QStyle::CE_ProgressBar, &opt, painter);
}

void QvObjectDelegate::initStyleProgressOption( QStyleOptionProgressBar *option, const QModelIndex & index ) const
{

	int value = index.data(QvObjectModel::ProgressRole).toInt();

	if (!option)
		return;
	
	option->rect = QRect(100, 100, 100, 100);
	option->state |= QStyle::State_Active;
	option->state |= QStyle::State_Enabled;
	option->state |= QStyle::State_Horizontal;
	option->minimum = 0;
	option->maximum = 100; //maximum?maximum:100;
	option->progress = value;
	option->textAlignment = Qt::AlignCenter;
	option->textVisible = true;
	option->text = QString("%1%").arg(value);

	if (QStyleOptionProgressBarV2 *optionV2
		= qstyleoption_cast<QStyleOptionProgressBarV2 *>(option)) {
			optionV2->orientation = Qt::Horizontal ;  // ### Qt 5: use State_Horizontal instead
			optionV2->invertedAppearance = false;
			optionV2->bottomToTop = true;
	}
}

void QvObjectDelegate::paintObjectHeader( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	painter->save();
	painter->translate(option.rect.topLeft());
	QRect clipRect = option.rect;
	clipRect.setHeight(HEADER_HEIGHT);
	clipRect.moveTo(0,0);
	painter->setClipRect(clipRect);
	
	painter->translate(ICON_SIZE + 2 * OFFSET_H, 0);
	QString msg_;
	if( index.flags() & Qt::ItemFlag(QvAbstractListItem::DownloadedNeedRestart) )
		msg_ = QCoreApplication::translate("list", "%1 downloaded and will be installed after restart.");

	if( index.flags() & Qt::ItemFlag(QvAbstractListItem::RemovedNeedRestart) )
		msg_ = QCoreApplication::translate("list", "%1 will be removed after restart.");

	if( index.flags() & Qt::ItemFlag(QvAbstractListItem::UpdatedNeedRestart) )
		msg_ = QCoreApplication::translate("list", "%1 will be updated after restart.");

	if( index.flags() & Qt::ItemFlag(QvAbstractListItem::DisabledNeedRestart) )
		msg_ = QCoreApplication::translate("list", "%1 will be disabled after restart.");

	if( index.flags() & Qt::ItemFlag(QvAbstractListItem::EnabledNeedRestart) )
		msg_ = QCoreApplication::translate("list", "%1 will be enabled after restart.");

	msg_ = QString(msg_).arg(index.data().toString());

	QFont f(option.font);
	f.setBold(true);
	painter->setFont(f);

	QColor baseColor;
	if( 
		(index.flags() & Qt::ItemFlag(QvAbstractListItem::DownloadedNeedRestart))
		|| (index.flags() & Qt::ItemFlag(QvAbstractListItem::UpdatedNeedRestart))
		)
		baseColor = QColor(0x4C, 0xBB,0x17);

	if(
		(index.flags() & Qt::ItemFlag(QvAbstractListItem::EnabledNeedRestart))
		|| (index.flags() & Qt::ItemFlag(QvAbstractListItem::DisabledNeedRestart))
		|| (index.flags() & Qt::ItemFlag(QvAbstractListItem::RemovedNeedRestart))
		)
		baseColor = QColor(0xBa, 0xA4,0x16);

	painter->setPen(baseColor.darker(125));
	clipRect.setWidth(clipRect.width() - ICON_SIZE - OFFSET_H *2);
	QRect tr = painter->fontMetrics().boundingRect(clipRect, Qt::TextWordWrap | Qt::AlignVCenter ,  msg_);
	tr.moveTo(0, 0);
	tr.setHeight(HEADER_HEIGHT);
	painter->drawText(tr, Qt::TextWordWrap | Qt::AlignBottom, msg_);
	painter->restore();
}

QVector<QvObjectDelegate::ButtonAction> QvObjectDelegate::getButtons( const QModelIndex &index ) const
{
	QVector<ButtonAction> tags_;


	if(needRestart(index) )
	{
		tags_ << baRestart << baCancel;
	} else if( index.flags() & Qt::ItemFlag(QvAbstractListItem::Downloading) ) {
		tags_ << baCancel;
	} else {
		
		bool installed = index.data(QvObjectModel::InstalledRole).toBool(),
			enabled = index.data(QvObjectModel::EnabledRole).toBool(),
			buildin = index.data(QvObjectModel::BuildInRole).toBool();

		if (installed && (index.flags() & Qt::ItemFlag(QvAbstractListItem::hasUpdate))) { tags_ << baUpdate;}
		if (installed && index.flags() & Qt::ItemFlag(QvAbstractListItem::canBeToggled)) { tags_ << (enabled  ? baDisable : baEnable);}
		if (installed && !buildin) { tags_  << baRemove;}
		if (!installed)	{ tags_ << baInstall; }
	}
	return tags_;
}

bool QvObjectDelegate::needRestart( const QModelIndex &index ) const
{
	return (index.flags() & Qt::ItemFlag(QvAbstractListItem::DownloadedNeedRestart))
		|| (index.flags() & Qt::ItemFlag(QvAbstractListItem::RemovedNeedRestart))
		|| (index.flags() & Qt::ItemFlag(QvAbstractListItem::UpdatedNeedRestart))
		|| (index.flags() & Qt::ItemFlag(QvAbstractListItem::DisabledNeedRestart))
		|| (index.flags() & Qt::ItemFlag(QvAbstractListItem::EnabledNeedRestart));
}

void QvObjectDelegate::drawItemBackground( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{

	painter->save();
	
	painter->setClipRect(option.rect);
	painter->setOpacity(0.2);

	QColor baseColor;
	if( 
		(index.flags() & Qt::ItemFlag(QvAbstractListItem::DownloadedNeedRestart))
		|| (index.flags() & Qt::ItemFlag(QvAbstractListItem::UpdatedNeedRestart))
		)
		baseColor = QColor(0x4C, 0xBB,0x17);

	if(
		(index.flags() & Qt::ItemFlag(QvAbstractListItem::EnabledNeedRestart))
		|| (index.flags() & Qt::ItemFlag(QvAbstractListItem::DisabledNeedRestart))
		|| (index.flags() & Qt::ItemFlag(QvAbstractListItem::RemovedNeedRestart))
		)
		baseColor = QColor(0xBa, 0xA4,0x16);


	baseColor.setAlpha(200);
	painter->setBrush(baseColor);
	painter->setPen(Qt::NoPen);
	painter->setRenderHint(QPainter::Antialiasing);

	painter->translate(option.rect.topLeft());
	painter->save();
	painter->translate(0, -STRIPE_WIDTH);	
	int count = option.rect.width() / (STRIPE_WIDTH *2)  + 2;
	
	for(int i =0; i < count; ++i)
	{
		painter->save();
		painter->rotate(30);
		painter->drawRect(0,0, STRIPE_WIDTH *.8, 1.5 * option.rect.height());
		painter->restore();
		painter->translate(STRIPE_WIDTH*2, 0);
	}
	painter->restore();

	QRect r = option.rect;
	r.moveTo(0,0);
	painter->setOpacity(1);
	QLinearGradient grad(QPointF(0, r.height()/5), QPointF(0, r.height()/5 * 3));
	QLinearGradient rgrad(grad);
	grad.setColorAt(0, QColor(0xFF, 0xFF, 0xFF, 60));	
	grad.setColorAt(1, QColor(Qt::white));
	
	painter->fillRect(r, QBrush(grad));
	painter->restore();
}
