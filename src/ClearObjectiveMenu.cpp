#include <cmath>

#include "ClearObjectiveMenu.h"

ClearObjectiveMenu::ClearObjectiveMenu(QTimer *timer, QGraphicsScene *scene)
		: PauseMenu(timer, scene), _time(0)
{
	this->_delta = (qreal)this->interval() / 1000;

	this->setOptions({ClearObjectiveMenuOptions::Yes,
										ClearObjectiveMenuOptions::No});
	this->setSelected(0);
	this->setTitle("YOU WIN!");
}

QString ClearObjectiveMenu::stringTime(qreal const time)
{
	// Get time in minutes and seconds as QString
	QString output;
	qreal seconds = ((int)(fmod(time, 60) * 10)) / (qreal)10;
	output = QString::number(seconds);
	if (fmod(seconds, 1) == 0)
		output += ".0";
	output += "s";
	if ((int)time / 60 > 0)
		output = QString::number((int)time / 60) + "m" + output;
	return output;
}

void ClearObjectiveMenu::show(QPainter *painter, const QRectF &rect)
{
	QFont font = this->scene()->font();

	// Set fonts sizes
	font.setPixelSize(32);
	QFont font32 = font;

	font.setPixelSize(16);
	QFont font16 = font;

	font.setPixelSize(12);
	QFont font12 = font;

	qreal offsetX = 2;

	// Translucent background
	QColor color(ColorPalette::lightYellow);
	color.setAlpha(191);
	painter->fillRect(rect, color);

	// Draw title
	painter->setFont(font32);
	painter->setPen(QPen(ColorPalette::darkGray));
	painter->drawText(
			rect.translated(0, -48 + offsetX),
			Qt::AlignCenter,
			this->title());

	// Draw score
	painter->setFont(font12);
	painter->setPen(QPen(ColorPalette::darkGray));
	painter->drawText(
			rect.translated(0, -28 + offsetX),
			Qt::AlignCenter,
			"You have reached the objective");
	painter->drawText(
			rect.translated(0, -16 + offsetX),
			Qt::AlignCenter,
			"in " + this->stringTime(this->time()));

	QList<QPair<QString, qreal>> scores = *this->scores();
	std::sort(scores.begin(), scores.end(), QPairSecondComparer());

	if ((scores.length() > 0 && scores[0].second > this->time()) ||
			scores.length() == 0)
	{
		// Draw new best score message
		painter->setFont(font16);
		painter->setPen(QPen(ColorPalette::darkYellow));
		painter->drawText(
				rect.translated(0.5, 2 + 0.5 + offsetX),
				Qt::AlignCenter,
				"New best time!");
		painter->setPen(QPen(ColorPalette::lightGray));
		painter->drawText(
				rect.translated(-0.5, 2 - 0.5 + offsetX),
				Qt::AlignCenter,
				"New best time!");
	}
	else
	{
		// Draw best score + username
		painter->setFont(font12);
		painter->drawText(
				rect.translated(0, -4 + offsetX),
				Qt::AlignCenter,
				"Best score is " + this->stringTime(scores[0].second));
		QString username = scores[0].first;
		username = username.length() > 25 ? username.left(25) + "..." : username;
		painter->drawText(
				rect.translated(0, 8 + offsetX),
				Qt::AlignCenter,
				" by " + username);
	}

	painter->setFont(font12);
	painter->setPen(QPen(ColorPalette::darkGray));
	painter->drawText(
			rect.translated(0, 20 + offsetX),
			Qt::AlignCenter,
			"Would you like to leave your");
	painter->drawText(
			rect.translated(0, 32 + offsetX),
			Qt::AlignCenter,
			"name in the records?");

	QPointF offset = rect.center() + QPoint(-4, 52 + offsetX);
	if (this->selectedText() == ClearObjectiveMenuOptions::Yes)
	{
		painter->setPen(QPen(ColorPalette::darkYellow));
		painter->drawText(
				rect.translated(-24 + 0.5, 48 + 0.5 + offsetX),
				Qt::AlignCenter,
				ClearObjectiveMenuOptions::Yes);
		painter->setPen(QPen(ColorPalette::lightGray));
		painter->drawText(
				rect.translated(-24 - 0.5, 48 - 0.5 + offsetX),
				Qt::AlignCenter,
				ClearObjectiveMenuOptions::Yes);

		painter->setPen(QPen(ColorPalette::darkGray));
		painter->drawText(
				rect.translated(24, 48 + offsetX),
				Qt::AlignCenter,
				ClearObjectiveMenuOptions::No);

		QPixmap arrow("resources/arrow.png");
		painter->drawPixmap(
				offset + QPoint(-24, 0),
				arrow);
	}
	else if (this->selectedText() == ClearObjectiveMenuOptions::No)
	{
		painter->setPen(QPen(ColorPalette::darkGray));
		painter->drawText(
				rect.translated(-24, 48 + offsetX),
				Qt::AlignCenter,
				ClearObjectiveMenuOptions::Yes);

		painter->setPen(QPen(ColorPalette::darkYellow));
		painter->drawText(
				rect.translated(24 + 0.5, 48 + 0.5 + offsetX),
				Qt::AlignCenter,
				ClearObjectiveMenuOptions::No);
		painter->setPen(QPen(ColorPalette::lightGray));
		painter->drawText(
				rect.translated(24 - 0.5, 48 - 0.5 + offsetX),
				Qt::AlignCenter,
				ClearObjectiveMenuOptions::No);

		QPixmap arrow("resources/arrow.png");
		painter->drawPixmap(
				offset + QPoint(24, 0),
				arrow);
	}
}

void ClearObjectiveMenu::keyPressEvent(QKeyEvent *event)
{
	QString selectedText = this->selectedText();
	switch (event->key())
	{
	case Qt::Key_Left:
		this->decrementSelect();
		Q_EMIT this->update();
		break;
	case Qt::Key_Right:
		this->incrementSelect();
		Q_EMIT this->update();
		break;
	case Qt::Key_Return:
	case Qt::Key_Enter:
		if (selectedText == ClearObjectiveMenuOptions::Yes)
			this->resume();
		else if (selectedText == ClearObjectiveMenuOptions::No)
			this->resume();
		Q_EMIT optionSelected(selectedText);
		break;
	default:
		break;
	}
}

void ClearObjectiveMenu::timeUpdate()
{
	this->_time += this->delta();
}