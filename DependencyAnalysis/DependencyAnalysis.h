#ifndef DEPENDENCYANALYSIS_H
#define DEPENDENCYANALYSIS_H
///////////////////////////////////////////////////////////////////////////////
// DependencyAnalysis.h - Finds files on which given file is dependent       //
// ver 1.0																     //
// Language:    C++, Visual Studio 2015                                      //
// Application: Project 3 , CSE687 - Object Oriented Design				     //
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10						     //
// Author:      Jinkal Arvind Javia, SUID: 425325424     				     //
//              jjavia@syr.edu											     //
// Source:		Jim Fawcett, Syracuse University, CST 4-187				     //
//              jfawcett@twcny.rr.com								         //
///////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* Finds, for each file in a specified file collection, all other files from the file collection on which they depend. 
* File A depends on file B, if and only if, it uses the name of any type or global function defined in file B. 
* It might do that by calling a function or method of a type or by inheriting the type. 
* Note that this intentionally does not record dependencies of a file on files outside the file set,
* e.g., language and platform libraries.
*
* Public Interface:
*------------------
* class DependencyAnalysis :
*		++ bool dependencyAnalyzer(std::string) :
*				        -- Finds the files on which the given file is dependent using the Type table
*		++ std::unordered_map<std::string, std::set<std::string>> retrieveDependencyTable() :
*						-- Returns the file dependency map
*       ++ void add(std::string a, std::string b) :
*						-- Adds a record to the file dependency map
*       ++ void showDependencyTable() :
*						-- Displays the Dependency Table
*
* Build Process:
* --------------
* Required Files: DependencyAnalysis.h, DependencyAnalysis.cpp, Tokenizer.h, Tokenizer.cpp,
*                 FileMgr.h, FileMgr.cpp, TypeTable.h, TypeTable.cpp, Parser.h, Parser.cpp, 
*                 ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.h, ConfigureParser.cpp,
*                 FileSystem.h, FileSystem.cpp, AST.h, AST.cpp, SemiExp.h, SemiExp.cpp, 
*				  itokcollection.h, ScopeStack.h, ScopeStack.cpp
*
* Build Command: devenv Project 3.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 5 April 16
* - first release
*/

#include "../FileMgr/FileMgr.h"
#include "../Tokenizer/Tokenizer.h"
#include "../Parser/ActionsAndRules.h"
#include "../TypeTable/TypeTable.h"
#include "../Parser/ConfigureParser.h"
#include "../Parser/Parser.h"
#include "../SemiExp/itokcollection.h"
#include "../SemiExp/SemiExp.h"
#include "../AST/AST.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <iostream>

class DependencyAnalysis
{
private:
	std::vector<std::string> _tokens;
	std::unordered_map<std::string, std::set<std::string>> _dependent;

public:
	bool dependencyAnalyzer(std::string); 
	//Returns the file dependency map
	std::unordered_map<std::string, std::set<std::string>> retrieveDependencyTable(){
		return _dependent;
	}
	//Adds a record to the file dependency map
	void add(std::string a, std::string b) {
		_dependent[a].insert(b);
	}
	//Displays the Dependency Table
	void showDependencyTable() {
		for (auto d : _dependent) {
			std::cout << "\n ++  " << d.first << " DEPENDS ON :\n";
			for (auto i : d.second){
					std::cout << "\t --  " << i << "\n";
			}
		}
	}
};
#endif

