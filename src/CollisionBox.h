#ifndef CPP_QT_COLLISION_BOX_H
#define CPP_QT_COLLISION_BOX_H

#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QPointF>
#include <QList>

class CollisionBox : public QGraphicsRectItem
{
private:
	QPointF offset;

protected:
	void update();

public:
	CollisionBox(const QRectF &rect, QGraphicsItem *parent = nullptr);

	void paint(
			QPainter *painter,
			const QStyleOptionGraphicsItem *option,
			QWidget *widget);

	QList<QGraphicsItem *> collidingItems(
			Qt::ItemSelectionMode mode = Qt::IntersectsItemShape) const;

	QPointF getOffset() const { return offset; }
	void setOffset(const QPointF value);
	void setOffsetX(const qreal x);
	void setOffsetY(const qreal y);

	QGraphicsRectItem *getTopCollidingItem() const;
	QGraphicsRectItem *getBottomCollidingItem() const;
	QGraphicsRectItem *getLeftCollidingItem() const;
	QGraphicsRectItem *getRightCollidingItem() const;
};

#endif // CPP_QT_COLLISION_BOX_H