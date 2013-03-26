#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Container
{
public:
	Container(glm::vec2 in_x, glm::vec2 in_y);
	~Container();

	void setSize(glm::vec2 in_x, glm::vec2 in_y);
	virtual void resize(glm::vec2 in_x, glm::vec2 in_y);
	virtual void render();
protected:
	glm::vec2 x;
	glm::vec2 y;
	glm::mat4 _orthogonalProjectionMatrix;
private:
};