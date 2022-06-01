#ifndef CPP_QT_PAUSE_MENU_H
#define CPP_QT_PAUSE_MENU_H

#include <QTimer>
#include <QWidget>

#include "BaseMenu.h"

namespace PauseMenuOptions
{
	const QString Restart = "restart";
	const QString Resume = "resume";
	const QString Quit = "quit";
};

class PauseMenu : public BaseMenu
{
	Q_OBJECT
private:
	QTimer *_timer;
	bool _paused;

	int _interval; // in milliseconds

protected:
	int interval() const { return _interval; }
	QTimer *timer() const { return _timer; }

	void setPauseStatus(bool const status);

public:
	PauseMenu(QTimer *timer, QGraphicsScene *scene);

	bool paused() const { return _paused; }

	void pause() { setPauseStatus(true); }
	void resume() { setPauseStatus(false); }

	void keyPressEvent(QKeyEvent *event);
};

#endif // CPP_QT_PAUSE_MENU_H