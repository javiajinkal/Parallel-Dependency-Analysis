#ifndef THREADPOOL_H
#define THREADPOOL_H
///////////////////////////////////////////////////////////////////////////////
// ThreadPool.h - child thread processes enqueued work items                 //
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
* Package provides facilities to enqueue work items and execute them asynchronously.
* Each work item will be a callable object that is supplied by application code.
*
* Public Interface:
*------------------
* template<typename Result, typename Parameter>
* class ThreadPool :
*		++ void start(size_t) :
*				        -- Pstart child thread that dequeus work items
*		++ void doWork(workItem<Result,Parameter>* pWi) :
*						-- enqueue work item
*		++ Result result() :
*						-- retrieve results with blocking call
*		++ void wait() :
*						-- wait for child thread to terminate
*
* Build Process:
* --------------
* Required Files: ThreadPool.h, ThreadPool.cpp, Workitem.h, Cpp11-BlockingQueue.h,
*                 Cpp11-BlockingQueue.cpp, Utilities.h, Utilities.cpp
*
* Build Command: devenv Project 3.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 5 April 16
* - first release
*/

#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "Workitem.h"
#include <thread>
#include <functional>

///////////////////////////////////////////////////////////////////////
// class to process work items
// - each work item is processed sequentially on a single child thread

template<typename Result, typename Parameter>
class ThreadPool
{
public:
	void start(size_t);
	void doWork(workItem<Result,Parameter>* pWi);
	Result result();
	void wait();
	~ThreadPool();
private:
	std::thread* _pThread;
	std::vector<std::thread*> _pThreads;
	BlockingQueue<workItem<Result, Parameter>*> _workItemQueue;
	BlockingQueue<Result> _resultsQueue;
};
//----< wait for child thread to terminate >---------------------------

template<typename Result, typename Parameter>
void ThreadPool<Result, Parameter>::wait()
{
	std::cout << "\n";
	//std::cout << std::endl << "\n  Number of Threads Waiting : " << _pThreads.size() << "\n";
	try {
		for (auto thread : _pThreads)
			thread->join();
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}
}
//----< enqueue work item >--------------------------------------------

template<typename Result, typename Parameter>
void ThreadPool<Result, Parameter>::doWork(workItem<Result,Parameter>* pWi)
{
	_workItemQueue.enQ(pWi);
}
//----< retrieve results with blocking call >--------------------------

template<typename Result, typename Parameter>
Result ThreadPool<Result,Parameter>::result()
{
	return _resultsQueue.deQ();
}
//----< start child thread that dequeus work items >-------------------

template<typename Result, typename Parameter>
void ThreadPool<Result, Parameter>::start(size_t size) {
	size_t count = 0;
	while (count < size){
		std::function<void()> threadProc =
		[&]() {
		while (true)
		{
			workItem<Result,Parameter>* pWi = _workItemQueue.deQ();
			if (pWi == nullptr)
			{
				std::cout << "\n" << "ThreadID: " << std::this_thread::get_id() << ", shutting down";
				_workItemQueue.enQ(nullptr);
				return;
			}
			Result result = (*pWi)();
			_resultsQueue.enQ(result);
		}
	};
	_pThread = new std::thread(threadProc);
	_pThreads.push_back(_pThread);
	count++;
   }
}
//----< clean up heap >------------------------------------------------

template<typename Result, typename Parameter>
ThreadPool<Result, Parameter>::~ThreadPool()
{
	delete _pThread;
}

#endif