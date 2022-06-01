#ifndef CPP_QT_START_MENU_H
#define CPP_QT_START_MENU_H

#include "PauseMenu.h"

namespace StartMenuOptions
{
	const QString Play = "play";
	const QString Quit = "quit";
};

class StartMenu : public PauseMenu
{
	Q_OBJECT
public:
	StartMenu(QTimer *timer, QGraphicsScene *scene);
	void keyPressEvent(QKeyEvent *event);
};

#endif // CPP_QT_START_MENU_H