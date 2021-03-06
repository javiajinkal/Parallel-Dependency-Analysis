///////////////////////////////////////////////////////////////////////////////
// ScopeStack.cpp - implements template stack holding specified element type //
// ver 1.0																     //
// Language:    C++, Visual Studio 2015                                      //
// Application: Project 3 , CSE687 - Object Oriented Design				     //
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10						     //
// Author:      Jinkal Arvind Javia, SUID: 425325424     				     //
//              jjavia@syr.edu											     //
// Source:		Jim Fawcett, Syracuse University, CST 4-187				     //
//              jfawcett@twcny.rr.com								         //
///////////////////////////////////////////////////////////////////////////////

//----< test stub >------------------------------------------------------

#ifdef TEST_SCOPESTACK

#include "ScopeStack.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

// Structure for a node of AST
struct element
{
  std::string type;
  std::string name;
  size_t startLineCount = 0;
  size_t endLineCount = 0;
  std::string show()
  {
    std::ostringstream temp;
    temp << "(";
    temp << type;
    temp << ", ";
    temp << name;
    temp << ", ";
    temp << startLineCount;
    temp << ", ";
    temp << endLineCount;
    temp << ")"; 
    return temp.str();
  }
  std::vector<element*> _children;
};

// Performs walk on the tree
void TreeWalk(element* pItem)
{
  static size_t indentLevel = 0;
  std::cout << "\n  " << std::string(2*indentLevel, ' ') << pItem->show();
  auto iter = pItem->_children.begin();
  ++indentLevel;
  while (iter != pItem->_children.end())
  {
    TreeWalk(*iter);
    ++iter;
  }
  --indentLevel;
}

int main()
{
  std::cout << "\n  Testing Scope Stack\n=====================\n";
  std::cout << "\n  pushing items onto ScopeStack instance after adding startLineCount\n --------------------------------------------------------------------";
  ScopeStack<element*> testStack;
  element* pItem = new element;
  pItem->type = "function";
  pItem->name = "fun1";
  pItem->startLineCount = 33;
  testStack.push(pItem);
  pItem = new element;
  pItem->type = "if";
  pItem->name = "";
  pItem->startLineCount = 38;
  (testStack.top()->_children).push_back(pItem);
  testStack.push(pItem);
  pItem = new element;
  pItem->type = "for";
  pItem->name = "";
  pItem->startLineCount = 40;
  (testStack.top()->_children).push_back(pItem);
  testStack.push(pItem);
  showStack<element>(testStack, false);
  std::cout << std::endl;
  std::cout << "\n  Popping two items off ScopeStack after adding endLineCount\n ------------------------------------------------------------";
  testStack.top()->endLineCount = 50;
  std::cout << "\n  " << testStack.pop()->show();
  testStack.top()->endLineCount = 55;
  std::cout << "\n  " << testStack.pop()->show() << "\n";
  std::cout << "\n  Pushing another item onto ScopeStack after adding startLineElement\n --------------------------------------------------------------------";
  pItem = new element;
  pItem->type = "while";
  pItem->name = "";
  pItem->startLineCount = 60;
  (testStack.top()->_children).push_back(pItem);
  testStack.push(pItem);
  std::cout << "\n  Stack now contains:\n ---------------------";
  showStack<element>(testStack, false);
  std::cout << "\n";
  std::cout << "\n  Popping last elements off stack after adding endLineCount\n -----------------------------------------------------------";
  testStack.top()->endLineCount = 70;
  element* pTop = testStack.pop();
  std::cout << "\n  " << pTop->show();
  testStack.top()->endLineCount = 75;
  pTop = testStack.pop();
  std::cout << "\n  " << pTop->show() << "\n";
  std::cout << "\n  Walking simulated AST tree:\n -----------------------------";
  TreeWalk(pTop);
  std::cout << "\n\n";
  return 0;
}
#endif
