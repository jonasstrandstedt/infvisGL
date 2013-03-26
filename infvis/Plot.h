#pragma once

#include "Container.h"
#include <iostream>
#include "DataCube.h"
#include "ColorMap.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>

class Plot: public Container
{
public:
	Plot(glm::vec2 in_x, glm::vec2 in_y);
	~Plot();

	virtual void resize(glm::vec2 in_x, glm::vec2 in_y);

	void setInput(DataCube *d);
	void setColorMap(ColorMap *c);
protected:
	DataCube *dc;
	ColorMap *colormap;
private:
};