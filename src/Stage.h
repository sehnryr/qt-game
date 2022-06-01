#ifndef CPP_QT_STAGE_H
#define CPP_QT_STAGE_H

#include <QGraphicsScene>

#include "Box.h"
#include "Player.h"
#include "PlayerView.h"

enum StageObjects
{
	Obj_Air = ' ',
	Obj_Bloc = 'X',
	Obj_Player = 'P',
	Obj_Objective = 'O'
};

class Stage
{
private:
	QGraphicsScene *_scene;
	int _width;
	int _height;
	int _timeLimit;
	qreal *_gravity;
	qreal *_jumpHeight;
	Box *_objective = nullptr;
	Player *_player = nullptr;
	QString _fileName;
	QPointF _spawnPoint;
	PlayerView *_playerView;
	QGraphicsRectItem *_leftWall;
	QGraphicsRectItem *_rightWall;
	QList<QPair<QString, qreal>> *_scores;
	QVector<QVector<StageObjects>> _stage;

protected:
	QGraphicsScene *scene() const { return _scene; }
	QString fileName() const { return _fileName; }

	void setWidth(int const w) { _width = w; }
	void setHeight(int const h) { _height = h; }
	void setStage(QVector<QVector<StageObjects>> const s) { _stage = s; }

	void readFile();
	void writeFile();

	void generate();

public:
	Stage(QGraphicsScene *scene);
	~Stage() {}

	int width() const { return _width; }
	int height() const { return _height; }
	int timeLimit() const { return _timeLimit; }
	qreal *gravity() const { return _gravity; }
	qreal *jumpHeight() const { return _jumpHeight; }
	Box *objective() const { return _objective; }
	Player *player() const { return _player; }
	QPointF spawnPoint() const { return _spawnPoint; }
	PlayerView *playerView() const { return _playerView; }
	QGraphicsRectItem *leftWall() const { return _leftWall; };
	QGraphicsRectItem *rightWall() const { return _rightWall; };
	QList<QPair<QString, qreal>> *scores() const { return _scores; }
	QVector<QVector<StageObjects>> stage() const { return _stage; }

	void addScore(QPair<QString, qreal> const pair);
};

#endif // CPP_QT_STAGE_H