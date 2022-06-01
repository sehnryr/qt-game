#include "RetryMenu.h"

RetryMenu::RetryMenu(QTimer *timer, QGraphicsScene *scene)
		: PauseMenu(timer, scene)
{
	this->setOptions({RetryMenuOptions::Retry,
										RetryMenuOptions::Quit});
	this->setSelected(0);
	this->setTitle(NULL);
}

void RetryMenu::keyPressEvent(QKeyEvent *event)
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
		if (selectedText == RetryMenuOptions::Retry)
			this->resume();
		else if (selectedText == RetryMenuOptions::Quit)
			QCoreApplication::quit();
		Q_EMIT optionSelected(selectedText);
		break;
	default:
		break;
	}
}