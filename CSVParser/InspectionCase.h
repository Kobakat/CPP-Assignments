#pragma once
#include <string>
#include <vector>

using std::string;

class InspectionCase
{

public:
    InspectionCase(std::vector<string>& ParamPackage);

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

public:

    inline int GetZip() const { return Zip; }
    inline const string& GetResults() const { return Results; }
    inline const string& GetInspectionDate() const { return InspectionDate; }
};