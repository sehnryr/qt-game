#ifndef CPP_QT_BOX_H
#define CPP_QT_BOX_H

#include <QGraphicsRectItem>
#include <QPixmap>

class Box : public QGraphicsRectItem
{
public:
	Box(qreal x,
			qreal y,
			qreal width = 16,
			qreal height = 16,
			QString image = "resources/bloc_dark.png",
			QGraphicsItem *parent = nullptr);
	virtual ~Box() {}
};

#endif // CPP_QT_BOX_H