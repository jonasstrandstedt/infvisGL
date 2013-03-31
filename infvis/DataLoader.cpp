#include "DataLoader.h"

#include <iostream>
#include <fstream>
#include <algorithm>

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
		std::ifstream *file = new std::ifstream(filenames.at(0).c_str());
		if (file->is_open())
		{
			std::cout << "File opened" << std::endl;

			items = std::count(std::istreambuf_iterator<char>(*file), std::istreambuf_iterator<char>(), '\n') ;

			std::string line;
			file->seekg (0, file->beg);
			getline(*file,line);
			//std::cout << "line = " << line << std::endl;
			
			size_t startpos = 0;
			size_t stoppos = line.find_first_of(";");
			++cols;
			while(startpos != std::string::npos && cols != 100) {
				startpos = line.find_first_of(";", stoppos);
				stoppos = line.find_first_of(";", startpos+1);
				++cols;
			}
			t = cols - 3;
			file->close();
			delete file;

			std::cout << "Creating DataCube("<< items << "," << t << "," << attribs << ")" << std::endl;
			DataCube * d = new DataCube(items,t,attribs);

			for (int i = 0; i < attribs; ++i)
			{
				file = new std::ifstream(filenames.at(i).c_str());
				if (!file->is_open())
				{
					std::cerr << "ERROR: Could not open file: " << filenames.at(i) << std::endl;
					exit(0);
				}

				d->setAttribName(i, attribNames.at(i));
				//std::getline(*file, line);
				int item = 0;
				while(std::getline(*file, line)) {

					
	 				//std::cout << "==================="<< std::endl;
					startpos = 0;
					stoppos = line.find_first_of(";");
					int column = 0;
					while(startpos != std::string::npos) {
						std::string val = line.substr(startpos+1, stoppos-startpos-1);

						if (column > 1)
						{
							if (item > 0)
							{
	 							//std::cout << "val (string) = " << val << std::endl;
								if (val.length() > 0)
								{
									for (int n = 0; n < val.length(); ++n) {
										if (val[n] == ',')
											val[n] = '.';
									}
									float fval = std::atof(val.c_str());
	 								 							//std::cout << "val (float)  = " << fval << std::endl;
									d->setItem(item-1, column -2, i, fval);
								}
							}else if(i == 0){
								d->setTimeName(column-2, val);
							}
						} else if(column == 0 && item > 0) {
							d->setEntryName(item-1, val);
						}


						startpos = line.find_first_of(";", stoppos);
						stoppos = line.find_first_of(";", startpos+1);
						++column;
					}
					
					
					++item;

				}
				file->close();
				delete file;
			}
			return d;
		}


	}
	return 0;
}