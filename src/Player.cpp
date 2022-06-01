#include <cmath>

#include "Player.h"

Player::Player(
		qreal x,
		qreal y,
		qreal w,
		qreal h,
		QGraphicsItem *parent)
		: QGraphicsRectItem(x, y, w, h, parent),
			jumpFactor(2),
			airResistanceFactor(1),
			walkSpeed(5 * 16),
			sprintFactor(2),
			walkingDistance(0),
			isAirborne(false),
			isGoingLeft(false),
			isGoingRight(false),
			isSprinting(false),
			isImmobileLeft(false),
			isImmobileRight(false),
			isLookingRight(true),
			velocity(0, 0)
{
	collisionBox = new CollisionBox(QRectF(0, 0, w, h), this);
	// collisionBox->show();

	spriteStill = QPixmap("resources/player_still.png");
	spriteMove1 = QPixmap("resources/player_move1.png");
	spriteMove2 = QPixmap("resources/player_move2.png");
}

Player::~Player()
{
}

void Player::paint(QPainter *painter,
									 const QStyleOptionGraphicsItem *option,
									 QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	QPixmap sprite = this->spriteStill;

	if (!this->getStillState() &&
			!this->getAirborneState() &&
			this->getWalkingDistance() > 0)
	{
		qreal walkingTime = fmod(this->getWalkingDistance(), 32);
		if (0 <= walkingTime && walkingTime < 8)
			sprite = spriteMove1;
		else if (16 <= walkingTime && walkingTime < 24)
			sprite = spriteMove2;
	}

	if (!this->getLookingRightState())
	{
		sprite = sprite.transformed(QTransform().scale(-1, 1));
	}

	painter->drawPixmap(
			this->rect().topLeft(),
			sprite);
}

void Player::moveBy(QPointF dp)
{
	QGraphicsRectItem::moveBy(dp.x(), dp.y());
}

void Player::setPos(QPointF pos)
{
	QGraphicsRectItem::setPos(pos);
}

void Player::moveBy(qreal dx, qreal dy)
{
	QPointF position = this->pos();
	this->setPos(position.x() + dx, position.y() + dy);
}

void Player::setPos(qreal ax, qreal ay)
{
	QPointF v = this->getVelocity();
	QPointF p = this->pos();

	QPointF dv = this->getDeltaVelocity();
	QPointF dp = this->getDeltaPosition();

	if (p.x() - ax != 0)
	{
		collisionBox->setOffsetX(dp.x());
		this->setVelocityX(v.x() + dv.x());
	}

	if (p.y() - ay != 0)
	{
		collisionBox->setOffsetY(dp.y());
		this->setVelocityY(v.y() + dv.y());
	}

	QGraphicsRectItem::setPos(ax, ay);
}

bool Player::getLookingRightState()
{
	bool isGoingLeft = this->getGoingLeftState();
	bool isGoingRight = this->getGoingRightState();

	qint8 lookingDirection = (isGoingRight ? 1 : 0) + (isGoingLeft ? -1 : 0);

	if (lookingDirection == -1)
		this->setLookingRightState(false);
	if (lookingDirection == 1)
		this->setLookingRightState(true);

	return this->isLookingRight;
}

bool Player::getStillState() const
{
	QPointF dp = this->getDeltaPosition();

	bool isGoingLeft = this->getGoingLeftState();
	bool isGoingRight = this->getGoingRightState();
	bool isStillVertically = dp.y() == 0;

	return isImmobileLeft && isStillVertically && isGoingLeft == isGoingRight;
}

QPointF Player::getDeltaVelocity() const
{
	// X
	qreal dvx = 0;

	// Y
	qreal vy = this->getVelocity().y();
	// https://www.desmos.com/calculator/mblivvxn02
	qreal dvy = pow(2, 2 * this->getJumpFactor() - 2) * this->getGravity() * this->delta;
	dvy = (vy + dvy < 0) ? dvy : this->getGravity() * this->delta;

	return QPointF(dvx, dvy);
}

QPointF Player::getDeltaPosition() const
{
	QPointF v = this->getVelocity();
	QPointF dv = this->getDeltaVelocity();

	// X
	qreal leftRatio = this->isGoingLeft && !this->isImmobileLeft ? 1 : 0;
	qreal rightRatio = this->isGoingRight && !this->isImmobileRight ? 1 : 0;

	if (this->getAirborneState())
	{
		leftRatio *= this->airResistanceFactor;
		rightRatio *= this->airResistanceFactor;
	}

	if (this->getSprintingState())
	{
		leftRatio *= this->sprintFactor;
		rightRatio *= this->sprintFactor;
	}

	qreal dx = (rightRatio - leftRatio) *
						 this->walkSpeed *
						 this->delta;

	// Y
	qreal vy = v.y() + dv.y();
	qreal dy = vy * this->delta;

	return QPointF(dx, dy);
}

void Player::jump()
{
	QGraphicsRectItem::moveBy(0, -1);
	this->setVelocityY(-pow(2, this->getJumpFactor()) *
										 sqrt(this->getGravity() * this->getJumpHeight() / 2));
}

void Player::update()
{
	CollisionBox *collisionBox = this->getCollisionBox();

	// Airborne time handle
	QGraphicsRectItem *floor = collisionBox->getBottomCollidingItem();
	qreal thisBottom = this->pos().y() + this->rect().height();
	if (floor &&
			thisBottom <= floor->rect().top() &&
			this->getVelocity().y() >= 0)
	{
		this->setVelocityY(0);

		QPointF pos = this->pos();
		this->setPos(pos.x(), floor->rect().top() - this->rect().height());

		this->setAirborneState(false);
		this->getCollisionBox()->setOffsetY(0);
	}
	else
	{
		this->setAirborneState(true);
	}

	// Down force gravity
	if (this->getAirborneState())
	{
		qreal dy = this->getDeltaPosition().y();
		this->moveBy(0, dy);
	}

	QGraphicsRectItem *ceiling = collisionBox->getTopCollidingItem();
	if (ceiling != nullptr &&
			this->getAirborneState() &&
			this->getVelocity().y() <= 0)
	{
		this->setVelocityY(0);
	}

	QGraphicsRectItem *rightWall = collisionBox->getRightCollidingItem();
	if (rightWall != nullptr &&
			this->getImmobileRightState() &&
			this->getGoingRightState())
	{
		QPointF pos = this->pos();
		this->setPos(rightWall->rect().left() - this->rect().width(), pos.y());
	}
	this->setImmobileRightState(rightWall != nullptr);

	QGraphicsRectItem *leftWall = collisionBox->getLeftCollidingItem();
	if (leftWall != nullptr &&
			this->getImmobileLeftState() &&
			this->getGoingLeftState())
	{
		QPointF pos = this->pos();
		this->setPos(leftWall->rect().right(), pos.y());
	}
	this->setImmobileLeftState(leftWall != nullptr);

	qreal dx = this->getDeltaPosition().x();
	this->moveBy(dx, 0);

	// Walk time handling
	if (!this->getAirborneState() && dx != 0)
	{
		this->setWalkingDistance(this->getWalkingDistance() + abs(dx));
	}
	else
	{
		this->setWalkingDistance(0);
	}

	QGraphicsRectItem::update();
}