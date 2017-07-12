/////////////////////////////////////////////////////////////////////
// Tokenizer.cpp - read words from a std::stream                   //
// ver 1.0                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Project 3 , CSE687 - Object Oriented Design        //
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10				   //
// Author:      Jinkal Arvind Javia, SUID: 425325424     	       //
//              jjavia@syr.edu		                               //
// Source:		Jim Fawcett, Syracuse University, CST 4-187        //
//              jfawcett@twcny.rr.com				               //
/////////////////////////////////////////////////////////////////////

#include "Tokenizer.h"
#include <iostream>
#include <cctype>
#include <string>
#include <map>

/*  Context is a shared data storage facility; with Context we don't need static members to share state pointes etc. Context holds and shares all that.
* - Context is an entirely private facility used by ConsumeState and its derived instances, so there is no need for encapsulation
*   and we use a struct. This won't be appropriate if we handed the context to clients of Toker.*/
namespace Scanner{
  struct Context{
	  Context();
	  ~Context();
	  std::string token;
	  std::istream* _pIn;
	  std::map<std::string, int> singleSpecialChar = { { "<",1 },{ ">",2 },{ "[",3 },{ "]",4 },{ "(",5 },{ ")",6 },{ "{",7 },{ "}",8 },{ ":",9 },{ "=",10 },{ "+",11 },{ "-",12 },{ "*",13 },{ "\\n",14 } };
	  std::map<std::string, int> doubleSpecialChar = { { "<<",1 },{ ">>",2 },{ "::",3 },{ "++",4 },{ "--",5 },{ "==",6 },{ "+=",7 },{ "-=",8 },{ "*=",9 },{ "/=",10 } };
	  int prevChar;
	  int currChar;
	  bool _doReturnComments;
	  size_t _lineCount;
	  ConsumeState* _pState;
	  ConsumeState* _pEatCppComment;
	  ConsumeState* _pEatCComment;
	  ConsumeState* _pEatWhitespace;
	  ConsumeState* _pEatPunctuator;
	  ConsumeState* _pEatAlphanum;
	  ConsumeState* _pEatNewline;
	  ConsumeState* _pEatSingleSpecialChar;
	  ConsumeState* _pEatDoubleSpecialChar;
	  ConsumeState* _pEatString;
	  ConsumeState* _pEatChar;
  };
  /* This ConsumeState class is nearly identical to the ConsumeState that uses static member sharing.
  *  We've just replaced the static members with references to those members stored in the context.*/
  class ConsumeState{
  public:
	using Token = std::string;
    ConsumeState();
	ConsumeState(const ConsumeState&) = delete;
	ConsumeState& operator=(const ConsumeState&) = delete;
    virtual ~ConsumeState();
    void attach(std::istream* pIn) { _pContext->_pIn = pIn; }
    virtual void eatChars() = 0;
    void consumeChars() {
      _pContext->_pState->eatChars();
	  _pContext->_pState = nextState();
	}
    bool canRead() { return _pContext->_pIn->good(); }
    std::string getTok() { return _pContext->token; }
    bool hasTok() { return _pContext->token.size() > 0; }
    ConsumeState* nextState();
	void setSpecialSingleChars(std::string ssc);
	void setSpecialCharPairs(std::string scp);
	void returnComments(bool doReturnComments = false);
	size_t currentLineCount();
	std::string checkFlag(bool flag, int nextchar, std::string charcurr, std::string charnext);
	ConsumeState* checkpunct(int, std::string, std::string);
	ConsumeState* checkCharString(int);
	void setContext(Context* pContext);
  protected:
	Context* _pContext;
  };
}

using namespace Scanner;

//----< used by Toker to provide Consumer with Context ptr >---------
void ConsumeState::setContext(Context* pContext)
{
	_pContext = pContext;
}

void testLog(const std::string& msg);

