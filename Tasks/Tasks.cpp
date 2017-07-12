///////////////////////////////////////////////////////////////////////////////
// Tasks.cpp - executes a specified callable object on a ThreadPool thread   //
// ver 1.0																     //
// Language:    C++, Visual Studio 2015                                      //
// Application: Project 3 , CSE687 - Object Oriented Design				     //
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10						     //
// Author:      Jinkal Arvind Javia, SUID: 425325424     				     //
//              jjavia@syr.edu											     //
// Source:		Jim Fawcett, Syracuse University, CST 4-187				     //
//              jfawcett@twcny.rr.com								         //
///////////////////////////////////////////////////////////////////////////////

#include "Tasks.h"

#ifdef TEST_TASKS
int main()
{
	std::cout << "\n--------Testing Tasks Package-------\n";
	size_t threadCount = 2;
	Task<bool, std::string> t;
	t.start(threadCount);

	// define 1st work item
	std::string s1 = "X";
	std::function<bool(std::string)> w1 = [&](std::string file) {
		return false;
	};
	workItem<bool, std::string> *wi1 = new workItem<bool, std::string>(&w1, &s1);
	t.doWork(wi1);
	t.result();

	// define 2nd work item
	std::string s2 = "Y";
	std::function<bool(std::string)> w2 = [&](std::string file) {
		return false;
	};
	workItem<bool, std::string> *wi2 = new workItem<bool, std::string>(&w2, &s2);
	t.doWork(wi2);
	t.result();

	t.doWork(nullptr);
	t.wait();
	return 0;
}
#endif