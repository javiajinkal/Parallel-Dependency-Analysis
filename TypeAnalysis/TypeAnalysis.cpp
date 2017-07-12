///////////////////////////////////////////////////////////////////////////////
// TypeAnalysis.cpp - Performs type analysis                                 //
// ver 1.0																     //
// Language:    C++, Visual Studio 2015                                      //
// Application: Project 3 , CSE687 - Object Oriented Design				     //
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10						     //
// Author:      Jinkal Arvind Javia, SUID: 425325424     				     //
//              jjavia@syr.edu											     //
// Source:		Jim Fawcett, Syracuse University, CST 4-187				     //
//              jfawcett@twcny.rr.com								         //
///////////////////////////////////////////////////////////////////////////////

#include "TypeAnalysis.h"
#include <iostream>
#include <string>

#define Util StringHelper
using namespace Utilities;

//Performs Type Analysis on the provided file
bool typeAnalysis::TypeAnalysis(std::string file)
{
	std::string fileSpec = FileSystem::Path::getFullFileSpec(file);
	ConfigParseToConsole configure;
	Parser* pParser = configure.Build();
	try {
		if (pParser) {
			if (!configure.Attach(fileSpec))
				std::cout << "\n  could not open file " << fileSpec << std::endl;
		}
		else {
			std::cout << "\n\n  Parser not built\n\n";
			return false;
		}
		while (pParser->next())            //Using the built parser
			pParser->parse();
		Repository* repo = Repository::getInstance(fileSpec);
		std::vector<element> vectorFunction;
		repo->FinalTypeTable();
		return true;
	}
	catch (std::exception& ex) {
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}
	return true;
}

#ifdef TEST_TYPEANALYSIS
int main()
{
	bool check;
	std::string fileSpec = "../TypeAnalysis/test.txt";
	std::cout << "\nPerforming Type Analysis on file :\n";
	std::cout << "\t : " << fileSpec <<"\n";
	typeAnalysis ma;
	check = ma.TypeAnalysis(fileSpec);
	if (check == false)
	{
		std::cout << "Type Analysis of input file is unsuccessful\n";
	}
	else
	{
		std::cout << "\nDisplaying Type Analysis result: \n\n";
		Repository::retrieveTable().showTypeTable();
		std::cout << "\nType Analysis of input file completed\n";
	}
	return 0;
}
#endif