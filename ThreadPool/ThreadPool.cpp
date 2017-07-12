///////////////////////////////////////////////////////////////////////////////
// ThreadPool.cpp - child thread processes enqueued work items               //
// ver 1.0																     //
// Language:    C++, Visual Studio 2015                                      //
// Application: Project 3 , CSE687 - Object Oriented Design				     //
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10						     //
// Author:      Jinkal Arvind Javia, SUID: 425325424     				     //
//              jjavia@syr.edu											     //
// Source:		Jim Fawcett, Syracuse University, CST 4-187				     //
//              jfawcett@twcny.rr.com								         //
///////////////////////////////////////////////////////////////////////////////

#include "../Utilities/Utilities.h"
#include "ThreadPool.h"
#include <string>
#include <iostream>
#include <conio.h>

//----< demonstrate ThreadPool class >----------------------------

using Util = Utilities::StringHelper;

#ifdef TEST_THREADPOOL
int main()
{
	std::cout << "\n--------Testing ThreadPool Package-------\n";
	Util::Title("Enqueued Work Items");

	std::cout << "\n  main thread id = " << std::this_thread::get_id();

	ThreadPool<bool, std::string> processor;
	processor.start(2);
	
	// define 1st work item
	std::string s1 = "X";
	std::function<bool(std::string)> w1 = [&](std::string file) {
		return false;
	};
	workItem<bool, std::string> *wi1 = new workItem<bool, std::string>(&w1, &s1);
	processor.doWork(wi1);
	std::cout << "\n Processor Result: " << processor.result();

	// define 2nd work item
	std::string s2 = "Y";
	std::function<bool(std::string)> w2 = [&](std::string file) {
		return false;
	};
	workItem<bool, std::string> *wi2 = new workItem<bool, std::string>(&w2, &s2);
	processor.doWork(wi2);

	// the following calls to result() block until results are enqueued
	std::cout << "\n  Processor Result: " << processor.result();
	processor.doWork(nullptr);

	// wait for child thread to complete
	processor.wait();
	std::cout << "\n\n";
}

#endif