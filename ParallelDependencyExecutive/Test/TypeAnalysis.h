#ifndef TYPEANALYSIS_H
#define TYPEANALYSIS_H
///////////////////////////////////////////////////////////////////////////////
// TypeAnalysis.h - Performs type analysis                                   //
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
* This package finds all the types defined in each of a collection of C++ source files. 
* It does this by building rules to detect global function and type definitions: classes, 
* structs, enums, typedefs and aliases, and capture their fully qualified names and files 
* where they are defined.
*
* Public Interface:
*------------------
* class typeAnalysis :
*		++ bool TypeAnalysis(std::string) :
*				        -- Performs Type Analysis on the provided file
*
* Build Process:
* --------------
* Required Files: TypeAnalysis.h, TypeAnalysis.cpp, TypeTable.h, TypeTable.cpp
*                 ActionsAndRules.h, ActionsAndRules.cpp, Parser.h, Parser.cpp,
*                 ConfigureParser.h, ConfigureParser.cpp, FileSystem.h, FileSystem.cpp,
*                 Utilities.h, Utilities.cpp,SemiExp.h, SemiExp.cpp, itokcollection.h,
*                 Tokenizer.h, Tokenizer.cpp, ScopeStack.h, ScopeStack.cpp, AST.h, AST.cpp
*
* Build Command: devenv Project 3.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 5 April 16
* - first release
*/

#include "../TypeTable/TypeTable.h"
#include "../Utilities/Utilities.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../FileMgr/FileSystem.h"
#include "../Parser/Parser.h"
#include <string>
#include <iostream>

class typeAnalysis {
public:
	bool TypeAnalysis(std::string);
};
#endif
