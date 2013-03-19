#pragma once

#include "DataCube.h"
#include <string>
#include <vector>

class DataLoader {
public:
	DataLoader();
	~DataLoader();

	void addAttribFromFile(std::string attribName, std::string filename);

	DataCube * getDataCube();
private:

	std::vector<std::string> attribNames;
	std::vector<std::string> filenames;
};