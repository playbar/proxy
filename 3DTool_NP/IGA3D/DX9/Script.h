#include "python.h"

class CScript
{
public:
	CScript();
	~CScript();

public:
	bool Init();
	int Run();
	void Destory();
    
private:
	bool		m_bPyInit;		//½Å±¾

	PyObject*	m_pModule;
	PyObject*	m_pFunc;

};

CScript* GetScript();