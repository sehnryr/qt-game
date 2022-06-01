#ifndef CPP_QT_BASE_MENU_H
#define CPP_QT_BASE_MENU_H

#include <QGraphicsScene>
#include <QPainter>
#include <QWidget>
#include <QRectF>
#include <QKeyEvent>
#include <QCoreApplication>

#include "ColorPalette.h"

class BaseMenu : public QObject
{
	Q_OBJECT
private:
	QGraphicsScene *_scene;
	QVector<QString> _options;
	int _selected = -1;
	QPointF _optionsOffset = QPointF(0, 0);
	QString _title = NULL;

protected:
	QGraphicsScene *scene() const { return _scene; }
	QVector<QString> options() const { return _options; }
	QPointF optionsOffset() const { return _optionsOffset; }
	QString title() const { return _title; }

	void setOptions(QVector<QString> const options) { _options = options; }
	void setSelected(int const selected) { _selected = selected; }
	void setTitle(QString const title) { _title = title; }

public:
	BaseMenu(QGraphicsScene *scene);
	~BaseMenu() {}

	void show(QPainter *painter, const QRectF &rect);

	int decrementSelect();
	int incrementSelect();

	int selected() const { return _selected; }
	QString selectedText() const { return options()[selected()]; }
signals:
	void optionSelected(QString option);
	void update();
};

#endif // CPP_QT_BASE_MENU_H