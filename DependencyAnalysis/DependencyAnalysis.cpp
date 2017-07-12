///////////////////////////////////////////////////////////////////////////////
// DependencyAnalysis.cpp - Finds files on which given file is dependent     //
// ver 1.0																     //
// Language:    C++, Visual Studio 2015                                      //
// Application: Project 3 , CSE687 - Object Oriented Design				     //
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10						     //
// Author:      Jinkal Arvind Javia, SUID: 425325424     				     //
//              jjavia@syr.edu											     //
// Source:		Jim Fawcett, Syracuse University, CST 4-187				     //
//              jfawcett@twcny.rr.com								         //
///////////////////////////////////////////////////////////////////////////////

#include "DependencyAnalysis.h"
#include <fstream>

//Finds the files on which the given file is dependent using the Type table
bool DependencyAnalysis::dependencyAnalyzer(std::string file){
	std::string fileSpec = FileSystem::Path::getFullFileSpec(file);
	std::ifstream in(fileSpec);
	if (!in.good()){
		std::cout << "\n  can't open " << fileSpec << "\n\n";
		return false;
	}
	Scanner::Toker toker;
	toker.attach(&in);
	do{
		std::string tok = toker.getTok();
		if (tok == "\n")
			tok = "newline";
		_tokens.push_back(tok);
	} while (in.good());
	Repository* repo = Repository::getInstance(fileSpec);
	TypeTable finaltable = repo->retrieveTable();
	std::unordered_map<std::string, TypeTableRecord> *finalmap = finaltable.retrieveRecord();
	for (auto t : _tokens) {
		std::unordered_map<std::string, TypeTableRecord>::iterator it = finalmap->find(t);
		if (it != finalmap->end()) { // found in map
			std::vector<std::pair<std::string, std::string>> FN = it->second.getFileNs();
			for (auto iter = FN.begin(); iter != FN.end(); iter++) {
					if (fileSpec != iter->first) {
						//std::string temp = t + " | " + iter->first;
						_dependent[fileSpec].insert(iter->first);
					}
			}
		}
	}
	_tokens.clear();
	return false;
}

#ifdef TEST_DEPENDENCYANALYSIS
int main()
{
	DependencyAnalysis da;
	//da.dependencyAnalyzer("../DependencyAnalysis/B.cpp");
	std::cout << "\nPerforming Dependency Analysis on file: \n";
	std::cout << "\t\t : " << "../DependencyAnalysis/B.cpp\n";
	TypeTable table;
	TypeTableRecord record;

	std::cout << "\nDisplaying Type Table \n\n";
	record.name() = "A";
	record.type() = "class";
	record.fileName() = "A.h";
	record.nameSpace() = "Global";
	record.addNameSpaceAndFileName("A.h", "Global");

	record.name() = "B";
	record.type() = "class";
	record.fileName() = "B.h";
	record.nameSpace() = "Global";
	record.addNameSpaceAndFileName("B.h", "Global");

	table.addRecord("A", "class", "A.h", "Global");
	table.addRecord("B", "class", "B.h", "Global");
	table.showTypeTable();

	std::string fileSpec = FileSystem::Path::getFullFileSpec("../DependencyAnalysis/B.cpp");
	std::ifstream in(fileSpec);
	if (!in.good()) {
		std::cout << "\n  can't open " << fileSpec << "\n\n";
		return false;
	}
	Scanner::Toker toker;
	toker.attach(&in);
	std::vector<std::string> t_;
	std::unordered_map<std::string, std::set<std::string>> dep;
	do {
		std::string tok = toker.getTok();
		if (tok == "\n")
			tok = "newline";
		t_.push_back(tok);
	} while (in.good());
	std::unordered_map<std::string, TypeTableRecord> *finalmap = table.retrieveRecord();
	for (auto t : t_) {
		std::unordered_map<std::string, TypeTableRecord>::iterator it = finalmap->find(t);
		if (it != finalmap->end()) { // found in map
			std::vector<std::pair<std::string, std::string>> FN = it->second.getFileNs();
			for (auto iter = FN.begin(); iter != FN.end(); iter++) {
				if (fileSpec != iter->first) {
					std::string temp = t + " | " + iter->first;
					dep[fileSpec].insert(temp);
				}
			}
		}
	}
	std::cout << "\nDisplaying Dependency Table\n";
	for (auto d : dep) {
		std::cout << "\n" << d.first << " DEPENDS ON :\n";
		for (auto i : d.second) {
			std::cout << "\t" << i << "\n";
		}
	}
	return 0;
}
#endif