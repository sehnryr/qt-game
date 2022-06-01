#ifndef CPP_QT_RETRY_MENU_H
#define CPP_QT_RETRY_MENU_H

#include "PauseMenu.h"

namespace RetryMenuOptions
{
	const QString Retry = "retry";
	const QString Quit = "quit";
};

class RetryMenu : public PauseMenu
{
	Q_OBJECT
public:
	RetryMenu(QTimer *timer, QGraphicsScene *scene);
	void keyPressEvent(QKeyEvent *event);
};

#endif // CPP_QT_RETRY_MENU_H