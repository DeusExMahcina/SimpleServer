#include "imports.hxx"
#include "ErrorHandler.hxx"
#include "ServerInit.hxx"

#define SMALLOC

#ifdef SMALLOC
	#define malloc s_malloc 
	#define free s_free
#endif

int main ( char * argv[] , int argc )
{
	HANDLE hServerHandler = INVALID_HANDLE_VALUE;
	int * p1 = ( int * ) malloc ( sizeof (int ) );
	int * p2 = ( int * ) malloc ( sizeof (int ) );

	free ( p1 );
	free ( p2 );
	
	Cleanup();

	InitializeServer ( "4545" );

	return 0;
}