#include <cmath>
#include <QDebug>
#include <QBrush>
#include <QDir>

#include "MyScene.h"
#include "Box.h"
#include "FontHalfBoldPixel.h"

const int refreshRate = 10;										 // in milliseconds
const qreal delta = (qreal)refreshRate / 1000; // in seconds

MyScene::MyScene(qreal *gravity, qreal *jumpHeight, QObject *parent)
		: QGraphicsScene(parent),
			_arrow("resources/arrow.png"),
			_gravity(gravity),
			_jumpHeight(jumpHeight)
{
	this->_stage = new Stage(this);

	// Set gravity & jump height
	*this->_gravity = *this->stage()->gravity();
	*this->_jumpHeight = *this->stage()->jumpHeight();

	// Set font
	QFont font = FontHalfBoldPixel();
	this->setFont(font);

	// Set timer
	this->_timer = new QTimer(this);
	connect(this->timer(), SIGNAL(timeout()), this, SLOT(update()));
	this->timer()->start(refreshRate);

	// Set player variables
	this->stage()->player()->setDelta(delta);
	this->stage()->player()->setGravity(this->gravity());
	this->stage()->player()->setJumpHeight(this->jumpHeight());

	// Show player view
	this->stage()->playerView()->show();

	// Set widget to get the font
	QWidget *widgetFont = new QWidget;
	widgetFont->setFont(this->font());

	// Pause Menu
	this->_pauseMenu = new PauseMenu(this->timer(), this);
	connect(this->pauseMenu(), &PauseMenu::optionSelected, this, &MyScene::restart);
	connect(this->pauseMenu(), &PauseMenu::update, this, &MyScene::update);

	// Start Menu
	this->_startMenu = new StartMenu(this->timer(), this);
	connect(this->startMenu(), &StartMenu::optionSelected, this, &MyScene::restart);
	connect(this->startMenu(), &StartMenu::update, this, &MyScene::update);

	// Retry Menu
	this->_retryMenu = new RetryMenu(this->timer(), this);
	connect(this->retryMenu(), &RetryMenu::optionSelected, this, &MyScene::restart);
	connect(this->retryMenu(), &RetryMenu::update, this, &MyScene::update);

	// Clear Objective Menu
	this->_clearObjectiveMenu = new ClearObjectiveMenu(this->timer(), this);
	connect(this->clearObjectiveMenu(), &ClearObjectiveMenu::optionSelected, this, &MyScene::restart);
	connect(this->clearObjectiveMenu(), &ClearObjectiveMenu::update, this, &MyScene::update);
	connect(this->timer(), SIGNAL(timeout()), this->clearObjectiveMenu(), SLOT(timeUpdate()));
	this->clearObjectiveMenu()->setScores(this->stage()->scores());

	// Save Score Menu
	this->_saveScoreMenu = new SaveScoreMenu(this->timer(), this);
	connect(this->saveScoreMenu(), &SaveScoreMenu::optionSelected, this, &MyScene::restart);
	connect(this->saveScoreMenu(), &SaveScoreMenu::update, this, &MyScene::update);
	this->saveScoreMenu()->setStage(this->stage());

	// Music
	QUrl baseUrl = QUrl::fromLocalFile(QDir::currentPath() + "/");

	QMediaPlaylist *playlist = new QMediaPlaylist();
	playlist->addMedia(baseUrl.resolved(QUrl("resources/music.mp3")));
	playlist->setPlaybackMode(QMediaPlaylist::Loop);

	this->_musicPlayer = new QMediaPlayer;
	musicPlayer()->setPlaylist(playlist);
	this->updateVolume();
	musicPlayer()->play();
}

MyScene::~MyScene()
{
	delete this->timer();
}

qreal MyScene::time() const
{
	qreal _time = this->clearObjectiveMenu()->time();
	return this->timeLimit() + (this->timeLimit() > 0 ? -_time : +_time);
}

void MyScene::drawForeground(QPainter *painter, const QRectF &rect)
{
	QLineF viewTop(rect.topLeft(), rect.topRight());
	QLineF viewLeft(rect.topLeft(), rect.bottomLeft());
	QLineF viewRight(rect.topRight(), rect.bottomRight());
	QLineF viewBottom(rect.bottomLeft(), rect.bottomRight());

	QPointF *intersectionPoint = new QPointF();

	if (this->playerObjectiveLine().intersects(viewTop, intersectionPoint) == QLineF::BoundedIntersection ||
			this->playerObjectiveLine().intersects(viewLeft, intersectionPoint) == QLineF::BoundedIntersection ||
			this->playerObjectiveLine().intersects(viewRight, intersectionPoint) == QLineF::BoundedIntersection ||
			this->playerObjectiveLine().intersects(viewBottom, intersectionPoint) == QLineF::BoundedIntersection)
	{
		QLineF fromSide(*intersectionPoint, this->playerObjectiveLine().p1());
		fromSide.setLength(this->arrow().width() / 2);

		painter->drawPixmap(
				fromSide.p2() + QPointF(-this->arrow().width() / 2, -this->arrow().height() / 2),
				this->arrow().transformed(QTransform().rotate(-fromSide.angle() - 90)));
	}

	// Get time in minutes and seconds as QString
	QString time = ClearObjectiveMenu::stringTime(this->time());

	painter->setFont(this->font());
	painter->setPen(QPen(QColor(ColorPalette::darkGray)));
	painter->drawText(rect.translated(5, 5), Qt::AlignLeft, time);
	painter->drawText(rect.translated(5, 4), Qt::AlignLeft, time);
	painter->drawText(rect.translated(5, 3), Qt::AlignLeft, time);
	painter->drawText(rect.translated(4, 5), Qt::AlignLeft, time);
	painter->drawText(rect.translated(4, 3), Qt::AlignLeft, time);
	painter->drawText(rect.translated(3, 5), Qt::AlignLeft, time);
	painter->drawText(rect.translated(3, 4), Qt::AlignLeft, time);
	painter->drawText(rect.translated(3, 3), Qt::AlignLeft, time);
	painter->setPen(QPen(QColor(ColorPalette::lightYellow)));
	painter->drawText(rect.translated(4, 4), Qt::AlignLeft, time);

	if (this->pauseMenu()->paused())
		this->pauseMenu()->show(painter, rect);

	if (this->startMenu()->paused())
		this->startMenu()->show(painter, rect);

	if (this->retryMenu()->paused())
		this->retryMenu()->show(painter, rect);

	if (this->clearObjectiveMenu()->paused())
		this->clearObjectiveMenu()->show(painter, rect);

	if (this->saveScoreMenu()->paused())
		this->saveScoreMenu()->show(painter, rect);
}

