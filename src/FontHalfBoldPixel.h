#ifndef CPP_QT_FONT_HALF_BOLD_PIXEL_H
#define CPP_QT_FONT_HALF_BOLD_PIXEL_H

#include <QFont>
#include <QFontDatabase>

class FontHalfBoldPixel : public QFont
{
private:
	const int _id = QFontDatabase::addApplicationFont("resources/HalfBoldPixel7-2rw3.ttf");
	const QString _family = QFontDatabase::applicationFontFamilies(_id).at(0);

public:
	explicit FontHalfBoldPixel() : QFont()
	{
		this->setFamily(_family);
		this->setPixelSize(16);
	}
};

#endif // CPP_QT_FONT_HALF_BOLD_PIXEL_H