#include "DataCube.h"

DataCube::DataCube(int entries, int timeStamps, int attributes) :
data(NULL)
{
	totalDataCount = entries * timeStamps * attributes;
	data = new float[totalDataCount];

	attribRange.resize(attributes,glm::vec2(0.0,1.0));

	dataCount[0] = entries;
	dataCount[1] = timeStamps;
	dataCount[2] = attributes;

	for(int i=0; i < totalDataCount; ++i)
		data[i] = 0.0f;
}

DataCube::~DataCube()
{
	if(data)
		delete[] data;
}

const int * DataCube::getDataCount()
{
	return dataCount;
}

float DataCube::getItem(int index, int time, int attrib)
{
	// Assert bounds
	return data[index * (dataCount[1]*dataCount[2]) + time * dataCount[2] + attrib];
}

const std::string & DataCube::getAttribName(int attrib)
{
	return attribName[attrib];
}

const std::string & DataCube::getEntryName(int index)
{
	return entryName[index];
}

const glm::vec2 & DataCube::getAttribRange(int attrib)
{
	return attribRange[attrib];
}

void DataCube::setItem(int index, int time, int attrib, float value)
{
	data[index * (dataCount[1]*dataCount[2]) + time * dataCount[2] + attrib] = value;
}

void DataCube::setAtrribName(int attrib, const std::string &name)
{
	attribName[attrib] = name;
}

void DataCube::setEntryName(int index, const std::string &name)
{
	entryName[index] = name;
}

void DataCube::calculateAttribRanges()
{
	for(int z=0; z<dataCount[2]; ++z)
	{
		attribRange[z] = glm::vec2(1.0e27, -1.0e27);
	}

	for(int x=0; x<dataCount[0]; ++x)
	{
		for(int y=0; y<dataCount[1]; ++y)
		{
			for(int z=0; z<dataCount[2]; ++z)
			{
				attribRange[z][0] = glm::min(getItem(x, y, z), attribRange[z][0]);
				attribRange[z][1] = glm::max(getItem(x, y, z), attribRange[z][1]);
			}
		}
	}
}