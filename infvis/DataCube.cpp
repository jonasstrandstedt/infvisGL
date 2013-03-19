#include "DataCube.h"

DataCube::DataCube(int entries, int timeStamps, int attributes) :
data(NULL)
{
	int totalSize = entries * timeStamps * attributes;
	data = new float[totalSize];

	dataCount[0] = entries;
	dataCount[1] = timeStamps;
	dataCount[2] = attributes;

	for(int i=0; i < totalSize; ++i)
		data[i] = 0.0f;
}

DataCube::~DataCube()
{
	if(data)
		delete[] data;
}
float DataCube::GetItem(int index, int time, int attrib)
{
	// Assert bounds
	return data[index * (dataCount[1]*dataCount[2]) + time * dataCount[2] + attrib];
}

void DataCube::SetItem(int index, int time, int attrib, float value)
{
	data[index * (dataCount[1]*dataCount[2]) + time * dataCount[2] + attrib] = value;
}

void DataCube::SetAtrributeName(int attrib, const std::string &name)
{
	attribName[attrib] = name;
}

void DataCube::SetEntryName(int index, const std::string &name)
{
	entryName[index] = name;
}