#ifndef PARALLELDEPENDENCYANALYSIS_H
#define PARALLELDEPENDENCYANALYSIS_H
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// ParallelDependencyAnalysis.h-Evaluates dependency graph for all packages in specified file collection //
// ver 1.0																								 //
// Language:    C++, Visual Studio 2015																	 //
// Application: Project 3 , CSE687 - Object Oriented Design												 //
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10														 //
// Author:      Jinkal Arvind Javia, SUID: 425325424     												 //
//              jjavia@syr.edu																			 //
// Source:		Jim Fawcett, Syracuse University, CST 4-187												 //
//              jfawcett@twcny.rr.com																	 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package evaluates the dependency graph for all the packages in a specified 
* file collection. For each file the analyses run asynchronously, using the facilities 
* provided by the Task class. The package performs two passes over the file collection. 
*
* In the first pass it finds, asynchronously, all the types defined in the collection using 
* services of the TypeAnalysis package. 
*
* In the second pass it finds, asynchronously, all the dependencies between files using 
* the DependencyAnalysis package.
*
* Public Interface:
*------------------
* class ParallelDependencyAnalysis :
*		++ bool parallelDependencyAnalyzerPass1(std::vector<std::string>, size_t tc) :
*				        -- Performs Pass 1 asynchronously using services of Tasks and TypeAnalysis package
*		++ bool parallelDependencyAnalyzerPass2(std::vector<std::string>, size_t tc) :
*						-- Performs Pass 2 asynchronously using services of Tasks and DependencyAnalysis package
*
* Build Process:
* --------------
* Required Files: ParallelDependencyAnalysis.h, ParallelDependencyAnalysis.cpp,
*                 TypeAnalysis.h, TypeAnalysis.cpp, Tasks.h, Tasks.cpp, AST.h,
*				  DependencyAnalysis.h, DependencyAnalysis.cpp, FileSystem.h, AST.cpp,
*                 FileSystem.cpp, TypeTable.h, TypeTable.cpp, ActionsAndRules.h, 
*                 ActionsAndRules.cpp, Parser.h, Parser.cpp, Utilities.h, Utilities.cpp,
*                 ConfigureParser.h, ConfigureParser.cpp, itokCollection.h, SemiExp.h, 
*                 SemiExp.cpp, Tokenizer.h, Tokenizer.cpp, ScopeStack.h, ScopeStack.cpp, 
*                 FileMgr.h, FileMgr.cpp, ThreadPool.h, ThreadPool.cpp, Workitem.h
*
* Build Command: devenv Project 3.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 5 April 16
* - first release
*/

#include "../DependencyAnalysis/DependencyAnalysis.h"
#include "../TypeAnalysis/TypeAnalysis.h"
#include "../FileMgr/FileSystem.h"
#include "../Tasks/Tasks.h"
#include <string>
#include <vector>

class ParallelDependencyAnalysis
{
public:
	bool parallelDependencyAnalyzerPass1(std::vector<std::string>, size_t tc);
	bool parallelDependencyAnalyzerPass2(std::vector<std::string>, size_t tc);
};

#endif
