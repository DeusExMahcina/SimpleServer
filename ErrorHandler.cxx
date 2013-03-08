#include "ErrorHandler.hxx"

struct alloc_mem
{
	void * ptr;
	struct alloc_mem * next;
};

DWORD dwLocalError = ERR_NO_ERR;
struct alloc_mem * memref = NULL;

void SetLocalError ( DWORD dwError )
{
	dwLocalError = dwError;
}

DWORD GetLocalError ( void )
{
	return dwLocalError;
}

BOOL Cleanup ( void )
{
	struct alloc_mem * memiter = NULL;
	struct alloc_mem * tempptr = NULL;
	memiter = memref;
	while ( NULL != memiter->next ){
		if ( NULL != memiter->ptr ) {
			free ( memiter->ptr );
		}
	}
	while ( NULL != memiter->next ){
		tempptr = memiter;
		memiter = memiter->next;
		free ( tempptr );
	}
	return TRUE;
}

void * s_malloc ( const size_t Size )
{
	void * ptr = NULL;
	struct alloc_mem * nodeiter = NULL;
	ptr = malloc ( Size );
	if( NULL == ptr )
	{
		return NULL;
	}
	if ( NULL == memref ){
		memref = ( struct alloc_mem * ) malloc ( sizeof ( struct alloc_mem ) );
		memref->next = NULL;
		memref->ptr = ptr;
		return ptr;
	}
	nodeiter = memref;
	while ( NULL != nodeiter->next ){
		nodeiter = nodeiter->next;
	}
	nodeiter->next = ( struct alloc_mem * ) malloc ( sizeof ( struct alloc_mem ) );
	nodeiter->next->ptr = ptr;
	nodeiter->next->next = NULL;
	return ptr;
}

void s_free ( void * _Memory )
{
	if ( NULL == memref ){
		SetLocalError ( ERR_NULL_MEMREF );
	}
	struct alloc_mem * nodeiter = NULL;
	struct alloc_mem * tempiter = NULL;
	nodeiter = memref;
	tempiter = memref;
	while ( ( nodeiter->next != NULL ) || ( _Memory == nodeiter->ptr ) ){
		nodeiter = nodeiter->next;
	}
	if ( NULL == nodeiter ){
		SetLocalError ( ERR_NO_ALLOC );
		return;
	} else {
		free ( _Memory );
		while ( ( tempiter->next != nodeiter ) ){
			if( ( NULL == tempiter->next ) && ( tempiter->next != nodeiter) ){
				SetLocalError ( ERR_NO_ALLOC );
				return;
			}
			tempiter = tempiter->next;
		}
		if( tempiter != NULL ){
			tempiter->next = nodeiter->next;
			free ( nodeiter );
			return;
		} else {
			return;
		}
	}
}