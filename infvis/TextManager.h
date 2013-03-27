#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

class TextManager
{
public:
	TextManager * getInstance();

	void printText(int x, int y, const char &text);
private:
	TextManager();
	~TextManager();

	static TextManager * instance;
}