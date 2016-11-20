#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "yumei_types.h"
#include "yumei_chain.h"

int yumei_chain_free_part( yumei_chain_t* chain );

yumei_chain_t* yumei_chain_insert( yumei_chain_t* chain, int length )
{
	yumei_chain_t* prev;
	int size;
	yumei_uchar_t* buf;

	size = sizeof( yumei_chain_t );
	prev = ( yumei_chain_t* ) malloc( size );
	memset( prev, 0, size );

	buf = ( yumei_uchar_t* )malloc( length );
	prev->buf = buf;
	prev->prev = 0;
	prev->length = length;

	if( chain )
	{
		chain->prev = prev;
		prev->next=  chain;
	}

	return prev;
	
}
yumei_chain_t* yumei_chain_append( yumei_chain_t* chain, int length )
{
	yumei_chain_t* next;
	int size;
	yumei_uchar_t* buf;

	size = sizeof( yumei_chain_t );
	next = ( yumei_chain_t* ) malloc( size );
	memset( next, 0, size );

	buf = ( yumei_uchar_t* )malloc( length );
	next->buf = buf;
	next->next = 0;
	next->length = length;

	if( chain )
	{
		chain->next = next;
		next->prev = chain;
	}
	
	return next;

}

yumei_chain_t* yumei_chain_link( yumei_chain_t* prev_chain, yumei_chain_t* next_chain )
{
	yumei_chain_t* chain;
	if( !prev_chain )
	{
		return next_chain;
	}

	if( !next_chain )
	{
		return prev_chain;
	}

	chain = prev_chain;
	while( chain->next )
	{
		chain = chain->next;
	}
	chain->next = next_chain;
	next_chain->prev = chain;

	
	return prev_chain;
}

yumei_chain_t* yumei_chain_get_next( yumei_chain_t* chain )
{
	yumei_chain_t* _chain;
	if( !chain )
	{
		return 0;
	}
	
	_chain = chain;
	_chain->first_set = 0;

	while( _chain )
	{
		if( _chain->first_set )
		{
			yumei_chain_free_part( chain );
			chain = _chain;
			break;
		}

		if( _chain->io_pos < _chain->io_len )
		{
			break;
		}

		
		_chain = _chain->next;
	}

	chain->prev = 0;
	chain->first_set = 0;
	return chain;
}


int yumei_chain_free_part( yumei_chain_t* chain )
{
	yumei_chain_t* next;
	if( !chain )
	{
		return -1;
	}

	while( chain )
	{
		if( chain->first_set )
		{
			break;
		}
		
		next = chain->next;
		printf( "free buf:%s\n", chain->buf );
		free( chain->buf );
		free( chain );

		chain = next;
		
	}

	return 0;
}

int yumei_chain_free( yumei_chain_t* chain )
{
	yumei_chain_t* next;
	if( !chain )
	{
		return -1;
	}

	while( chain )
	{
		next = chain->next;
		free( chain->buf );
		free( chain );

		chain = next;
	}

	return 0;
}

int yumei_chain_clear_io( yumei_chain_t* chain )
{
	if( !chain )
	{
		return -1;
	}

	while( chain )
	{
		if( chain->first_set )
		{
			break;
		}

		chain->io_pos = 0;
		chain = chain->next;
	}

	return 0;
}

int yumei_chain_first_set( yumei_chain_t* chain )
{
	if( !chain )
	{
		return -1;
	}

	chain->first_set = 1;

	return 0;
}
