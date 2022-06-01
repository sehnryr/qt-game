#ifndef CPP_QT_SAVE_SCORE_MENU_H
#define CPP_QT_SAVE_SCORE_MENU_H

#include "PauseMenu.h"
#include "Stage.h"

namespace SaveScoreMenuOptions
{
	const QString Register = "register";
}

class SaveScoreMenu : public PauseMenu
{
	Q_OBJECT
private:
	Stage *_stage;
	QString _username;
	qreal _timeScore;

protected:
	QString username() const { return _username; }
	qreal timeScore() const { return _timeScore; }

	void setUsername(QString const username) { _username = username; }

public:
	SaveScoreMenu(QTimer *timer, QGraphicsScene *scene);
	void show(QPainter *painter, const QRectF &rect);
	void keyPressEvent(QKeyEvent *event);

	void setTimeScore(qreal const score) { _timeScore = score; }

	Stage *stage() const { return _stage; }
	void setStage(Stage *stage) { _stage = stage; }
};

#endif // CPP_QT_SAVE_SCORE_MENU_H