//Checks what the nextState is for the input stream
ConsumeState* ConsumeState::nextState(){
  if (!(_pContext->_pIn->good()))
	return nullptr;
  int chNext = _pContext->_pIn->peek();
  bool flag = true;
  std::string current, next, specialChar;
  current = _pContext->currChar;
  specialChar = checkFlag(flag,chNext,current,next);
  if (_pContext->currChar == '/' && chNext == '/') {
	  testLog("state: eatCppComment");
	  return _pContext->_pEatCppComment;
  }
  if (_pContext->currChar == '/' && chNext == '*') {
	  testLog("state: eatCComment");
	  return _pContext->_pEatCComment;
  }
  ConsumeState* ccs = checkCharString(_pContext->currChar);
  if (ccs != nullptr)
	  return ccs;
  if (std::isspace(_pContext->currChar) && _pContext->currChar != '\n'){
    testLog("state: eatWhitespace");
    return _pContext->_pEatWhitespace;
  }
  if (_pContext->currChar == '\n'){
    testLog("state: eatNewLine");
    return _pContext->_pEatNewline;
  } 
  if (std::isalnum(_pContext->currChar) || (_pContext->currChar == '_' && (std::isalnum(chNext) || chNext == '_'))){
    testLog("state: eatAlphanum");
    return _pContext->_pEatAlphanum;
  }
  ConsumeState* punct = checkpunct(_pContext->currChar, current, specialChar);
  if (punct!= nullptr)
	  return punct;
  if (!_pContext->_pIn->good())
	  return _pContext->_pEatWhitespace;
  throw(std::logic_error("invalid type"));
}

//Eats Whitespace whenever they are encountered
class EatWhitespace : public ConsumeState
{
public:
  EatWhitespace(Context* pContext){
	  _pContext = pContext;
  }
  virtual void eatChars()
  {
	  _pContext->token.clear();
    do {
      if (!_pContext->_pIn->good())  // end of stream
        return;
	  _pContext->currChar = _pContext->_pIn->get();
    } while (std::isspace(_pContext->currChar) && _pContext->currChar != '\n');
  }
};

/*EatCppComment eats Cpp comment when show_comment is false,
else it returns Cpp comment as single token*/
class EatCppComment : public ConsumeState
{
public:
  EatCppComment(Context* pContext){
	  _pContext = pContext;
  }
  virtual void eatChars()
  {
	  _pContext->token.clear();
    do {
		if(_pContext->_doReturnComments)
			_pContext->token += _pContext->currChar;
		if (!_pContext->_pIn->good())  // end of stream
			return;
		_pContext->currChar = _pContext->_pIn->get();
    } while (_pContext->currChar != '\n');
  }
};

/*EatCComment eats C comment when show_comment is false,
else it returns C comment as single token*/
class EatCComment : public ConsumeState
{
public:
	EatCComment(Context* pContext) {
		_pContext = pContext;
	}
  virtual void eatChars()
  {
	  _pContext->token.clear();
    do {
		if (!_pContext->_pIn->good())  // end of stream
			return;
		if (_pContext->currChar == '\n')
			++(_pContext->_lineCount);
		if(_pContext->_doReturnComments)
			_pContext->token += _pContext->currChar;
		_pContext->currChar = _pContext->_pIn->get();
    } while (_pContext->currChar != '*' || _pContext->_pIn->peek() != '/');
	if (_pContext->currChar == '*'){
		if(_pContext->_doReturnComments)
			_pContext->token += _pContext->currChar;
		_pContext->currChar = _pContext->_pIn->get();
	}
	if (_pContext->currChar == '/'){
		if(_pContext->_doReturnComments)
			_pContext->token += _pContext->currChar;
	}
	_pContext->_pIn->get();
	_pContext->currChar = _pContext->_pIn->get();
  }
};

/*EatSingleSpecialChar returns single token for each single special
character encountered*/
class EatSingleSpecialChar : public ConsumeState
{
public:
	EatSingleSpecialChar(Context* pContext) {
		_pContext = pContext;
	}
	virtual void eatChars()
	{
		std::string current, next, specialChar;
		current = _pContext->currChar;
		next = _pContext->_pIn->peek();
		specialChar = current + next;
		_pContext->token.clear();
		//if (!_pIn->good())  // end of stream
		//	return;
	    if ((_pContext->singleSpecialChar.find(current) != _pContext->singleSpecialChar.end())){
			_pContext->token += _pContext->currChar;
		}
		else if ((_pContext->singleSpecialChar.find(specialChar) != _pContext->singleSpecialChar.end())){
			_pContext->token += _pContext->currChar;
			_pContext->currChar = _pContext->_pIn->get();
			_pContext->token += _pContext->currChar;
		}
		_pContext->currChar = _pContext->_pIn->get();
	}
};

