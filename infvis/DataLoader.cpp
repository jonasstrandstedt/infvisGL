#include "DataLoader.h"

#include <iostream>
#include <fstream>
#include <ifstream>

DataLoader::DataLoader()
{

}
DataLoader::~DataLoader()
{

}

void DataLoader::addAttribFromFile(std::string attribName, std::string filename)
{
	attribNames.push_back(attribName);
	filenames.push_back(filename);
}

DataCube * DataLoader::getDataCube()
{

	int attribs = attribNames.size();
	int items = 0;
	int t = 0;

	if (attribs > 0)
	{

		int cols = 0;
		std::ifstream *file = new std::ifstream(attribNames.at(0).c_str(), ios_base::in);

		std::string line;
		getline(file,line);
		size_t startpos = 0;
		size_t stoppos =line.find_first_of(";\n");
		++cols;
		while(startpos != std::string::npos) {
			startpos = line.find_first_of(";\n");
			stoppos = line.find_first_of(";\n", startpos);
			++cols;
		}
		items = cols;

    	file->seekg (0, is.beg);
	 	items = std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n');



    	file->seekg (0, is.beg);
    	/*
		while(getline(file,line)) {

		}
*/
		DataCube * d = new DataCube(items,t,attribs);

		return d;
	}
	return 0;
}