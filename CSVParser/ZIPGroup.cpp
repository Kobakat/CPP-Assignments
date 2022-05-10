#include "ZIPGroup.h"

string ZIPGroup::FailCase = "Fail";


void ZIPGroup::ProcessCase(const InspectionCase& Case)
{
	if (Case.GetResults() == FailCase)
	{
		FailedInspections++;

		//We assume the date to follow the form M/D/YYYY

		//Missing date entry, skip.
		if (Case.GetResults()[0] == '#')
		{
			return;
		}

		//This is proof a date type class would be far superior than a string
		//We could reasonably break this ugly parsing into a more intelligent function
		string Buffer;
		auto Iter = --Case.GetInspectionDate().end();

		//First check the year
		//Store each char into a reverse buffer
		while (*Iter != '/')
		{
			Buffer += *Iter;
			Iter--;
		}

		//flip the conver
		std::reverse(Buffer.begin(), Buffer.end());

		//convert into an int
		int Year = std::stoi(Buffer);

		//Happened in a previous year
		if (Year < LastFailureDate.Year)
		{
			return;
		}

		Buffer.clear();

		//Now parse the month from the front
		Iter = Case.GetInspectionDate().begin();
		while (*Iter != '/')
		{
			Buffer += *Iter;
			Iter++;
		}

		int Month = std::stoi(Buffer);

		//Happened in a previous month
		if (Month < LastFailureDate.Month)
		{
			return;
		}

		Iter++;
		Buffer.clear();

		while (*Iter != '/')
		{
			Buffer += *Iter;
			Iter++;
		}

		int Day = std::stoi(Buffer);
		//Happened in previous day
		if (Day < LastFailureDate.Day)
		{
			return;
		}

		//This is the most recent failure on record
		LastFailureDate = Date(Month, Day, Year);
	}
}
