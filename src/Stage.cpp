#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QStringList>

#include "Stage.h"

Stage::Stage(QGraphicsScene *scene) : _scene(scene), _fileName("stage.txt")
{
	this->_gravity = new qreal;
	this->_jumpHeight = new qreal;

	this->_scores = new QList<QPair<QString, qreal>>;

	this->readFile();
	this->setHeight(this->stage().length());
	this->setWidth(this->stage()[0].length());

	this->generate();
}

void Stage::readFile()
{
	QFile file(this->fileName());
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug() << file.errorString();
		QCoreApplication::exit(1);
	}

	QVector<QVector<StageObjects>> stage;
	int lineCount = 0;
	while (!file.atEnd())
	{
		lineCount++;

		QByteArray line = file.readLine().replace("\n", "");
		QString lineAsString(line);

		if (lineCount - 1 == 0)
		{
			QStringList settings = lineAsString.split(",");
			this->_timeLimit = settings[0].toInt();
			*this->_gravity = (int)(settings[1].toDouble() * 10) / (qreal)10;
			*this->_jumpHeight = (int)(settings[2].toDouble() * 10) / (qreal)10;
			continue;
		}

		if (lineAsString.startsWith("#"))
		{
			lineAsString = lineAsString.replace("#", "");
			QStringList row = lineAsString.split(",");
			QString playerName = row[0];
			qreal timeScore = row[1].toDouble();
			this->scores()->append(QPair(playerName, timeScore));
			continue;
		}

		if (stage.length() > 0 && stage[0].length() != lineAsString.length())
		{
			qDebug() << "Lines in stage maker file are not the same.";
			QCoreApplication::exit(1);
		}

		QVector<StageObjects> row;
		for (int j = 0; j < lineAsString.length(); j++)
		{
			QChar chr = lineAsString.at(j);
			if (chr == QChar(StageObjects::Obj_Bloc))
				row.append(StageObjects::Obj_Bloc);
			else if (chr == QChar(StageObjects::Obj_Player))
				row.append(StageObjects::Obj_Player);
			else if (chr == QChar(StageObjects::Obj_Objective))
				row.append(StageObjects::Obj_Objective);
			else
				row.append(StageObjects::Obj_Air);
		}
		if (!row.isEmpty())
			stage.append(row);
	}

	if (stage.length() == 0)
	{
		qDebug() << "Stage maker file is empty.";
		QCoreApplication::exit(1);
	}

	this->setStage(stage);
}

void Stage::writeFile()
{
	QFile file(this->fileName());
	if (!file.open(QIODevice::Append | QIODevice::Text))
	{
		qDebug() << file.errorString();
		QCoreApplication::exit(1);
	}

	file.resize(0);
	QTextStream out(&file);
	out << this->timeLimit() << ","
			<< *this->gravity() << ","
			<< *this->jumpHeight() << Qt::endl;

	QPair<QString, qreal> score;
	foreach (score, *this->scores())
		out << "#" << score.first << "," << score.second << Qt::endl;

	QVector<StageObjects> row;
	foreach (row, this->stage())
	{
		foreach (StageObjects object, row)
			switch (object)
			{
			case StageObjects::Obj_Bloc:
				out << "X";
				break;
			case StageObjects::Obj_Player:
				out << "P";
				break;
			case StageObjects::Obj_Objective:
				out << "O";
				break;
			case StageObjects::Obj_Air:
			default:
				out << " ";
				break;
			}
		out << Qt::endl;
	}
}

void Stage::generate()
{
	QPointF stageOffset(0, 0);

	for (int i = 0; i < this->height(); i++)
		for (int j = 0; j < this->width(); j++)
			switch (this->stage()[i][j])
			{
			case StageObjects::Obj_Bloc:
				this->scene()->addItem(new Box(
						(j + stageOffset.x()) * 16,
						(i + stageOffset.y()) * 16));
				break;
			case StageObjects::Obj_Player:
				if (this->player() != nullptr)
				{
					qDebug() << "There is more than one player in the stage.";
					QCoreApplication::exit(1);
				}
				this->_player = new Player();
				this->_spawnPoint.setX((j + stageOffset.x()) * 16);
				this->_spawnPoint.setY((i + stageOffset.y()) * 16 + 16 - Player().rect().height());
				this->player()->moveBy(this->spawnPoint());
				this->scene()->addItem(this->player());
				break;
			case StageObjects::Obj_Objective:
				if (this->objective() != nullptr)
				{
					qDebug() << "There is more than one objective in the stage.";
					QCoreApplication::exit(1);
				}
				this->_objective = new Box(
						(j + stageOffset.x()) * 16,
						(i + stageOffset.y() - 1) * 16,
						16, 2 * 16, "resources/objective.png");
				this->scene()->addItem(this->objective());
				break;
			case StageObjects::Obj_Air:
			default:
				break;
			}

	if (this->player() == nullptr)
	{
		qDebug() << "There is no player in the stage.";
		QCoreApplication::exit(1);
	}

	if (this->objective() == nullptr)
	{
		qDebug() << "There is no objective in the stage.";
		QCoreApplication::exit(1);
	}

	// Player scene walls
	this->_leftWall = new QGraphicsRectItem(this->width() * 16 + 2, 0, 0, 16);
	this->_rightWall = new QGraphicsRectItem(-2, 0, 0, 16);
	this->leftWall()->setPen(Qt::NoPen);
	this->rightWall()->setPen(Qt::NoPen);
	this->scene()->addItem(this->leftWall());
	this->scene()->addItem(this->rightWall());

	// Player view
	this->_playerView = new PlayerView();
	this->playerView()->resize(480, 320);
	this->playerView()->setScene(this->scene());
	this->playerView()->centerOn(this->player());
	this->playerView()->scale(2, 2);

	// Set scene rectangle
	this->scene()->setSceneRect(
			0, 0,
			(this->width() + stageOffset.x()) * 16,
			(this->height() + stageOffset.y()) * 16);
}

void Stage::addScore(QPair<QString, qreal> const pair)
{
	this->scores()->append(pair);
	this->writeFile();
}
