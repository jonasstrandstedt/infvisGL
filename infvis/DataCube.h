#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

class DataCube
{
public:
	DataCube(int entries, int timeStamps, int attributes);
	~DataCube();


	const int * getDataCount();
	float getItem(int index, int time, int attrib);
	const std::string & getAttribName(int attrib);
	const std::string & getEntryName(int index);
	const glm::vec2 & getAttribRange(int attribute);

	void setItem(int index, int time, int attrib, float value);
	void setAtrribName(int attrib, const std::string &name);
	void setEntryName(int index, const std::string &name);

	void calculateAttribRanges();

private:
	float * data;

	int dataCount[3];
	int totalDataCount;

	std::vector<std::string> attribName;
	std::vector<glm::vec2> attribRange;

	std::vector<std::string> entryName;
};