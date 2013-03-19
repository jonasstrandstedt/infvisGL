#pragma once

#include <vector>
#include <string>

class DataCube
{
public:
	DataCube(int entries, int timeStamps, int attributes) :
	data(NULL)
	{
		int totalSize = entries * timeStamps * attributes;
		data = new float[totalSize];

		dataCount[0] = entries;
		dataCount[1] = timeStamps;
		dataCount[2] = attributes;

		for(int i=0; i < dataCount[0]*dataCount[1]*dataCount[2]; ++i)
			data[i] = 0.0f;
	}
	~DataCube()
	{
		if(data)
			delete[] data;
	}
	inline float GetItem(int index, int time, int attrib)
	{
		// Assert bounds
		return data[index * (dataCount[1]*dataCount[2]) + time * dataCount[2] + attrib];
	}

	inline void SetItem(int index, int time, int attrib, float value)
	{
		data[index * (dataCount[1]*dataCount[2]) + time * dataCount[2] + attrib] = value;
	}

	void SetAtrributeName(int attrib, const std::string &name)
	{
		attribName[attrib] = name;
	}

	void SetEntryName(int index, const std::string &name)
	{
		entryName[index] = name;
	}
private:
	float * data;

	int dataCount[3];

	std::vector<std::string> attribName;
	std::vector<std::string> entryName;
};