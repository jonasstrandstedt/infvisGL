#include "ColorMap.h"

#include <cstdio>

ColorMap::ColorMap() :
	dc(NULL),
	range(0.0,1.0),
	index(0)
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

void ColorMap::setIndex(int _index)
{
	if(_index >= 0)
	{
		index = _index;
		updateRange();
	}
}


int ColorMap::getIndex()
{
	return index;
}

glm::vec3 ColorMap::map(float _value)
{
	if(parts.size() > 0)
	{
		// Remap value to [0-1]
		_value = (_value - range[0]) / (range[1] - range[0]);

		int index = (int)( _value * 0.99999 * (float)(parts.size()) );

		float min = (float)index / (float)(parts.size());
		float max = (float)(index+1) / (float)(parts.size());
		_value = (_value - min) / (max - min);

		index = glm::clamp(index, 0, (int)parts.size()-1);
		return parts[index]->map(_value);
	}
	return glm::vec3(1.0);
}

void ColorMap::updateRange()
{
	if(dc)
	{
		const int * dataCount = dc->getDataCount();

		if(index < dataCount[3])
			range = dc->getAttribRange(index);
	}
}

ColorMap::LinearPart::LinearPart(const glm::vec3 &_low, const glm::vec3 &_high) :
	lowColor(_low),
	highColor(_high)
{

}

glm::vec3 ColorMap::LinearPart::map(float _value)
{
	return lowColor*(1.0f-_value) + highColor * _value;
}

ColorMap::HSVPart::HSVPart(const glm::vec3 &_low, const glm::vec3 &_high)
{
	lowHSV = _low;
	lowHSV.y = glm::clamp(lowHSV.y, 0.0f, 1.0f);
	lowHSV.z = glm::clamp(lowHSV.z, 0.0f, 1.0f);

	highHSV = _high;
	highHSV.y = glm::clamp(highHSV.y, 0.0f, 1.0f);
	highHSV.z = glm::clamp(highHSV.z, 0.0f, 1.0f);
}

glm::vec3 ColorMap::HSVPart::map(float _value)
{
	glm::vec3 hsv = lowHSV*(1.0f-_value) + highHSV * _value;

	float h = fmodf(hsv.x, 360.0);

	float C = hsv.y * hsv.z;
	float H = h/60.0;
	float X = C * (1.0 - glm::abs(fmodf(H,2.0) - 1.0));

	glm::vec3 rgb;

	if 		(0.0 <= H && H < 1.0)
		rgb = glm::vec3(C,X,0);
	else if (1.0 <= H && H < 2.0)
		rgb = glm::vec3(X,C,0);
	else if (2.0 <= H && H < 3.0)
		rgb = glm::vec3(0,C,X);
	else if (3.0 <= H && H < 4.0)
		rgb = glm::vec3(0,X,C);
	else if (4.0 <= H && H < 5.0)
		rgb = glm::vec3(X,0,C);
	else if (5.0 <= H && H < 6.0)
		rgb = glm::vec3(C,0,X);

	/*
	printf("hsv:(%3.1f, %1.1f, %1.1f), H: %i, rgb:(%1.1f, %1.1f, %1.1f) \n",
		hsv.x, hsv.y, hsv.z,
		H,
		rgb.x, rgb.y, rgb.z);
	*/

	rgb += hsv.z - C;

	return rgb;
}