QLineF MyScene::playerObjectiveLine() const
{
	QPointF objectiveCenter = (this->objective()->rect().topLeft() + this->objective()->rect().bottomRight()) / 2;
	QPointF playerCenter = (this->player()->rect().topLeft() + this->player()->rect().bottomRight()) / 2 +
												 this->player()->pos();

	return QLineF(playerCenter, objectiveCenter);
}

void MyScene::update()
{
	if (!this->retryMenu()->paused() && this->time() < 0)
	{
		this->retryMenu()->pause();
	}

	if (!this->pauseMenu()->paused() &&
			!this->startMenu()->paused() &&
			!this->retryMenu()->paused() &&
			!this->clearObjectiveMenu()->paused() &&
			!this->saveScoreMenu()->paused())
	{
		if (this->objective()->collidesWithItem(this->player()->getCollisionBox()))
		{
			this->clearObjectiveMenu()->pause();
		}

		this->player()->update();
		this->playerView()->centerOn(this->player());

		this->leftWall()->setPos(this->leftWall()->pos().x(), this->player()->pos().y());
		this->rightWall()->setPos(this->rightWall()->pos().x(), this->player()->pos().y());

		if (this->player()->bottom() > this->stage()->height() * 16 + this->player()->rect().height() * 2)
		{
			this->retryMenu()->pause();
		}

		this->updateVolume();
	}

	QGraphicsScene::update();
}

void MyScene::keyPressEvent(QKeyEvent *event)
{
	// Handle start menu
	if (this->startMenu()->paused())
	{
		this->startMenu()->keyPressEvent(event);
		return;
	}

	// Handle retry menu
	if (this->retryMenu()->paused())
	{
		this->retryMenu()->keyPressEvent(event);
		return;
	}

	// Handle clear objective menu
	if (this->clearObjectiveMenu()->paused())
	{
		this->clearObjectiveMenu()->keyPressEvent(event);
		return;
	}

	// Handle save name menu
	if (this->saveScoreMenu()->paused())
	{
		this->saveScoreMenu()->keyPressEvent(event);
		return;
	}

	// Handle pause menu
	this->pauseMenu()->keyPressEvent(event);

	// Handle game gestures
	if (!this->pauseMenu()->paused())
	{
		switch (event->key())
		{
		case Qt::Key_Left:
		case Qt::Key_A:
			this->player()->setImmobileRightState(false);
			this->player()->setGoingLeftState(true);
			break;
		case Qt::Key_Right:
		case Qt::Key_D:
			this->player()->setImmobileLeftState(false);
			this->player()->setGoingRightState(true);
			break;
		case Qt::Key_Up:
		case Qt::Key_W:
		case Qt::Key_Space:
			if (!this->player()->getAirborneState())
			{
				this->player()->jump();
			}
			break;
		case Qt::Key_Control:
			this->player()->setSprintingState(true);
			break;
		default:
			break;
		}
	}
}

void MyScene::keyReleaseEvent(QKeyEvent *event)
{
	// Handle game gestures
	switch (event->key())
	{
	case Qt::Key_Left:
	case Qt::Key_A:
		this->player()->setGoingLeftState(false);
		break;
	case Qt::Key_Right:
	case Qt::Key_D:
		this->player()->setGoingRightState(false);
		break;
	case Qt::Key_Control:
		this->player()->setSprintingState(false);
		break;
	default:
		break;
	}
}

void MyScene::updateVolume()
{
	int k = 25;
	qreal playerObjectiveDistance = this->playerObjectiveLine().length();
	musicPlayer()->setVolume(k * 100 / (playerObjectiveDistance + k) + 5);
}

void MyScene::restart(QString reason)
{
	if (reason == PauseMenuOptions::Restart ||
			reason == RetryMenuOptions::Retry)
	{
		this->clearObjectiveMenu()->setTime(0);
		this->player()->setPos(this->stage()->spawnPoint());
		this->update();
	}
	else if (reason == ClearObjectiveMenuOptions::No ||
					 reason == SaveScoreMenuOptions::Register)
	{
		this->retryMenu()->pause();
		this->update();
	}
	else if (reason == ClearObjectiveMenuOptions::Yes)
	{
		this->saveScoreMenu()->pause();
		this->saveScoreMenu()->setTimeScore(this->clearObjectiveMenu()->time());
		this->update();
	}
}