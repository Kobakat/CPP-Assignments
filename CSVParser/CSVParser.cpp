#include "CSVParser.h"
#include <algorithm>

void CSVParser::Parse(const string& Path, std::vector<InspectionCase>& Results)
{
	string Line;
	ifstream File = ifstream(Path);

	//skip first line
	getline(File, Line);

	//Push all the data from the CSV into the results vector
	while (getline(File, Line))
	{	
		getline(File, Line);
		std::vector<string> CasePackage;
		bool bEvaluatingPair = false;
		string Variable;

		for(auto Iter = Line.begin(); Iter != Line.end(); Iter++)
		{		
			bool bReachedLastCharacter = *Iter == ',';
			
			if (bEvaluatingPair == false && *Iter == '"')
			{
				bEvaluatingPair = true;
			}

			else if (bEvaluatingPair && *Iter == '"')
			{
				bEvaluatingPair = false;
				bReachedLastCharacter = true;

				if (Iter != Line.end() - 1)
				{
					++Iter;
				}		
			}

			if (bEvaluatingPair || !bReachedLastCharacter)
			{
				Variable += *Iter;
			}

			else
			{
				CasePackage.push_back(Variable);
				Variable.clear();
			}				
		}

		Results.push_back(std::move(InspectionCase(CasePackage)));
	}
}

void CSVParser::Sort(std::vector<InspectionCase>& Data, std::vector<ZIPGroup>& Results)
{
	//Hash map for zip code lookup
	std::unordered_map<int, ZIPGroup> ZipGroups;

	//Get number of failures for a given zip code in this map
	for (InspectionCase& Case : Data)
	{
		//New zip code
		if (!ZipGroups.count(Case.GetZip()))
		{
			ZipGroups.insert(std::pair<int, ZIPGroup>(Case.GetZip(), ZIPGroup(Case.GetZip())));
		}

		//We process the case even if its new to update the most recent date with this group
		ZipGroups.at(Case.GetZip()).ProcessCase(Case);
	}

	//Copy zip group map into vector
	for (auto Iter = ZipGroups.begin(); Iter != ZipGroups.end(); ++Iter)
	{
		Results.push_back(Iter->second);
	}

	//Now sort
	std::sort(Results.begin(), Results.end());
}


int main()
{
	CSVParser Parser;
	std::vector<InspectionCase> Cases;
	std::vector<ZIPGroup> SortedGroups;

	Parser.Parse("Food_Inspections.csv", Cases);
	Parser.Sort(Cases, SortedGroups);

	for (auto Iter = SortedGroups.begin(); Iter != SortedGroups.end(); ++Iter)
	{
		std::cout << "ZIP: " << Iter->GetZip() << " | Fails: " << Iter->GetFailedInspections() << " | Most Recent Fail Date: " << Iter->GetLastFailDate().GetDateString() << std::endl;
	}

	return 0;
}