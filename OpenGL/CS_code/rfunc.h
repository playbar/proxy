
#ifndef RFUNC_H
#define RFUNC_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void *RedirectFunction( char *pSource, int iNbBytesToCopy, DWORD pDestination ) ;

#endif
