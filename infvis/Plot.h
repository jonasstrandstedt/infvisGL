#pragma once

#include "Container.h"
#include <iostream>
#include "DataCube.h"
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
protected:
	DataCube *dc;
private:
};