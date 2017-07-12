#ifndef TYPETABLE_H
#define TYPETABLE_H
///////////////////////////////////////////////////////////////////////////////
// TypeTable.h - Builds, stores, and provides access to type table           //
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
* Package provides facilities to build a Type Table. Each of the record of the table
* has Name, Type, file name where it occurs and its namespace. The package provides
* class TypeTableRecord which provides facilities to build a record and to retrieve
* required name, type, file name and namespace. The package also provides a class 
* TypeTable which enables us to add record to the table, retrieve type table and to
* display the type table, and access it. 
*
* Public Interface:
*------------------
* class TypeTableRecord :
*		++ Name& name() :
*						-- returns value of _name
*		++ Type& type() :
*						-- returns value of _type
*       ++ FileName& fileName() :
*						-- returns value of _whereDefined
*		++ Namespace& nameSpace() :
*						-- returns value of _nameSpace
*		++ void setname(Name n) :
*				        -- sets value of _name
*		++ void settype(Type t) :
*						-- sets value of _type
*		++ void setfilename(FileName fn) :
*						-- sets value of _whereDefined
*		++ void setnamespace(Namespace ns) :
*						-- sets value of _nameSpace
*		++ void addNameSpaceAndFileName(const FileName fn, const Namespace ns) :
*						-- pushes back FileName, Namespace pair onto vector _namespacesAndFileName
*		++ size_t numberOfNameSpacesAndFileNames() :
*						-- returns size of vector _namespacesAndFileName
*		++ std::pair<FileName,Namespace> operator[](size_t n) :
*						-- returns the index of entry in vector _namespacesAndFileName
*
* class TypeTable :
*		++ void addRecord(std::string name, std::string type, std::string fileName, std::string nameSpace) :
*						-- Adds record to type table
*		++ void showTypeTable() :
*						-- Displays the type table
*		++ std::unordered_map<std::string, TypeTableRecord>* retrieveRecord() :
*						-- Returns the type table
*		++ void insert(TypeTable* tp) :
*						-- If entry is found inserts to its second field, otherwise create a new entry in map
*
* Build Process:
* --------------
* Required Files: TypeTable.h, TypeTable.cpp
*
* Build Command: devenv Project 3.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 5 April 16
* - first release
*/

#include <string> 
#include <vector>
#include <unordered_map>
#include <iostream>
#include <iomanip>

class TypeTableRecord {
public:
	using Namespace = std::string;
	using Name = std::string;
	using Type = std::string;
	using FileName = std::string;
	using iterator = std::vector<std::pair<FileName, Namespace>>::iterator;

	Name& name() { return _name; }
	void setname(Name n) { _name = n; }
	Type& type() { return _type; }
	void settype(Type t) { _type = t; }
	FileName& fileName() { return _whereDefined; }
	void setfilename(FileName fn) { _whereDefined = fn; }
	Namespace& nameSpace() { return _nameSpace; }
	void setnamespace(Namespace ns) { _nameSpace = ns; }
	void addNameSpaceAndFileName(const FileName fn, const Namespace ns){
			_namespacesAndFileName.push_back(std::make_pair(fn, ns));
	}
	size_t numberOfNameSpacesAndFileNames() { return _namespacesAndFileName.size(); }
	std::vector<std::pair<FileName, Namespace>> getFileNs(){
		return _namespacesAndFileName;
	}
	iterator begin() { return _namespacesAndFileName.begin(); }
	iterator end() { return _namespacesAndFileName.end(); }
	std::pair<FileName,Namespace> operator[](size_t n) const{
		if (n < 0 || n >= _namespacesAndFileName.size())
			throw(std::exception("index out of range"));
		return _namespacesAndFileName[n];
	}
private:
	Name _name;
	Type _type;
	FileName _whereDefined;
	Namespace _nameSpace;
	std::vector<std::pair<FileName, Namespace>> _namespacesAndFileName;
};

//Table needs to add records, store them, and provide access.
class TypeTable
{
public:
	using iterator = typename std::unordered_map<std::string, TypeTableRecord>::iterator;
	TypeTable() {
		_records = new std::unordered_map<std::string, TypeTableRecord>();
	}
	//Adds record to type table
	void addRecord(std::string name, std::string type, std::string fileName, std::string nameSpace) { 
		if (_records->find(name) == _records->end()) {
			TypeTableRecord record;
			record.settype(type);
			record.addNameSpaceAndFileName(fileName, nameSpace);
			(*_records)[name] = record;
		}
		else {
			TypeTableRecord overlap = (*_records)[name];
			overlap.addNameSpaceAndFileName(fileName, nameSpace);
			(*_records)[name] = overlap;
		}
	}
	iterator begin() { return _records->begin(); }
	iterator end() { return _records->end(); }
	void showTypeTable() {                                      //Displays Type Table
		for (auto record : *_records) {
			bool check = false;
			std::cout << std::setw(20) << record.first  << " | ";
			TypeTableRecord showrecord = record.second;
			std::string t = showrecord.type();
			std::cout << std::setw(15) << t << " | ";
			std::vector<std::pair<std::string, std::string>> FN = record.second.getFileNs();
			for (auto it = FN.begin(); it != FN.end(); it++){
				if (check) { std::cout << "\n" << "\t\t\t\t\t "; }
				check = true;
				std::string ns = it->second;
				std::cout << std::setw(20) << ns << " | ";
				std::string fn = it->first;
				std::cout << std::setw(30) << fn ;
			}
			std::cout << "\n";
		}
	}
	std::unordered_map<std::string, TypeTableRecord>* retrieveRecord() //returns the Type Table
	{
		return _records;
	}
	void insert(TypeTable* tp) //If entry is found adds to its second field
	{
		std::unordered_map<std::string, TypeTableRecord>::iterator it;
		for (it = tp->retrieveRecord()->begin(); it != tp->retrieveRecord()->end(); it++) {
			(*_records)[it->first] = it->second;
		}
	}
private:
	std::unordered_map<std::string, TypeTableRecord> *_records;
};

#endif