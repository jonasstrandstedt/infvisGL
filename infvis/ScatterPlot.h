#pragma once

#include "Plot.h"
#include <iostream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "Circle.h"

class ScatterPlot: public Plot
{
public:
	ScatterPlot(glm::vec2 in_x, glm::vec2 in_y);
	~ScatterPlot();

	void setYear(int y);
	void render();
protected:
private:

	int year;
	gl4::Circle *primitive;
};