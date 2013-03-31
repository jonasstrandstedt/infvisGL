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
	const std::string & getTimeName(int index);
	const glm::vec2 & getAttribRange(int attribute);
	
	bool isSet(int index, int time, int attrib);
	bool isSetAll(int index, int time);

	void setItem(int index, int time, int attrib, float value);
	void setAttribName(int attrib, const std::string name);
	void setEntryName(int index, const std::string name);
	void setTimeName(int index, const std::string name);

	void calculateAttribRanges();

	void interpolateAndClamp();

private:
	float * data;
	bool * dataExist;

	int dataCount[3];
	int totalDataCount;

	std::vector<std::string> *attribName;
	std::vector<std::string> *entryName;
	std::vector<std::string> *timeName;

	std::vector<glm::vec2> attribRange;

};