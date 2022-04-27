#include "CSVParser.h"

void CSVParser::Parse(const string& Path, std::vector<InspectionCase>& Results)
{
	string Line;
	ifstream File = ifstream(Path);

	//skip first line
	getline(File, Line);

	while (getline(File, Line))
	{	
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

		Results.push_back(InspectionCase(CasePackage));
	}
}


int main()
{
	CSVParser Parser;
	std::vector<InspectionCase> Cases;

	Parser.Parse("Food_Inspections.csv", Cases);

	return 0;
}