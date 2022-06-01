#ifndef CPP_QT_PLAYER_H
#define CPP_QT_PLAYER_H

#include <QGraphicsRectItem>
#include <QPixmap>

#include "CollisionBox.h"

class Player : public QObject, public QGraphicsRectItem
{
	Q_OBJECT
private:
	qreal delta;
	qreal jumpFactor;
	qreal airResistanceFactor;
	qreal walkSpeed;
	qreal sprintFactor;
	qreal walkingDistance;
	qreal *_gravity = nullptr;
	qreal *_jumpHeight = nullptr;

	bool isAirborne;
	bool isGoingLeft;
	bool isGoingRight;
	bool isSprinting;
	bool isImmobileLeft;
	bool isImmobileRight;
	bool isLookingRight;

	QPointF velocity;

	CollisionBox *collisionBox;

	QPixmap spriteStill;
	QPixmap spriteMove1;
	QPixmap spriteMove2;

public:
	Player(
			qreal x = 0,
			qreal y = 0,
			qreal w = 16,
			qreal h = 21,
			QGraphicsItem *parent = nullptr);
	virtual ~Player();

	void paint(QPainter *painter,
						 const QStyleOptionGraphicsItem *option,
						 QWidget *widget);

	void moveBy(QPointF dp);
	void setPos(QPointF pos);
	void moveBy(qreal dx, qreal dy);
	void setPos(qreal ax, qreal ay);

	qreal getDelta() const { return delta; }
	void setDelta(qreal const value) { delta = value; }

	qreal getGravity() const { return *_gravity * 16; }
	void setGravity(qreal *const value) { _gravity = value; }

	qreal getJumpHeight() const { return (*_jumpHeight + 0.1) * 16; }
	void setJumpHeight(qreal *const value) { _jumpHeight = value; }

	qreal getJumpFactor() const { return jumpFactor; }
	void setJumpFactor(qreal const value) { jumpFactor = value; }

	qreal getAirResistanceFactor() const { return airResistanceFactor; }
	void setAirResistanceFactor(qreal const value) { airResistanceFactor = value; }

	qreal getWalkSpeed() const { return walkSpeed; }
	void setWalkSpeed(qreal const value) { walkSpeed = value; }

	qreal getWalkingDistance() const { return walkingDistance; }
	void setWalkingDistance(qreal const value) { walkingDistance = value; }

	bool getAirborneState() const { return isAirborne; }
	void setAirborneState(bool const state) { isAirborne = state; }

	bool getGoingLeftState() const { return isGoingLeft; }
	void setGoingLeftState(bool const status) { isGoingLeft = status; }

	bool getGoingRightState() const { return isGoingRight; }
	void setGoingRightState(bool const status) { isGoingRight = status; }

	bool getSprintingState() const { return isSprinting; }
	void setSprintingState(bool const state) { isSprinting = state; }

	bool getImmobileLeftState() const { return isImmobileLeft; }
	void setImmobileLeftState(bool const state) { isImmobileLeft = state; }

	bool getImmobileRightState() const { return isImmobileRight; }
	void setImmobileRightState(bool const state) { isImmobileRight = state; }

	bool getLookingRightState();
	void setLookingRightState(bool const state) { isLookingRight = state; }

	bool getStillState() const;

	QPointF getVelocity() const { return velocity; }
	void setVelocityX(qreal x) { velocity.setX(x); }
	void setVelocityY(qreal y) { velocity.setY(y); }
	void setVelocity(QPointF point) { this->setVelocity(point.x(), point.y()); }
	void setVelocity(qreal x, qreal y)
	{
		this->setVelocityX(x);
		this->setVelocityY(y);
	}

	QPointF getDeltaVelocity() const;
	QPointF getDeltaPosition() const;

	CollisionBox *getCollisionBox() const { return collisionBox; }

	qreal top() const { return this->pos().y(); }
	qreal bottom() const { return this->pos().y() + this->rect().height(); }
	qreal left() const { return this->pos().x(); }
	qreal right() const { return this->pos().x() + this->rect().width(); }

	void jump();

public slots:
	void update();
};

#endif // CPP_QT_PLAYER_H