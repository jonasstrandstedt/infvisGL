#include "DataCube.h"
#include <assert.h> 
#include <iostream>

DataCube::DataCube(int entries, int timeStamps, int attributes) :
data(NULL)
{
	totalDataCount = entries * timeStamps * attributes;
	data = new float[totalDataCount];
	dataExist = new bool[totalDataCount];

	attribRange.resize(attributes,glm::vec2(0.0,1.0));

	dataCount[0] = entries;
	dataCount[1] = timeStamps;
	dataCount[2] = attributes;

	for(int i=0; i < totalDataCount; ++i)
	{
		data[i] = 0.0f;
		dataExist[i] = false;
	}
}

DataCube::~DataCube()
{
	if(data)
		delete[] data;
	if(dataExist)
		delete[] dataExist;
}

const int * DataCube::getDataCount()
{
	return dataCount;
}

float DataCube::getItem(int index, int time, int attrib)
{
	// Assert bounds

	assert (index >= 0);
	assert (time >= 0);
	assert (attrib >= 0);
	assert (index < dataCount[0]);
	assert (time < dataCount[1]);
	assert (attrib < dataCount[2]);

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

	assert (index >= 0);
	assert (time >= 0);
	assert (attrib >= 0);
	assert (index < dataCount[0]);
	assert (time < dataCount[1]);
	assert (attrib < dataCount[2]);

	int pos = index * (dataCount[1]*dataCount[2]) + time * dataCount[2] + attrib; 
	dataExist[pos] = true;
	data[pos] = value;
}

bool DataCube::isSet(int index, int time, int attrib)
{
	int pos = index * (dataCount[1]*dataCount[2]) + time * dataCount[2] + attrib; 
	return dataExist[pos];
}

bool DataCube::isSetAll(int index, int time)
{
	for (int i = 0; i < dataCount[2]; ++i)
	{
		int pos = index * (dataCount[1]*dataCount[2]) + time * dataCount[2] + i;
		if ( ! dataExist[pos])
		{
			return false;
		}
	}
	return true;
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
				if (isSet(x,y,z))
				{
					attribRange[z][0] = glm::min(getItem(x, y, z), attribRange[z][0]);
					attribRange[z][1] = glm::max(getItem(x, y, z), attribRange[z][1]);
				}
			}
		}
	}
}


void DataCube::interpolateAndClamp()
{
	for (int entry = 0; entry < dataCount[0]; ++entry)
	{
		for (int attribute = 0; attribute < dataCount[2]; ++attribute)
		{
			int last_data_timestamp = -1;
			for (int timeStamp = 0; timeStamp < dataCount[1]; ++timeStamp)
			{
				int pos = entry * (dataCount[1]*dataCount[2]) + timeStamp * dataCount[2] + attribute;
				int last_data_pos = entry * (dataCount[1]*dataCount[2]) + last_data_timestamp * dataCount[2] + attribute;
				//std::cout << "timestamp = " << timeStamp << std::endl;
				//std::cout << "last_data_timestamp = " << last_data_timestamp << std::endl;
				if ( dataExist[pos] &&  last_data_timestamp < timeStamp -1 && last_data_timestamp != -1)
				{
					float startdata = data[last_data_pos];
					float stopdata = data[pos];
					float diff = stopdata - startdata;
					int nrofsteps = timeStamp - last_data_timestamp;
					float stepsize = diff / (float)nrofsteps;

					//std::cout << "About to interpolate: " << last_data_timestamp << " -> " << timeStamp << std::endl;
					for (int i = 1; i < nrofsteps; ++i)
					{
						if (last_data_timestamp + i >= dataCount[1])
						{
							std::cout << "timestamp = " << timeStamp << std::endl;
							std::cout << "last_data_timestamp = " << last_data_timestamp << std::endl;
							std::cout << "nrofsteps = " << nrofsteps << std::endl;
							std::cout << last_data_timestamp + i << " -> " << startdata + (i)* stepsize << std::endl;
						}
						//std::cout << last_data_timestamp + i << " -> " << startdata + (i)* stepsize << std::endl;
						setItem(entry, last_data_timestamp + i, attribute, startdata + (i)* stepsize);
					}
					last_data_timestamp = timeStamp;
				} else if (dataExist[pos])
				{
					last_data_timestamp = timeStamp;
				}
			}
		}
	}
}