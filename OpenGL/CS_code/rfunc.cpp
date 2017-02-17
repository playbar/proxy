
#include "rfunc.h"
#include "mpatcher.h"

#define MakePtr( cast, ptr, addValue )( cast )( ( DWORD )( ptr ) + ( DWORD )( addValue ) )

//===================================================================================
void *RedirectFunction( char *pSource, int iNbBytesToCopy, DWORD pDestination ) 
{ 
	DWORD old_attributes; 
	char *pTrampoline = new char[ iNbBytesToCopy + 5 ]; 
	memcpy( pTrampoline, pSource, iNbBytesToCopy ); 
	*( ( char * )( pTrampoline + iNbBytesToCopy ) ) = '\xE9';
	*( ( DWORD * )( pTrampoline + iNbBytesToCopy + 1 ) ) = (DWORD)pSource - ( (DWORD)pTrampoline + 5 ); 
	VirtualProtect( pSource, 5, PAGE_READWRITE, &old_attributes ); 
	*pSource = '\xE9';
	*( ( DWORD * )( pSource + 1 ) ) = pDestination - ( (DWORD)pSource + 5 ); 
	VirtualProtect( pSource, 5, old_attributes, &old_attributes ); 
	return ( void * )pTrampoline; 
}
