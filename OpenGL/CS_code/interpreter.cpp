
#define WIN32_LEAN_AND_MEAN
#include "interpreter.h"
#include "cvar.h"
#include "windows.h"
#include <fstream>

//===================================================================================
CommandInterpreter cmd;
#define cmd yxvkjn yxcvkjna
void Con_Echo(const char *fmt, ... );
void HandleCvarInt(char* name, int* value);
void HandleCvarFloat(char* name, float* value);
void HlEngineCommand(const char* command);
bool isHlCvar(char* name);
void HandleHlCvar(char* name);

//===================================================================================
void CommandInterpreter::logExec(const char* command)
{
	Con_Echo("FUNCTION %s CALL ArgCount %d\n",command,preExecArgs.size() );
	string collect;
	for(int i=0;i<preExecArgs.size();i++)
	{
		collect += " [";
		collect += preExecArgs[i];
		collect += "] ";
	}
	Con_Echo("Arguments %s", collect.c_str() );
} 

//===================================================================================
void CommandInterpreter::exec(const char* cmdlist)
{
	string my_copy = cmdlist;
	char* from = const_cast<char*>(my_copy.c_str());
	char* to   = from;	
	while(*from==' '||*from=='\t'){ ++from;++to; }
	while(*to>=' ' && *to!= ';'){
		if(*to=='\"')
		{
			do{ ++to; }while(*to && *to!='\"');
		}
		else
		{
			++to;
		}
	}
	do{
		if(from[0]=='/' && from[1]=='/') { return; }
		if(from<to)
		{
			char oldch = *to;
			*to = 0;
			exec_one(from);
			*to = oldch;
		}
		if(!*to) { break; }
		++to; 
		from = to;
		while(*from==' '||*from=='\t'){ ++from;++to; }  
		while(*to>=' ' && *to!= ';') ++to;            
	} while(1);
}

//===================================================================================
void CommandInterpreter::extractArguments(const char* const_args)
{
	preExecArgs.clear();
	char* args = const_cast<char*>(const_args);
	while(*args)
	{
		while(*args && *args<=' ') args++;
		if(!*args) break;
		char* start;
		if(*args=='\"') { start = ++args; while(*args!='\"' && *args ) args++;  }
		else            { start = args;   while(*args>' ') args++; }
		char last = *args;
		*args = 0;
		preExecArgs.push_back(start);
		*args = last;
		if(*args=='\"') args++;
	}
}

//===================================================================================
void CommandInterpreter::collectArguments(string& dest,int from,int to)
{
	dest.erase();
	--from;--to;
	int nArgs = preExecArgs.size();
	to = (to<nArgs) ? to : (nArgs-1);
	while(from<=to){
		dest += preExecArgs[from];
		if(from!=to) dest += " ";
		++from;
	};
}

//===================================================================================
void CommandInterpreter::createRandomPrefix()
{
	static char characterBox[] = "0123456789abcdefghijklmnopqrstuvwxyz"
		                         "ABCDEFGHIJKLMNOPQRSTUVWXYZ!$%&/()=?{}[]*#-.,<>~+_";
	static int len = sizeof(characterBox)-1;
	excludePrefixChar[0] = characterBox[rand()%len];
	excludePrefixChar[1] = characterBox[rand()%len];
	excludePrefixChar[2] = characterBox[rand()%len];
	excludePrefixChar[3] = characterBox[rand()%len];
	excludePrefixChar[4] = 0;
}

//===================================================================================
void CommandInterpreter::exec_one(const char* cur_cmd)
{
	if(*cur_cmd=='#' || *cur_cmd=='.')
	{
		if( false )
		{
			static string hlcommand;
			hlcommand.erase();
			hlcommand += excludePrefixChar;
			hlcommand += (cur_cmd+1);
			HlEngineCommand(hlcommand.c_str());
		} 
		else 
		{
			HlEngineCommand(cur_cmd+1);
		}
		return;
	}
	char  command[32];
	char* commandPos = command;
	int   commandCharsLeft = 31;
	while(*cur_cmd>' ' && commandCharsLeft)
	{
		*commandPos = tolower(*cur_cmd);
		commandPos++;
		cur_cmd++;
		commandCharsLeft--;
	}
	*commandPos = 0;
	while(*cur_cmd>' ') cur_cmd++;
	if(names.find(command))
	{
		Entry& entry = entries[names.num];
		switch(entry.type)
		{
		case Entry::ALIAS:{
			string& content = *(string*)(entry.data);
			if(cvar.info==5)Con_Echo("ALIAS %s EXPANSION: %s\n",command, content.c_str());
			exec(const_cast<char*>(content.c_str()));
						  }break;
		case Entry::COMMAND:{
			typedef void (*CmdFunc)();
			CmdFunc function = (CmdFunc)(entry.data);
			extractArguments(cur_cmd);
			if(cvar.info==5)logExec(command);
			function();
							}break;
		case Entry::CVAR_INT:
			extractArguments(cur_cmd);
			HandleCvarInt( command,   (int*)entry.data );
			break;
		case Entry::CVAR_FLOAT:
			extractArguments(cur_cmd);
			HandleCvarFloat( command, (float*)entry.data );
			break;
		case Entry::HL_CVAR:
			extractArguments(cur_cmd);
			HandleHlCvar(command);
			break;
		}
	} else {
		if(!isHlCvar(command))
		{
			Con_Echo("Unknown command &w%s",command);
		}
		else
		{
			Add(command,Entry::HL_CVAR,NULL);
			extractArguments(cur_cmd);
			HandleHlCvar(command);
		}
	}
}

//===================================================================================
void CommandInterpreter::execFile(const char* filename)
{
	if(strstr(filename,"..")) 
	{
		Con_Echo( "forbidden.\n" );
		return;
	}
	ifstream ifs(filename);
	if(!ifs)
	{
		return;
	}
	char buf[1024];
	while(ifs)
	{
		ifs.getline(buf,550,10);
		if(!ifs) { break; }
		exec(buf);
	}
}

//===================================================================================
void  CommandInterpreter::Add( const char *name, int type, void* data  )
{
	if(names.find(name))
	{
		Con_Echo("&w%s&a is already registered.\n",name);
		return;
	}
	int index = entries.size();
	Entry tmp = {type,data};
	entries.push_back(tmp);
	names.add(name,index);
}

//===================================================================================
inline static void lowercase(char* str)
{
	while(*str){ *str = tolower(*str); ++str; }
}

//===================================================================================
void  CommandInterpreter::AddAlias    ( const char *NamE, string& newcontent )
{
	char name[36];
	strncpy(name,NamE,31);
	name[31]=0;
	lowercase(name);
	if(names.find(name))
	{
		Entry& entry = entries[names.num];
		if(entry.type!=Entry::ALIAS)
		{
			Con_Echo("&w%s&a is already registered.\n",name);
			return;
		}
		*(string*)(entry.data) = newcontent;
		return;
	}
	if(isHlCvar(name))
	{
		Con_Echo("&w%s&a is a HL cvar.\n",name);
		return;
	}
	int index = entries.size();
	string* initial_content = new string;
	*initial_content = newcontent;
	Entry tmp = {Entry::ALIAS, initial_content};
	entries.push_back(tmp);
	names.add(name,index);
}
