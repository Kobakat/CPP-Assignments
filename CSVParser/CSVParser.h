#pragma once
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "InspectionCase.h"
#include "ZIPGroup.h"

using std::string;
using std::ifstream;
using std::istringstream;

class CSVParser
{
public:

	/** Parses the CSV file pointed to at Path and stuffs parsed data into Results vector*/
	void Parse(const string& Path, std::vector<InspectionCase>& Results);
	
	/** Sorts a collection of data by ZIP group, fail date, and most recent failure*/
	void Sort(std::vector<InspectionCase>& Data, std::vector<ZIPGroup>& Results);

private: 

	bool IsInt() const { return false;}
	bool IsDouble() const { return false; }	
};

