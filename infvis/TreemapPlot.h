#pragma once

#include "Plot.h"

#include <iostream>
#include <vector>
#include <string>
#include <glm/glm.hpp>

class TreemapPlot: public Plot
{
public:
	TreemapPlot();
	TreemapPlot(glm::vec2 in_x, glm::vec2 in_y);
	~TreemapPlot();

	void renderPlot();

protected:
private:
	gl4::VBO *primitive;
};