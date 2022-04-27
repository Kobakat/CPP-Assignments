#pragma once
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "InspectionCase.h"

using std::string;
using std::ifstream;
using std::istringstream;

class CSVParser
{
public:

	void Parse(const string& Path, std::vector<InspectionCase>& Results);

private: 

	bool IsInt() const { return false;}
	bool IsDouble() const { return false; }
};

