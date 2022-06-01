#ifndef CPP_QT_CLEAR_OBJECTIVE_MENU_H
#define CPP_QT_CLEAR_OBJECTIVE_MENU_H

#include "PauseMenu.h"
#include "QPairComparer.h"

namespace ClearObjectiveMenuOptions
{
	const QString Yes = "yes";
	const QString No = "no";
};

class ClearObjectiveMenu : public PauseMenu
{
	Q_OBJECT
private:
	qreal _time;
	qreal _delta;
	QList<QPair<QString, qreal>> *_scores = Q_NULLPTR;

protected:
	qreal delta() const { return _delta; }

public:
	ClearObjectiveMenu(QTimer *timer, QGraphicsScene *scene);
	void show(QPainter *painter, const QRectF &rect);
	void keyPressEvent(QKeyEvent *event);

	qreal time() const { return _time; }
	void setTime(qreal const time) { _time = time; }

	QList<QPair<QString, qreal>> *scores() const { return _scores; }
	void setScores(QList<QPair<QString, qreal>> *scores) { _scores = scores; }

	static QString stringTime(qreal const time);

public slots:
	void timeUpdate();
};

#endif // CPP_QT_CLEAR_OBJECTIVE_MENU_H