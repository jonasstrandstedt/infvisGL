#pragma once

#include "Container.h"
#include <iostream>
#include <vector>
#include <string>
#include <glm/glm.hpp>

class SplitContainer: public Container
{
public:
	SplitContainer(glm::vec2 in_x, glm::vec2 in_y);
	~SplitContainer();

	void addChild(Container *c);

	virtual void resize(glm::vec2 in_x, glm::vec2 in_y);
	virtual void render();

private:

	std::vector<Container*> children;
};

