#include "SplitContainer.h"

SplitContainer::SplitContainer(glm::vec2 in_x, glm::vec2 in_y) : Container(in_x, in_y)
{
	std::cout << "SplitContainer::SplitContainer()" << std::endl;
	top = 0;
	bottom = 0;
}
SplitContainer::~SplitContainer() 
{
}

void SplitContainer::setTopChild(Container *c)
{
	top = c;
	glm::vec2 out_x = glm::vec2(x[0],x[1]);
	glm::vec2 out_y = glm::vec2(y[0]+(y[1]-y[0])/2.0f,y[1]);
	top->resize(out_x,out_y);
}

void SplitContainer::setBottomChild(Container *c)
{
	bottom = c;
	glm::vec2 out_x = glm::vec2(x[0],x[1]);
	glm::vec2 out_y = glm::vec2(y[0],y[0]+(y[1]-y[0])/2.0f);
	bottom->resize(out_x,out_y);
}

void SplitContainer::resize(glm::vec2 in_x, glm::vec2 in_y)
{
	setSize(in_x, in_y);

	if (top != 0)
	{
		glm::vec2 out_x = glm::vec2(x[0],x[1]);
		glm::vec2 out_y = glm::vec2(y[0]+(y[1]-y[0])/2.0f,y[1]);
		top->resize(out_x, out_y);
	}
	if (bottom != 0)
	{
		glm::vec2 out_x = glm::vec2(x[0],x[1]);
		glm::vec2 out_y = glm::vec2(y[0],y[0]+(y[1]-y[0])/2.0f);
		bottom->resize(out_x, out_y);
	}
}

void SplitContainer::render()
{
	if (top != 0)
	{
		top->render();
	}
	if (bottom != 0)
	{
		bottom->render();
	}


}


/*
glViewport(	GLint  	x,
 	GLint  	y,
 	GLsizei  	width,
 	GLsizei  	height);

 	*/