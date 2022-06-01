#include "PauseMenu.h"

PauseMenu::PauseMenu(QTimer *timer, QGraphicsScene *scene)
		: BaseMenu(scene), _timer(timer), _paused(false)
{
	this->_interval = this->timer()->interval();

	this->setOptions({PauseMenuOptions::Restart,
										PauseMenuOptions::Resume,
										PauseMenuOptions::Quit});
	this->setSelected(1);
	this->setTitle("Pause");
}

void PauseMenu::setPauseStatus(bool const status)
{
	if (status != this->paused())
	{
		if (status)
			this->timer()->stop();
		else
			this->timer()->start(this->interval());

		this->_paused = status;
	}
}

void PauseMenu::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape || event->key() == Qt::Key_Pause)
	{
		if (this->paused())
			this->resume();
		else
			this->pause();
		Q_EMIT this->update();
	}

	if (this->paused())
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
			if (selectedText == PauseMenuOptions::Restart)
				this->resume();
			else if (selectedText == PauseMenuOptions::Quit)
				QCoreApplication::quit();
			else
				this->resume();
			Q_EMIT optionSelected(selectedText);
			break;
		default:
			break;
		}
	}
}