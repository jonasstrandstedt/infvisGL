#include "TextManager.h"

TextManager TextManager::instance = NULL;

TextManager * TextManager::getInstance()
{
	if(!TextManager::instance)
	{
		TextManager::instance = new TextManager();
	}
	return TextManager::instance;
}

void TextManager::printText(int x, int y, const char &text)
{
	
}

TextManager::TextManager()
{

}

TextManager::~TextManager()
{

}