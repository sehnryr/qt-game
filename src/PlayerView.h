#ifndef CPP_QT_PLAYER_VIEW_H
#define CPP_QT_PLAYER_VIEW_H

#include <QGraphicsView>
#include <QWheelEvent>

class PlayerView : public QGraphicsView
{
	Q_OBJECT
public:
	explicit PlayerView(QWidget *parent = nullptr) : QGraphicsView(parent)
	{
		QPixmap backgroundSprite("resources/background.png");
		QBrush brush(backgroundSprite);

		this->setBackgroundBrush(brush);
		this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	}

protected:
	virtual void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE
	{
		event->accept();
	}
};

#endif // CPP_QT_PLAYER_VIEW_H