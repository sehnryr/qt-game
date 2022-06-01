#include "StartMenu.h"

StartMenu::StartMenu(QTimer *timer, QGraphicsScene *scene)
		: PauseMenu(timer, scene)
{
	this->setOptions({StartMenuOptions::Play,
										StartMenuOptions::Quit});
	this->setSelected(0);
	this->setTitle("thing maker");

	// Pause on launch
	this->setPauseStatus(true);
}

void StartMenu::keyPressEvent(QKeyEvent *event)
{
	QString selectedText = this->selectedText();
	switch (event->key())
	{
	case Qt::Key_Up:
		this->decrementSelect();
		Q_EMIT this->update();
		break;
	case Qt::Key_Down:
		this->incrementSelect();
		Q_EMIT this->update();
		break;
	case Qt::Key_Return:
	case Qt::Key_Enter:
		if (selectedText == StartMenuOptions::Play)
			this->resume();
		else
			QCoreApplication::quit();
		Q_EMIT optionSelected(selectedText);
		break;
	default:
		break;
	}
}