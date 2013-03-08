#pragma once
#include "imports.hxx"

BOOL OpenDbgConsole ( void );

BOOL EnumerateThreads ( void );
BOOL mDbgSuspendThread ( HANDLE hThread ) ;
BOOL mDbgResumeThread ( HANDLE hThread) ;

BOOL mDbgShowAsm ( HANDLE hCurrentThread ) ;

BOOL ParseLine ( PCHAR szLine ) ;