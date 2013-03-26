#include "ColorMap.h"

ColorMap::ColorMap() :
	dc(NULL),
	range(0.0,1.0),
	axis(0)
{

}

ColorMap::~ColorMap()
{

}

void ColorMap::addPart(Part * _part)
{
	if(_part)
		parts.push_back(_part);
}

void ColorMap::clearParts()
{
	parts.clear();
}

void ColorMap::setDataCube(DataCube * _cube)
{
	if(_cube)
	{
		dc = _cube;
		updateRange();
	}
}

void ColorMap::setAxis(int _axis)
{
	if(axis >= 0)
	{
		axis = _axis;
		updateRange();
	}
}

glm::vec3 ColorMap::map(float _value)
{
	if(parts.size() > 0)
	{
		// Remap value to [0-1]
		_value = (_value - range[0]) / (range[1] - range[0]);

		int index = (int)( _value * 0.99 * (float)(parts.size()) );

		float min = (float)index / (float)(parts.size());
		float max = (float)(index+1) / (float)(parts.size());
		_value = (_value - min) / (max - min);

		return parts[index]->map(_value);
	}
	return glm::vec3(1.0);
}

void ColorMap::updateRange()
{
	if(dc)
	{
		const int * dataCount = dc->getDataCount();

		if(axis < dataCount[3])
			range = dc->getAttribRange(axis);
	}
}

ColorMap::LinearPart::LinearPart(const glm::vec3 &_low, const glm::vec3 &_high) :
	lowColor(_low),
	highColor(_high)
{

}

glm::vec3 ColorMap::LinearPart::map(float _value)
{
	return lowColor*(1.0f-_value) + highColor*(_value);
}