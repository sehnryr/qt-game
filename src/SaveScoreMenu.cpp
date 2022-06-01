#include <QDebug>

#include "SaveScoreMenu.h"
#include "ClearObjectiveMenu.h"

SaveScoreMenu::SaveScoreMenu(QTimer *timer, QGraphicsScene *scene)
		: PauseMenu(timer, scene)
{
	this->setTitle(NULL);
}

void SaveScoreMenu::show(QPainter *painter, const QRectF &rect)
{
	QFont font = this->scene()->font();

	// Set fonts sizes
	font.setPixelSize(16);
	QFont font16 = font;
	QFontMetrics metrics16(font16);

	// Translucent background
	QColor color(ColorPalette::lightYellow);
	color.setAlpha(191);
	painter->fillRect(rect, color);

	QString nameLine = "name: " + this->username();
	QString scoreLine = "score: " + ClearObjectiveMenu::stringTime(this->timeScore());

	painter->setPen(QPen(ColorPalette::darkGray));
	painter->drawText(
			rect.translated(0, -8),
			Qt::AlignCenter,
			scoreLine);
	painter->drawText(
			rect.translated(0, 8),
			Qt::AlignCenter,
			nameLine);

	QPointF rectCenter = rect.center();
	QPoint nameLineBottomRight = metrics16.boundingRect(nameLine).bottomRight();

	QPixmap arrow("resources/arrow.png");
	painter->drawPixmap(
			rectCenter + nameLineBottomRight / 2 + QPoint(0, 8),
			arrow);
}

void SaveScoreMenu::keyPressEvent(QKeyEvent *event)
{
	if ((Qt::Key_A <= event->key() && event->key() <= Qt::Key_Z) ||
			event->key() == Qt::Key_Space)
	{
		this->_username += event->text();
		Q_EMIT this->update();
	}
	else if (event->key() == Qt::Key_Backspace)
	{
		this->_username.resize(this->username().length() - 1);
		Q_EMIT this->update();
	}
	else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
	{
		this->stage()->addScore(QPair(this->username(), this->timeScore()));
		this->resume();
		Q_EMIT this->optionSelected(SaveScoreMenuOptions::Register);
	}
}