/*EatDoubleSpecialChar returns single token for each double special
character encountered*/
class EatDoubleSpecialChar : public ConsumeState
{
public:
	EatDoubleSpecialChar(Context* pContext) {
		_pContext = pContext;
	}
	virtual void eatChars()
	{
		std::string current, next,specialChar;
		current = _pContext->currChar;
		next = _pContext->_pIn->peek();
		specialChar = current + next;
		_pContext->token.clear();
		if (!_pContext->_pIn->good())  // end of stream
			return;
		if (_pContext->doubleSpecialChar.find(specialChar) != _pContext->doubleSpecialChar.end())
		{
			_pContext->token += _pContext->currChar;
			_pContext->currChar = _pContext->_pIn->get();
			_pContext->token += _pContext->currChar;
		}
		_pContext->currChar = _pContext->_pIn->get();
	}
};

/*EatChar returns single token for each character encountered*/
class EatChar : public ConsumeState
{
public:
	EatChar(Context* pContext) {
		_pContext = pContext;
	}
	virtual void eatChars()
	{
		_pContext->token.clear();
		if (!_pContext->_pIn->good())  // end of stream
			return;
		_pContext->currChar = _pContext->_pIn->get();
		if (_pContext->currChar == '\''){
			_pContext->currChar = _pContext->_pIn->get();
			return;
		}
		if (_pContext->currChar == '\\'){
			if (_pContext->_pIn->peek() != '\"' && _pContext->_pIn->peek() != '\'')
				_pContext->token += _pContext->currChar;
			_pContext->currChar = _pContext->_pIn->get();
			_pContext->token += _pContext->currChar;
			_pContext->currChar = _pContext->_pIn->get();
			_pContext->currChar = _pContext->_pIn->get();
			return;
		}
		_pContext->token += _pContext->currChar;
		_pContext->currChar = _pContext->_pIn->get();
		_pContext->currChar = _pContext->_pIn->get();
	}
};

/*EatString returns single token for each string encountered*/
class EatString : public ConsumeState
{
public:
	EatString(Context* pContext) {
		_pContext = pContext;
	}
	virtual void eatChars()
	{
		_pContext->token.clear();
		if (!_pContext->_pIn->good())  // end of stream
			return;
		_pContext->token += _pContext->currChar;
		_pContext->currChar = _pContext->_pIn->get();
		if (_pContext->currChar == '"'){
			_pContext->token += _pContext->currChar;
			_pContext->currChar = _pContext->_pIn->get();
			return;
		}
		else {
			do {
				if (!_pContext->_pIn->good())  // end of stream
					return;
				if((_pContext->currChar == '\\' && _pContext->_pIn->peek() == '"')|| (_pContext->currChar == '\\' && _pContext->_pIn->peek() == '\'')){
					_pContext->currChar = _pContext->_pIn->get();
					_pContext->token += _pContext->currChar;
					_pContext->currChar = _pContext->_pIn->get();
				}
				else if (_pContext->currChar == '\\' && _pContext->_pIn->peek() == '\\'){
					_pContext->token += _pContext->currChar;
					_pContext->currChar = _pContext->_pIn->get();
					_pContext->token += _pContext->currChar;
					_pContext->currChar = _pContext->_pIn->get();
				}
				else {
					_pContext->token += _pContext->currChar;
					_pContext->currChar = _pContext->_pIn->get();
				}
			} while (_pContext->currChar != '"');
			_pContext->token += _pContext->currChar;
			_pContext->currChar = _pContext->_pIn->get();
		}
	}
};

/*EatPunctuator returns single token for punctuators encountered*/
class EatPunctuator : public ConsumeState
{
public:
  EatPunctuator(Context* pContext) {
		_pContext = pContext;
  }
  virtual void eatChars()
  {
	  _pContext->token.clear();
	std::string specialChar;
	std::string current, next;
    do {
		_pContext->token += _pContext->currChar;
      if (!_pContext->_pIn->good())  // end of stream
        return;
	  _pContext->currChar = _pContext->_pIn->get();
	  current = _pContext->currChar;
	  next = _pContext->_pIn->peek();
	  specialChar = current + next;
    } while (ispunct(_pContext->currChar) && _pContext->currChar!='/' && _pContext->currChar!='\'' && _pContext->currChar != '\"' && (_pContext->doubleSpecialChar.find(specialChar) == _pContext->doubleSpecialChar.end()) && (_pContext->singleSpecialChar.find(current) == _pContext->singleSpecialChar.end()));
  }
};

