#ifndef WORKITEM_H
#define WORKITEM_H
///////////////////////////////////////////////////////////////////////////////
// Workitem.h - custom class which contains work items                       //
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
* Package provides custom class: workItem, for containing work items
*
* Public Interface:
*------------------
* template <typename returnType, typename Param1>
* class workItem :
*		++ workItem(std::function<returnType(Param1)> *myfun, Param1 *param1) :
*				        -- Initializes _param1, _myfunction
*		++ returnType operator() () :
*						-- returns _myfunction(_param1)
*
* Build Process:
* --------------
* Required Files: Workitem.h, ThreadPool.h, ThreadPool.cpp
*
* Build Command: devenv Project 3.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 5 April 16
* - first release
*/

#include "ThreadPool.h"
#include<vector>
#include<iostream>
#include<functional>
#include <thread>

template <typename returnType, typename Param1>
class workItem {
public:
	workItem(std::function<returnType(Param1)> *myfun, Param1 *param1) {
		_param1 = *param1;
		_myfunction = *myfun;
	}
	returnType operator() () {
		std::cout << std::endl << "ThreadID: " << std::this_thread::get_id() << ", Processing file : " << _param1;
		return _myfunction(_param1);
	}
	~workItem() {}
private:
	std::function<returnType(Param1)> _myfunction;
	Param1 _param1;
};

#endif

