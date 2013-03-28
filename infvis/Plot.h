#pragma once

#include "Container.h"
#include <iostream>
#include "DataCube.h"
#include "ColorMap.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "FBO.h"
#include "Circle.h"

enum {
	AXIS_X = 0,
	AXIS_Y,
	AXIS_Z
};

class Plot: public Container
{
public:
	Plot();
	Plot(glm::vec2 in_x, glm::vec2 in_y);
	~Plot();

	virtual void resize(glm::vec2 in_x, glm::vec2 in_y);
	void render();

	void setInput(DataCube *d);
	void setColorMap(ColorMap *c);
	void invalidate();
	bool isValid();
	void setBackgroundColor(glm::vec4 bc);
protected:
	DataCube *dc;
	ColorMap *colormap;

	virtual void renderPlot() = 0;

	void setValid();
private:
	glm::vec4 backgroundColor;
	gl4::FBO *_fbo;
	gl4::VBO *_quad;
	bool valid;
	unsigned int renders;
};