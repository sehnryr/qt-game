#include "CollisionBox.h"

CollisionBox::CollisionBox(const QRectF &rect, QGraphicsItem *parent)
		: QGraphicsRectItem(rect, parent),
			offset(0, 0)
{
	this->hide();
}

void CollisionBox::paint(
		QPainter *painter,
		const QStyleOptionGraphicsItem *option,
		QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	if (this->isVisible())
	{
		QPen pen(QColor("red"));
		painter->setPen(pen);
		painter->drawRect(this->rect());
	}
}

void CollisionBox::update()
{
	QPointF offset = this->getOffset();
	QGraphicsRectItem *parent = dynamic_cast<QGraphicsRectItem *>(this->parentItem());

	QRectF rect = parent->rect();

	QPointF topLeft = rect.topLeft() +
										QPointF(0, offset.y() < 0 ? offset.y() : 0) +
										QPointF(offset.x() < 0 ? offset.x() : 0, 0);
	QPointF bottomRight = rect.bottomRight() +
												QPointF(0, offset.y() > 0 ? offset.y() : 0) +
												QPointF(offset.x() > 0 ? offset.x() : 0, 0);

	this->setRect(QRectF(topLeft, bottomRight));

	QGraphicsRectItem::update();
}

/*
 * Override of QGraphicsRectItem::collidingItems which excludes the parent
 * from the colliding items.
 */
QList<QGraphicsItem *> CollisionBox::collidingItems(
		Qt::ItemSelectionMode mode) const
{
	QList<QGraphicsItem *> collidingItems = QGraphicsRectItem::collidingItems(mode);

	foreach (QGraphicsItem *item, collidingItems)
	{
		if (item == this->parentItem())
		{
			collidingItems.removeOne(item);
		}
	}

	return collidingItems;
}

void CollisionBox::setOffset(const QPointF value)
{
	if (value != this->offset)
	{
		this->offset = value;
		this->update();
	}
}

void CollisionBox::setOffsetX(const qreal x)
{
	if (x != this->offset.x())
	{
		this->offset.setX(x);
		this->update();
	}
}

void CollisionBox::setOffsetY(const qreal y)
{
	if (y != this->offset.y())
	{
		this->offset.setY(y);
		this->update();
	}
}

QGraphicsRectItem *CollisionBox::getTopCollidingItem() const
{
	QGraphicsRectItem *parent = dynamic_cast<QGraphicsRectItem *>(this->parentItem());
	QList<QGraphicsItem *> collidingItems = this->collidingItems();
	QGraphicsItem *topItem = nullptr;

	foreach (QGraphicsItem *item, collidingItems)
	{
		QPointF itemPos = item->pos();
		QGraphicsRectItem *rectItem = dynamic_cast<QGraphicsRectItem *>(item);
		QLineF bottomLine(rectItem->rect().bottomLeft() + itemPos, rectItem->rect().bottomRight() + itemPos);
		QGraphicsLineItem *lineItem = new QGraphicsLineItem(bottomLine);

		qreal playerTop = parent->pos().y();
		qreal playerRight = parent->pos().x() + parent->rect().width();
		qreal playerLeft = parent->pos().x();

		if (this->collidesWithItem(lineItem) &&
				playerTop >= rectItem->rect().bottom() &&
				playerRight > rectItem->rect().left() &&
				playerLeft < rectItem->rect().right())
		{
			if (topItem == nullptr)
			{
				topItem = item;
				continue;
			}

			if (item->boundingRect().bottom() > topItem->boundingRect().bottom())
			{
				topItem = item;
			}
		}
	}

	return dynamic_cast<QGraphicsRectItem *>(topItem);
}

QGraphicsRectItem *CollisionBox::getBottomCollidingItem() const
{
	QGraphicsRectItem *parent = dynamic_cast<QGraphicsRectItem *>(this->parentItem());
	QList<QGraphicsItem *> collidingItems = this->collidingItems();
	QGraphicsItem *topItem = nullptr;

	foreach (QGraphicsItem *item, collidingItems)
	{
		QPointF itemPos = item->pos();
		QGraphicsRectItem *rectItem = dynamic_cast<QGraphicsRectItem *>(item);
		QLineF topLine(rectItem->rect().topLeft() + itemPos, rectItem->rect().topRight() + itemPos);
		QGraphicsLineItem *lineItem = new QGraphicsLineItem(topLine);

		qreal playerBottom = parent->pos().y() + parent->rect().height();
		qreal playerRight = parent->pos().x() + parent->rect().width();
		qreal playerLeft = parent->pos().x();

		if (this->collidesWithItem(lineItem) &&
				playerBottom <= rectItem->rect().top() &&
				playerRight > rectItem->rect().left() &&
				playerLeft < rectItem->rect().right())
		{
			if (topItem == nullptr)
			{
				topItem = item;
				continue;
			}

			if (item->boundingRect().top() < topItem->boundingRect().top())
			{
				topItem = item;
			}
		}
	}

	return dynamic_cast<QGraphicsRectItem *>(topItem);
}

QGraphicsRectItem *CollisionBox::getLeftCollidingItem() const
{
	QGraphicsRectItem *parent = dynamic_cast<QGraphicsRectItem *>(this->parentItem());
	QList<QGraphicsItem *> collidingItems = this->collidingItems();
	QGraphicsItem *topItem = nullptr;

	foreach (QGraphicsItem *item, collidingItems)
	{
		QPointF itemPos = item->pos();
		QGraphicsRectItem *rectItem = dynamic_cast<QGraphicsRectItem *>(item);
		QLineF topLine(rectItem->rect().topRight() + itemPos, rectItem->rect().bottomRight() + itemPos);
		QGraphicsLineItem *lineItem = new QGraphicsLineItem(topLine);

		qreal playerBottom = parent->pos().y() + parent->rect().height();
		qreal playerLeft = parent->pos().x();

		if (this->collidesWithItem(lineItem) &&
				playerLeft >= rectItem->rect().right() &&
				playerBottom > rectItem->rect().top())
		{
			if (topItem == nullptr)
			{
				topItem = item;
				continue;
			}

			if (item->boundingRect().right() > topItem->boundingRect().right())
			{
				topItem = item;
			}
		}
	}

	return dynamic_cast<QGraphicsRectItem *>(topItem);
}

QGraphicsRectItem *CollisionBox::getRightCollidingItem() const
{
	QGraphicsRectItem *parent = dynamic_cast<QGraphicsRectItem *>(this->parentItem());
	QList<QGraphicsItem *> collidingItems = this->collidingItems();
	QGraphicsItem *topItem = nullptr;

	foreach (QGraphicsItem *item, collidingItems)
	{
		QPointF itemPos = item->pos();
		QGraphicsRectItem *rectItem = dynamic_cast<QGraphicsRectItem *>(item);
		QLineF topLine(rectItem->rect().topLeft() + itemPos, rectItem->rect().bottomLeft() + itemPos);
		QGraphicsLineItem *lineItem = new QGraphicsLineItem(topLine);

		qreal playerBottom = parent->pos().y() + parent->rect().height();
		qreal playerRight = parent->pos().x() + parent->rect().width();

		if (this->collidesWithItem(lineItem) &&
				playerRight <= rectItem->rect().left() &&
				playerBottom > rectItem->rect().top())
		{
			if (topItem == nullptr)
			{
				topItem = item;
				continue;
			}

			if (item->boundingRect().left() < topItem->boundingRect().left())
			{
				topItem = item;
			}
		}
	}

	return dynamic_cast<QGraphicsRectItem *>(topItem);
}