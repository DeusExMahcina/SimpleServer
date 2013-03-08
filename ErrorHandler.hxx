#pragma once

#include "imports.hxx"
#include "ecodes.hxx"

void SetLocalError ( DWORD dwError ) ;
DWORD GetLocalError ( void ) ;
BOOL Cleanup ( void );


void * s_malloc ( size_t Size ) ;
void s_free ( void * _Memory ) ;