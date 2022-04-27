#pragma once
#include <string>
#include <vector>

using std::string;

class InspectionCase
{

public:
    InspectionCase(std::vector<string>& Package)
    {
		if (Package.size() < 17)
		{
			for (int i = Package.size() - 1; i < 17; i++)
			{
				Package.push_back("");
			}
		}

		if (Package[0] != "")
		{
			InspectionID = std::stoi(Package[0]);
		}

		DBAName = Package[1];
		AKAName = Package[2];

		if (Package[3] != "")
		{
			License = std::stoi(Package[3]);
		}

		FacilityType = Package[4];
		Risk = Package[5];
		Address = Package[6];
		City = Package[7];
		State = Package[8];

		if (Package[9] != "")
		{
			Zip = std::stoi(Package[9]);
		}

		InspectionDate = Package[10];
		InspectionType = Package[11];
		Results = Package[12];
		Violation = Package[13];

		if (Package[14] != "")
		{
			Latitude = std::stod(Package[14]);
		}

		if (Package[15] != "")
		{
			Longitude = std::stod(Package[15]);
		}

		Location = Package[16];
    }

private:
    int InspectionID;
    int License;
    int Zip;

    double Latitude;
    double Longitude;

    string Location;
    string DBAName;
    string AKAName;
    string FacilityType;
    string Risk;
    string Address;
    string City;
    string State;
    string InspectionDate;
    string InspectionType;
    string Results;
    string Violation;    
};