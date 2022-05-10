#pragma once
#include <string>
#include "InspectionCase.h"

using std::string;

struct Date
{
	Date() 
		: Month(-1)
		, Day(-1)
		, Year(-1)
	{}

	Date(int M, int D, int Y)
		: Month(M)
		, Day(D)
		, Year(Y)
	{}

	bool operator>(const Date& RHS) const
	{
		if (Year > RHS.Year)
		{
			return true;
		}

		if (Year < RHS.Year)
		{
			return false;
		}

		if (Month > RHS.Month)
		{
			return true;
		}

		if (Month < RHS.Month)
		{
			return false;
		}

		return Day > RHS.Day;
	}

	bool operator<(const Date& RHS) const
	{
		return !((*this) > RHS);
	}

	int Month; //M
	int Day; //D
	int Year; //YYYY

	const string GetDateString() const 
	{
		return std::to_string(Month) + "/" + std::to_string(Day) + "/" + std::to_string(Year);
	}
};

class ZIPGroup
{
public:

	ZIPGroup(int ZIP) 
		: ZipCode(ZIP)
		, FailedInspections(0)
	{}

	bool operator>(const ZIPGroup& RHS) const 
	{
		//More inspections
		if (FailedInspections > RHS.FailedInspections)
		{
			return true;
		}

		//Less inspections
		if (FailedInspections < RHS.FailedInspections)
		{
			return false;
		}

		//Equal inspections, just compare date
		return LastFailureDate > RHS.LastFailureDate;
	}

	//evil hacky way to trick algo::sort to order top to bottom
	//this actually returns the opposite of what "<" is expected to be
	bool operator<(const ZIPGroup& RHS) const
	{
		return (*this) > RHS;
	}

private:

	int ZipCode;
	int FailedInspections;
	Date LastFailureDate; //cache for faster evals

	/** Cache the fail case string so we don't need to construct one everytime we evaluate if a case failed inspection*/
	static string FailCase;

public:

	inline int GetZip() const { return ZipCode; }
	inline int GetFailedInspections() const { return FailedInspections; }
	inline const Date& GetLastFailDate() const { return LastFailureDate; } //use inline ref so we don't need an ugly copy for eache eval

	/** Process a case and evaluate if it failed. If it does, increment FailedInspections and update FailDate if necessary*/
	void ProcessCase(const InspectionCase& Case);
};