/*EatAlphanum returns single token for alphanums when encountered*/
class EatAlphanum : public ConsumeState
{
public:
	EatAlphanum(Context* pContext) {
		_pContext = pContext;
	}
  virtual void eatChars()
  {
	  _pContext->token.clear();
    do {
		_pContext->token += _pContext->currChar;
      if (!_pContext->_pIn->good())  // end of stream
        return;
	  _pContext->currChar = _pContext->_pIn->get();
    } while (isalnum(_pContext->currChar) || _pContext->currChar == '_');
  }
};

/*EatNewLine returns single token when newline is encountered*/
class EatNewline : public ConsumeState
{
public:
	EatNewline(Context* pContext) {
		_pContext = pContext;
	}
  virtual void eatChars()
  {
	  _pContext->token.clear();
	if (_pContext->currChar == '\n')
	{
		++(_pContext->_lineCount);
		_pContext->token += _pContext->currChar;
		if (!_pContext->_pIn->good())  // end of stream
			return;
		_pContext->currChar = _pContext->_pIn->get();
	}
  }
};

//Function to check if next state is EatPunctuator OR EatDoubleSpecialChar OR EatSingleSpecialChar
ConsumeState* Scanner::ConsumeState::checkpunct(int current, std::string specialChar1,std::string specialChar2)
{
	if (ispunct(_pContext->currChar) && (_pContext->doubleSpecialChar.find(specialChar2) == _pContext->doubleSpecialChar.end()) && (_pContext->singleSpecialChar.find(specialChar2) == _pContext->singleSpecialChar.end()) &&(_pContext->singleSpecialChar.find(specialChar1) == _pContext->singleSpecialChar.end())) {
		testLog("state: eatPunctuator");
		return _pContext->_pEatPunctuator;
	}
	if (_pContext->doubleSpecialChar.find(specialChar2) != _pContext->doubleSpecialChar.end()) {
		testLog("state: eatDoubleSpecialChar");
		return _pContext->_pEatDoubleSpecialChar;
	}
	if (_pContext->singleSpecialChar.find(specialChar1) != _pContext->singleSpecialChar.end() || (_pContext->singleSpecialChar.find(specialChar2) != _pContext->singleSpecialChar.end())) {
		testLog("state: eatSingleSpecialChar");
		return _pContext->_pEatSingleSpecialChar;
	}
	return nullptr;
}

//Function to check if next state is EatChar OR EatString
ConsumeState* Scanner::ConsumeState::checkCharString(int current) {
	if (current == '\'') {
		testLog("state: eatChar");
		return _pContext->_pEatChar;
	}
	if (current == '"') {
		testLog("state: eatString");
		return _pContext->_pEatString;
	}
	return nullptr;
}

//Function to check if End of file is detected and sets flag accordingly
std::string Scanner::ConsumeState::checkFlag(bool flag,int nextchar,std::string charcurr, std::string charnext)
{
	std::string specialChar;
	//if (!(_pIn->good()))
		//return "";
	if (nextchar == EOF) {
		_pContext->_pIn->clear(); //clear() restores state to good
		flag = false;
		std::string temp;
		temp += _pContext->currChar;
		return temp;
	}
	if (flag) {
		charcurr = _pContext->currChar;
	    charnext = _pContext->_pIn->peek();
		specialChar = charcurr + charnext;
	}
	return specialChar;
}

//Function setSpecialSingleChars(string ssc): to change defaults of Single Special Character tokens
void Scanner::ConsumeState::setSpecialSingleChars(std::string ssc)
{
	std::string::size_type p = 0;
	std::string::size_type q = ssc.find(',');
	if (q == std::string::npos)
	{
		_pContext->singleSpecialChar[ssc] = _pContext->singleSpecialChar.size() + 1;
	}
	while (q != std::string::npos) 
	{
		_pContext->singleSpecialChar[ssc.substr(p, q - p)] = _pContext->singleSpecialChar.size() + 1;
		p = ++p;
		q = ssc.find(',', q);
		if (q == std::string::npos)
			_pContext->singleSpecialChar[ssc.substr(p, ssc.length())] = _pContext->singleSpecialChar.size() + 1;
	}
}

void::Scanner::Toker::setSpecialSingleChars(std::string ssc)
{
	pConsumer->setSpecialSingleChars(ssc);
}

