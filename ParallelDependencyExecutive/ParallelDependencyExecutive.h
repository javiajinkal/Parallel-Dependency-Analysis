#ifndef PARALLELDEPENDENCYEXECUTIVE_H
#define PARALLELDEPENDENCYEXECUTIVE_H
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ParallelDependencyExecutive.h - conducts dependency analysis on all files matching one or more patterns  //
//								   in a directory tree rooted at a specified path							//
// ver 1.0																									//
// Language:    C++, Visual Studio 2015																		//
// Application: Project 3 , CSE687 - Object Oriented Design													//
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10															//
// Author:      Jinkal Arvind Javia, SUID: 425325424     													//
//              jjavia@syr.edu																				//
// Source:		Jim Fawcett, Syracuse University, CST 4-187													//
//              jfawcett@twcny.rr.com																		//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This packahge collects all of the files matching one or more patterns in a directory tree rooted at a 
* specified path. On that collection it conducts type and dependency analysis using the services of the 
* ParallelDependencyAnalysis package. It also displays the results by listing each of the files and 
* their dependencies.
*
* Public Interface:
*------------------
* class ParallelDependencyExecutive :
*		++ std::vector <std::string> getfiles(std::string path, std::vector<std::string> pattern) :
*				        -- Grabs all the files on the given path which matches the specified pattern
*		++ std::vector<std::string> patternCommandLine(std::string filepatterns) :
*						-- Parse all the file patterns from command line
*		++ bool runParallelDependencyExecutive(std::vector<std::string>, size_t) :
*						-- Runs ParallelDependencyAnalysis on the files collected from the command line
*
* Build Process:
* --------------
* Required Files: ParallelDependencyExecutive.h, ParallelDependencyExecutive.cpp, 
*                 ParallelDependencyAnalysis.h, ParallelDependencyAnalysis.cpp,
*                 TypeAnalysis.h, TypeAnalysis.cpp, Tasks.h, Tasks.cpp, AST.h,
*				  DependencyAnalysis.h, DependencyAnalysis.cpp, FileSystem.h, AST.cpp,
*                 FileSystem.cpp, TypeTable.h, TypeTable.cpp, ActionsAndRules.h, 
*                 ActionsAndRules.cpp, Parser.h, Parser.cpp, Utilities.h, Utilities.cpp,
*                 ConfigureParser.h, ConfigureParser.cpp, itokCollection.h, SemiExp.h, 
*                 SemiExp.cpp, Tokenizer.h, Tokenizer.cpp, ScopeStack.h, ScopeStack.cpp, 
*                 FileMgr.h, FileMgr.cpp, ThreadPool.h, ThreadPool.cpp, Workitem.h,
*				  DataStore.h, DataStore.cpp
*
* Build Command: devenv Project 3.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 5 April 16
* - first release
*/

#include "../ParallelDependencyAnalysis/ParallelDependencyAnalysis.h"
#include "../FileMgr/FileSystem.h"
#include "../FileMgr/FileMgr.h"
#include <string>
#include <iostream>
#include <vector>

//Excercises all the packages of Project 3 and demonstrates all requirements
class ParallelDependencyExecutive
{
private:
	std::vector <std::string> files;
public:
	std::vector <std::string> getfiles(std::string path, std::vector<std::string> pattern);
	std::vector<std::string> patternCommandLine(std::string filepatterns);
	bool runParallelDependencyExecutive(std::vector<std::string>, size_t);
};

#endif
