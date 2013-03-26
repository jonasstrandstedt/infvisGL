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

	void setSizeIndex(int sa);
	void setAxisIndex(int axis, int index);
protected:
private:

	int x_index;
	int y_index;
	int sizeIndex;
	int year;
	gl4::Circle *primitive;
};