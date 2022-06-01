#include <QPen>
#include <QBrush>

#include "Box.h"

Box::Box(
		qreal x,
		qreal y,
		qreal width,
		qreal height,
		QString image,
		QGraphicsItem *parent)
		: QGraphicsRectItem(x, y, width, height, parent)
{
	QPixmap boxSprite(image);
	QBrush brush(boxSprite);
	brush.setTransform(QTransform().translate(x,y));
	this->setBrush(brush);
	this->setPen(Qt::NoPen);
}