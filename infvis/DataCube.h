#pragma once

#include <vector>
#include <string>

class DataCube
{
public:
	DataCube(int entries, int timeStamps, int attributes);
	~DataCube();

	float GetItem(int index, int time, int attrib);

	void SetItem(int index, int time, int attrib, float value);
	void SetAtrributeName(int attrib, const std::string &name);
	void SetEntryName(int index, const std::string &name);
private:
	float * data;

	int dataCount[3];

	std::vector<std::string> attribName;
	std::vector<std::string> entryName;
};