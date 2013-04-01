#include "SplitContainer.h"

SplitContainer::SplitContainer(glm::vec2 in_x, glm::vec2 in_y) : Container(in_x, in_y)
{
	std::cout << "SplitContainer::SplitContainer()" << std::endl;
}
SplitContainer::~SplitContainer() 
{
}

void SplitContainer::addChild(Container *c)
{
	children.push_back(c);

	int size = children.size();
	float diff = (y[1]-y[0]) / (float) size;
	for (int i = 0; i < size; ++i)
	{
		Container *child = children.at(i);
		glm::vec2 out_x = glm::vec2(x[0],x[1]);
		glm::vec2 out_y = glm::vec2(y[0]+diff*i,y[0]+diff*(i+1));
		child->resize(out_x,out_y);
	}
}

void SplitContainer::resize(glm::vec2 in_x, glm::vec2 in_y)
{
	setSize(in_x, in_y);

	int size = children.size();
	float diff = (y[1]-y[0]) / (float) size;
	for (int i = 0; i < size; ++i)
	{
		Container *child = children.at(i);
		glm::vec2 out_x = glm::vec2(x[0],x[1]);
		glm::vec2 out_y = glm::vec2(y[0]+diff*i,y[0]+diff*(i+1));
		child->resize(out_x,out_y);
	}
}

void SplitContainer::render()
{
	for (int i = 0; i < children.size(); ++i)
	{
		Container *child = children.at(i);
		child->render();
	}


}
