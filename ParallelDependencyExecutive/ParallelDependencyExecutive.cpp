//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ParallelDependencyExecutive.cpp - conducts dependency analysis on all files matching one or more patterns//
//								     in a directory tree rooted at a specified path							//
// ver 1.0																									//
// Language:    C++, Visual Studio 2015																		//
// Application: Project 3 , CSE687 - Object Oriented Design													//
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10															//
// Author:      Jinkal Arvind Javia, SUID: 425325424     													//
//              jjavia@syr.edu																				//
// Source:		Jim Fawcett, Syracuse University, CST 4-187													//
//              jfawcett@twcny.rr.com																		//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ParallelDependencyExecutive.h"
#include <iterator>
#include <sstream>

//Parse all the file patterns from command line
std::vector<std::string> ParallelDependencyExecutive::patternCommandLine(std::string filepatterns) {
	std::istringstream iss(filepatterns);
	std::vector<std::string> patternVector{ std::istream_iterator<std::string>{iss},   // adding to vector
		std::istream_iterator<std::string>{} };
	return patternVector;
}

// Grabs all the files on the given path which matches the specified pattern
std::vector <std::string> ParallelDependencyExecutive::getfiles(std::string path, std::vector<std::string> pattern)
{
	std::vector <std::string> result;
	DataStore ds;
	FileMgr fm(path, ds);
	for (std::string pat : pattern)
		fm.addPattern(pat);
	fm.search();
	for (auto fs : ds)
		result.push_back(fs);
	return result;
}

//Runs ParallelDependencyAnalysis on the files collected from the command line
bool ParallelDependencyExecutive::runParallelDependencyExecutive(std::vector<std::string> files,size_t threadCount)
{
	ParallelDependencyAnalysis pda;
	std::cout << "\nREQUIREMENT: 8 | CALLS PARALLELDEPENDENCYANALYSIS=====================\n";
	pda.parallelDependencyAnalyzerPass1(files, threadCount);
	pda.parallelDependencyAnalyzerPass2(files, threadCount);
	return false;
}

#ifndef TEST_PARALLELDEPENDENCYEXECUTIVE
int main(int argc, char* argv[])
{
	ParallelDependencyExecutive pde;
	std::string path;
	std::vector <std::string> pat;
	std::vector <std::string> files;
	size_t tc = 5;
	std::cout << "\nREQUIREMENT: 10 | THIS IS START OF TEST SUITE=========================\n";
	// Processing files, named on the command line
	if (argc == 3)   //Looking for pattern from command line
	{
		path = argv[1];
		pat = pde.patternCommandLine(argv[2]);
	}
	else { // When no pattern provided, default pattern is set to .cpp and .h
		pat.push_back("*.cpp");
		pat.push_back("*.h");
		path = "./Test"; // set default path.
		std::cout << "\nPath and Patterns are set as default path and patterns as they are not specified on command line.";
	}
	std::cout << "\nInput Path on command line: " << path;
	std::cout << "\nInput Patterns on command line are: ";
	for (auto p : pat)
		std::cout << p << "  ";
	std::cout << "\n\nREQUIREMENT: 3 | ENFORCES \"SINGLE RESPONSIBILITY PRINCIPLE\"===========\n";
	std::cout << "\n======================================================================\n";
	std::cout << "\nREQUIREMENT: 9 | CALLS PARALLELDEPENDENCYEXECUTIVE====================\n";
	std::cout << "\nFiles found under directory tree rooted at a specified path are :";
	files = pde.getfiles(path, pat);
	std::cout << "\n======================================================================\n";
	std::cout << "Thread Count : " << tc;
	std::cout << "\n======================================================================\n";
	pde.runParallelDependencyExecutive(files,tc);
    std::cout << "\n=======================THIS IS END OF TEST SUITE======================\n";
	return 0;
}
#endif
