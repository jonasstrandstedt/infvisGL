#include "Plot.h"

Plot::Plot(glm::vec2 in_x, glm::vec2 in_y) : Container(in_x, in_y) 
{
	std::cout << "Plot::Plot()" << std::endl;
	dc = 0;
}
Plot::~Plot()
{

}

void Plot::resize(glm::vec2 in_x, glm::vec2 in_y){
	setSize(in_x, in_y);
}


void Plot::setColorMap(ColorMap *c)
{
	colormap = c;
}

void Plot::setInput(DataCube *d) {
	dc = d;
}