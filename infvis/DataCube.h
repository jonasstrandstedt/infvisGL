#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

class DataCube
{
public:
	DataCube(int entries, int timeStamps, int attributes);
	~DataCube();

	float GetItem(int index, int time, int attrib);
	const std::string & GetAttribName(int attrib);
	const std::string & GetEntryName(int index);
	const glm::vec2 & GetAttribRange(int attribute);

	void SetItem(int index, int time, int attrib, float value);
	void SetAtrribName(int attrib, const std::string &name);
	void SetEntryName(int index, const std::string &name);

	void CalculateAttribRanges();
private:
	float * data;

	int dataCount[3];
	int totalDataCount;

	std::vector<std::string> attribName;
	std::vector<glm::vec2> attribRange;

	std::vector<std::string> entryName;
};