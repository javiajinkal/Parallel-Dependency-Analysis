///////////////////////////////////////////////////////////////////////////////
// TypeTable.cpp - Builds, stores, and provides access to type table         //
// ver 1.0																     //
// Language:    C++, Visual Studio 2015                                      //
// Application: Project 3 , CSE687 - Object Oriented Design				     //
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10						     //
// Author:      Jinkal Arvind Javia, SUID: 425325424     				     //
//              jjavia@syr.edu											     //
// Source:		Jim Fawcett, Syracuse University, CST 4-187				     //
//              jfawcett@twcny.rr.com								         //
///////////////////////////////////////////////////////////////////////////////

#include "TypeTable.h"
#include <iostream>
#include <iomanip>

#ifdef TEST_TYPETABLE
int main()
{
	TypeTable table;
	TypeTableRecord record;

	std::cout << "\nDisplaying Type Table \n\n";
	record.name() = "X";
	record.type() = "class";
	record.fileName() = "X.h";
	record.nameSpace() = "typeAnalysis";
	record.addNameSpaceAndFileName("X.h","pqr");
	
	record.name() = "Y";
	record.type() = "struct";
	record.fileName() = "Y.cpp";
	record.nameSpace() = "abc";
	record.addNameSpaceAndFileName("Y.cpp", "abc");

	table.addRecord("X", "class", "X.h", "pqr");
	table.addRecord("Y", "struct", "Y.cpp", "abc");

	table.showTypeTable();
	std::cout << "\n\n";
	return 0;
}
#endif