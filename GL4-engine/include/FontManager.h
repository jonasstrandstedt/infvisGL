#pragma once

#include "VBO.h"
#include <vector>
#include <glm/glm.hpp>

class FontManager
{
public:
	static FontManager * getInstance();

	void render(float width=1.0f, float height=1.0f);
	void addText(float x, float y, const char * text, float fontSize=32.0f, const glm::vec3 &color=glm::vec3(1.0));
private:
	static FontManager * instance;

	typedef struct
	{
	    int x,y,width,height;
	    float x_offset,y_offset,x_advance;
	    float s0, t0, s1, t1;
	    unsigned char index;
	}Character;

	class FontVBO;

	FontManager();
	~FontManager();

	void loadFontData(const char * filename, int width, int height);

	FontVBO * buffer;
	std::vector<Character> charmap;

	int fontmapWidth, fontmapHeight;
	float characterSize;
};

class FontManager::FontVBO : public gl4::VBO
{
public:
	FontVBO();
	~FontVBO();

	void push_back(Vertex * vData, int vCount, int * iData, int iCount);
	virtual void init();
private:
	std::vector<Vertex> vertices;
	std::vector<int> indices;
};