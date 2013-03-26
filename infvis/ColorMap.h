#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "DataCube.h"

class ColorMap
{
public:
	class Part;
	class LinearPart;
	class HSVPart;

	ColorMap();
	~ColorMap();

	void addPart(Part * _part);
	void clearParts();

	void setDataCube(DataCube * _cube);
	void setIndex(int _index);

	glm::vec3 map(float _value);
private:
	void updateRange();

	std::vector<Part*> parts;

	DataCube * dc;
	glm::vec2 range;
	int index;
};

class ColorMap::Part
{
public:
	virtual glm::vec3 map(float _value) = 0;
};

class ColorMap::LinearPart : public ColorMap::Part
{
public:
	LinearPart(const glm::vec3 &_low, const glm::vec3 &_high);

	glm::vec3 map(float _value);
private:
	glm::vec3 lowColor;
	glm::vec3 highColor;
};

class ColorMap::HSVPart : public ColorMap::Part
{
public:
	HSVPart(const glm::vec3 &_low, const glm::vec3 &_high);

	glm::vec3 map(float _value);
private:
	glm::vec3 lowHSV;
	glm::vec3 highHSV;
};