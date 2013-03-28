#include "TreemapPlot.h"

TreemapPlot::TreemapPlot() : Plot()
{
	std::cout << "TreemapPlot::TreemapPlot()" << std::endl;

	primitive = new gl4::VBO();
	primitive->setProportions(1.0,1.0);
	primitive->init();
}

TreemapPlot::TreemapPlot(glm::vec2 in_x, glm::vec2 in_y) : Plot(in_x, in_y)
{
	std::cout << "TreemapPlot::~TreemapPlot(glm::vec2 in_x, glm::vec2 in_y)" << std::endl;

}

TreemapPlot::~TreemapPlot()
{
	std::cout << "TreemapPlot::~TreemapPlot()" << std::endl;
	delete primitive;
}

void TreemapPlot::renderPlot()
{

}