//Function setSpecialCharPairs(std::string spc): to change defaults of Double Special Character tokens
void Scanner::ConsumeState::setSpecialCharPairs(std::string spc)
{
	std::string::size_type p = 0;
	std::string::size_type q = spc.find(',');
	if (q == std::string::npos)
	{
		_pContext->doubleSpecialChar[spc] = _pContext->doubleSpecialChar.size() + 1;
	}
	while (q != std::string::npos) {
		_pContext->doubleSpecialChar[spc.substr(p, q - p)] = _pContext->doubleSpecialChar.size() + 1;
		p = ++q;
		q = spc.find(',', q);
		if (q == std::string::npos)
			_pContext->doubleSpecialChar[spc.substr(p, spc.length())] = _pContext->doubleSpecialChar.size() + 1;
	}
}

void::Scanner::Toker::setSpecialCharPairs(std::string spc)
{
	pConsumer->setSpecialCharPairs(spc);
}

//----< turn on\off returning comments as tokens >-------------------
void Toker::returnComments(bool doReturnComments)
{
	pConsumer->returnComments(doReturnComments);
}

//----< return number of newlines collected from stream >------------

size_t Toker::currentLineCount() { return pConsumer->currentLineCount(); }

//----< return number of newlines collected from stream >------------

size_t ConsumeState::currentLineCount() { return _pContext->_lineCount; }

//----< start/stop returning comments as tokens >--------------------

void ConsumeState::returnComments(bool doReturnComments)
{
	_pContext->_doReturnComments = doReturnComments;
}

//Construct shared data storage
Context::Context()
{
	  _pEatAlphanum = new EatAlphanum(this);
	  _pEatCComment = new EatCComment(this);
	  _pEatCppComment = new EatCppComment(this);
	  _pEatPunctuator = new EatPunctuator(this);
	  _pEatWhitespace = new EatWhitespace(this);
	  _pEatNewline = new EatNewline(this);
	  _pEatDoubleSpecialChar = new EatDoubleSpecialChar(this);
	  _pEatSingleSpecialChar = new EatSingleSpecialChar(this);
	  _pEatString = new EatString(this);
	  _pEatChar = new EatChar(this);
	  _pState = _pEatWhitespace;
	  _lineCount = 1;
	  _doReturnComments = false;
}

//Return shared resourses
Context::~Context()
{
	delete _pEatAlphanum;
    delete _pEatCComment;
    delete _pEatCppComment;
    delete _pEatPunctuator;
    delete _pEatWhitespace;
    delete _pEatNewline;
	delete _pEatSingleSpecialChar;
	delete _pEatDoubleSpecialChar;
	delete _pEatString;
	delete _pEatChar;
}

//----< no longer has anything to do - will be removed >-------------

ConsumeState::ConsumeState() {}

//----< no longer has anything to do - will be removed >-------------

ConsumeState::~ConsumeState() {}

Toker::Toker()
{
	_pContext = new Context();
	pConsumer = _pContext->_pEatWhitespace;
	pConsumer->setContext(_pContext);
}

//----< destructor deletes the current state >-----------------------
/*
* deleting any state deletes them all since derived destructor
* implicitly invokes the base destructor
*/
Toker::~Toker()
{
	delete _pContext;
	_pContext = nullptr;
}

//returns true if attaching pConsumer pointer to input stream is successful
bool Toker::attach(std::istream* pIn)
{
  if (pIn != nullptr && pIn->good())
  {
    pConsumer->attach(pIn);
    return true;
  }
  return false;
}

//gets the tokens
std::string Toker::getTok()
{
  while(true) 
  {
    if (!pConsumer->canRead())
      return "";
    pConsumer->consumeChars();
    if (pConsumer->hasTok())
      break;
  }
  return pConsumer->getTok();
}

bool Toker::canRead() { return pConsumer->canRead(); }

void testLog(const std::string& msg)
{
#ifdef TEST_LOG
  std::cout << "\n  " << msg;
#endif
}

//----< test stub >--------------------------------------------------

#ifdef TEST_TOKENIZER

#include <fstream>

int main()
{  
  std::string fileSpec = "../Tokenizer/test.txt";
  //std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
  //std::string fileSpec = "../Tokenizer/Tokenizer.h";
 
  std::ifstream in(fileSpec);
  if (!in.good())
  {
    std::cout << "\n  can't open " << fileSpec << "\n\n";
    return 1;
  }
  Toker toker;
  toker.attach(&in);
 do
  {
    std::string tok = toker.getTok();
    if (tok == "\n")
      tok = "newline";
    std::cout << "\n -- " << tok;
 } while (in.good());
  std::cout << "\n\n";
  return 0;
}
#endif