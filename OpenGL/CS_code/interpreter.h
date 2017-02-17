
#ifndef INTERPRETER_H
#define INTERPRETER_H

#pragma warning (disable:4786)
#pragma warning (disable:4018)
#include <vector>
#include <string>
using namespace std;
#include "stringfinder.h"

class CommandInterpreter
{

public:
	void AddCommand  ( const char *name, void(*func)()   ){ Add(name, Entry::COMMAND,   func); }
	void AddAlias    ( const char *name, string& content );
	void AddCvarInt  ( const char *name, int   *var      ){ Add(name, Entry::CVAR_INT,   var); }
	void AddCvarFloat( const char *name, float *var      ){ Add(name, Entry::CVAR_FLOAT, var); }
	void exec    (const char* cmdlist);
	void exec_one(const char* cmd);
	void execFile(const char* filename);
	string&  argS(int i){ --i;if(i<preExecArgs.size()) return preExecArgs[i];                  else return emptyString; }
	char*	 argC(int i){ --i;if(i<preExecArgs.size()) return const_cast<char*>(preExecArgs[i].c_str()); else return "";}
	int      argI(int i){ --i;if(i<preExecArgs.size()) return atoi(preExecArgs[i].c_str());              else return  0;}
	float    argF(int i){ --i;if(i<preExecArgs.size()) return (float)atof(preExecArgs[i].c_str());    else return  0.0f;}
	void     collectArguments(string& dest,int from=1,int to=100000);
	void init()	{createRandomPrefix();}
	CommandInterpreter(){init();}
	union{
		char          excludePrefixChar[5]; 
		unsigned long excludePrefixDword;   
	};
protected:
	void   Add( const char* name, int type, void* data);
	struct Entry
	{
		enum { CVAR_FLOAT, CVAR_INT, COMMAND, ALIAS, HL_CVAR };
		int   type;
		void* data; 
	};
protected:
	void extractArguments(const char* args);
	void logExec(const char* command);
	void createRandomPrefix();
	vector<Entry>  entries;
	StringFinder   names;
	vector<string> preExecArgs;
	string         emptyString;
};
extern CommandInterpreter cmd;

#endif
