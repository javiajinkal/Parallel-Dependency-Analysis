//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ParallelDependencyAnalysis.cpp -Evaluates dependency graph for all packages in specified file collection //
// ver 1.0																									//
// Language:    C++, Visual Studio 2015																		//
// Application: Project 3 , CSE687 - Object Oriented Design													//
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10															//
// Author:      Jinkal Arvind Javia, SUID: 425325424     													//
//              jjavia@syr.edu																				//
// Source:		Jim Fawcett, Syracuse University, CST 4-187													//
//              jfawcett@twcny.rr.com																		//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ParallelDependencyAnalysis.h"

//Performs Pass 1 asynchronously using services of Tasks and TypeAnalysis package
bool ParallelDependencyAnalysis::parallelDependencyAnalyzerPass1(std::vector<std::string> files, size_t threadCount)
{
	typeAnalysis ta;
	Task<bool, std::string> t;
	t.start(threadCount);
	std::cout << "\n================================PASS 1================================\n";
	std::cout << "\nREQUIREMENT: 4, 5, 6 | PROVIDES THREADPOOL, TASKS & TYPEANALYSIS======\n";
	std::cout << "\n======================================================================\n";
	try {
		if (files.size() > 0) {
			for (size_t i = 0; i < files.size(); i++) {
				std::function<bool(std::string)> w = [&](std::string file) { return ta.TypeAnalysis(files[i]); };
				workItem<bool, std::string> *wi1 = new workItem<bool, std::string>(&w, &files[i]);
				t.doWork(wi1);
				t.result();
			}
		}
	}
	catch (std::exception ex) {
		std::cout << ex.what();
		return false;
	}
	t.doWork(nullptr);
	t.wait();
	Repository::FinalTypeTable();
	std::cout << "\n\n===========================FINAL TYPE TABLE===========================\n\n";
	std::cout << "TYPE TABLE STRUCTURE : " << "	NAME  |	TYPE  | NAMESPACE |  FILE NAME  \n";
	std::cout << "======================================================================\n";
	Repository::retrieveTable().showTypeTable();
	return true;
}

//Performs Pass 2 asynchronously using services of Tasks and DependencyAnalysis package
bool ParallelDependencyAnalysis::parallelDependencyAnalyzerPass2(std::vector<std::string> files, size_t threadCount)
{
	DependencyAnalysis da;
	Task<bool, std::string> t2;
	t2.start(threadCount);
	std::cout << "\n================================PASS 2================================\n";
	std::cout << "\nREQUIREMENT: 4, 5, 7 | PROVIDES THREADPOOL, TASKS & DEPENDENCYANALYSIS\n";
	std::cout << "\n======================================================================\n";
	try {
		if (files.size() > 0) {
			for (size_t i = 0; i < files.size(); i++) {
				std::function<bool(std::string)> w = [&](std::string file) { return da.dependencyAnalyzer(files[i]); };
				workItem<bool, std::string> *wi1 = new workItem<bool, std::string>(&w, &files[i]);
				t2.doWork(wi1);
				t2.result();
			}
		}
	}
	catch (std::exception ex) {
		std::cout << ex.what();
		return false;
	}
	t2.doWork(nullptr);
	t2.wait();
	std::cout << "\n\n========================FINAL DEPENDENCY TABLE========================\n";
	da.showDependencyTable();
	return true;
}

#ifdef TEST_PARALLELDEPENDENCYANALYSIS
int main()
{
	std::cout << "\nTESTING PARALLEL DEPENDENCY ANALYSIS PACKAGE\n";
	std::string path;
	path = "../ParallelDependencyAnalysis/Test/";
	std::vector<std::string> currfiles;
	currfiles = FileSystem::Directory::getFiles(path);
	for (size_t i = 0; i < currfiles.size(); ++i)
		std::cout << "\n    " << currfiles[i].c_str();
	std::cout << "\n";
	ParallelDependencyAnalysis pda;
	size_t tc = 5;
	bool c1, c2;
	c1 = pda.parallelDependencyAnalyzerPass1(currfiles,tc);
	c2 = pda.parallelDependencyAnalyzerPass2(currfiles,tc);
	return 0;
}
#endif