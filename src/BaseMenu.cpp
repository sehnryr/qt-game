#include "BaseMenu.h"

BaseMenu::BaseMenu(QGraphicsScene *scene) : _scene(scene) {}

void BaseMenu::show(QPainter *painter, const QRectF &rect)
{
	QFont font = this->scene()->font();

	// Set fonts sizes
	font.setPixelSize(32);
	QFont font32 = font;
	QFontMetrics metrics32(font32);

	font.setPixelSize(16);
	QFont font16 = font;
	QFontMetrics metrics16(font16);

	// Translucent background
	QColor color(ColorPalette::lightYellow);
	color.setAlpha(191);
	painter->fillRect(rect, color);

	int titleHeight = metrics32.boundingRect(this->title()).height();
	int optionsHeight = 16 * this->options().size();
	int separatorHeight = 12;
	int itemsHeight = (this->title() != NULL ? titleHeight : 0) +
										separatorHeight +
										optionsHeight;

	QPoint titleOffset(0, -itemsHeight / 2 + titleHeight / 2);
	QPoint optionsOffset(0, titleOffset.y() + titleHeight / 2 + separatorHeight);

	// Shows title
	if (this->title() != NULL)
	{
		painter->setFont(font32);
		painter->setPen(QPen(ColorPalette::darkGray));
		painter->drawText(
				rect.translated(titleOffset),
				Qt::AlignCenter,
				this->title());
	}

	// Shows options
	painter->setFont(font16);
	for (int i = 0; i < this->options().size(); i++)
	{
		QString optionText = this->options()[i];
		QPoint optionPos = QPoint(0, 16 * i) + optionsOffset;

		if (i == this->selected())
		{
			// Draw shadow
			painter->setPen(QPen(ColorPalette::darkYellow));
			painter->drawText(
					rect.translated(optionPos + QPointF(0.5, 0.5)),
					Qt::AlignCenter,
					optionText);

			// Draw translated text
			painter->setPen(QPen(ColorPalette::darkGray));
			painter->drawText(
					rect.translated(optionPos - QPointF(0.5, 0.5)),
					Qt::AlignCenter,
					optionText);

			// Draw arrow to the left of the selected option
			QPointF textCenter = rect.center() + optionPos;
			qreal textWidth = metrics16.boundingRect(optionText).width();
			QPointF arrowPos = QPointF(textCenter.x() - textWidth / 2, textCenter.y());

			QPixmap arrow("resources/arrow.png");
			painter->drawPixmap(
					arrowPos - QPointF(arrow.width() + 4, arrow.height() / 2),
					arrow.transformed(QTransform().rotate(+90)));
		}
		else
		{
			painter->setPen(QPen(ColorPalette::darkGray));
			painter->drawText(
					rect.translated(optionPos),
					Qt::AlignCenter,
					optionText);
		}
	}
}

int BaseMenu::decrementSelect()
{
	if (this->options().size() > 0)
		this->_selected = this->selected() > 0
													? this->selected() - 1
													: this->options().size() - 1;
	return this->selected();
}

int BaseMenu::incrementSelect()
{
	if (this->options().size() > 0)
		this->_selected = this->selected() < this->options().size() - 1
													? this->selected() + 1
													: 0;
	return this->selected();
}