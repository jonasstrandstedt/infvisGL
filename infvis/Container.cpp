#include "Container.h"

Container::Container(glm::vec2 in_x, glm::vec2 in_y) 
{
	std::cout << "Container::Container()" << std::endl;

	glm::mat4 Orthogonal = glm::ortho(0.0f,1.0f, 0.0f, 1.0f, 0.1f, 100.0f);
	glm::mat4 OrthogonalView = glm::lookAt(		glm::vec3(0,0,1), // Camera pos
												glm::vec3(0,0,0), // and looks at the origin
												glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
																);
	_orthogonalProjectionMatrix = Orthogonal * OrthogonalView;

	setSize(in_x, in_y);
}
Container::~Container() 
{
}



void Container::setSize(glm::vec2 in_x, glm::vec2 in_y)
{

	x = in_x;
	y = in_y;

}

void Container::resize(glm::vec2 in_x, glm::vec2 in_y)
{
	setSize(in_x, in_y);
}

void Container::render()
{

}

/*
void Container::render()
{
	glViewport(	GLint  	x,
 	GLint  	y,
 	GLsizei  	width,
 	GLsizei  	height);


}
*/