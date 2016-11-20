#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "yumei_types.h"
#include "yumei_pool.h"

#define FUFEI_POOL_NUM_RESERVE 8

yumei_mem_t* yumei_mem_alloc( int length );
yumei_mem_t* yumei_mem_ralloc( yumei_mem_t* mem, int nlength );
void yumei_mem_free( yumei_mem_t* mem );

void* yumei_raw_malloc( yumei_pool_t* pool, int length )
{
	yumei_raw_t* raw;
	yumei_raw_t* prev;
	yumei_uchar_t* data;
	int size;

	if( !pool || length <= 0 )
	{
		return 0;
	}

	data = ( yumei_uchar_t* )malloc( length );
	size = sizeof( yumei_raw_t );
	raw = ( yumei_raw_t* ) malloc( size );
	memset( raw, 0, size );
	raw->length = length;
	raw->next = 0;
	raw->data = data;

	if( pool->raws )
	{
		prev = pool->raws;
		while( prev->next )
		{
			prev = prev->next;
		}
		prev->next = raw;
	}
	else
	{
		pool->raws = raw;
	}

	return data;

}

yumei_mem_t* yumei_malloc( yumei_pool_t* pool, int length )
{
	yumei_mem_t** mems;
	yumei_mem_t*  mem;
	yumei_mem_t*  rtm;
	int mem_num;
	int current;
	int mem_length;
	int size;
	yumei_uchar_t* data;
	int i;
	

	if(!pool || length <= 0 )
	{
		return 0;
	}

	size = sizeof( yumei_mem_t );
	rtm = ( yumei_mem_t* ) malloc( size );
	memset( rtm, 0, size );
	rtm->length = length;
	rtm->pool = pool;

	
	if( pool->used >= pool->num - 1 )
	{
		size = sizeof( yumei_mem_t* );
		mem_num = pool->num + FUFEI_POOL_NUM_RESERVE;
		pool->mems = ( yumei_mem_t** )realloc( pool->mems, mem_num * size );
		pool->current = pool->mems[ pool->curIndex ];
		
		for( i = pool->num; i < mem_num; ++i )
		{
			pool->mems[ i ] = yumei_mem_alloc( pool->reserved );
		}

		pool->num = mem_num;
	}
	

	if( length > pool->reserved )
	{
		if( pool->large_used >= pool->large_num - 1 )
		{
			size = sizeof( yumei_mem_t* );
			mem_num = pool->large_num + FUFEI_POOL_NUM_RESERVE;
			pool->large = ( yumei_mem_t** )realloc( pool->large, mem_num * size );
			pool->large_num = mem_num;
		}

		mem = yumei_mem_alloc( length );
		pool->large[ pool->large_used++ ] = mem;

		if( length > pool->max )
		{
			pool->max = length;
		}

		rtm->start = rtm->pos = mem->start;
		rtm->last = rtm->start + length -1;
		rtm->parent = 0;
		rtm->grandparent = pool->large + pool->large_used - 1;
		return rtm;
	}

	mem = pool->current;
	mem_length = mem->last - mem->pos + 1;
	if( mem_length >= length )
	{
		rtm->start = rtm->pos = mem->pos;
		mem->pos+= length;
	}
	else
	{
		pool->used++;
		pool->curIndex++;
		pool->current = pool->mems[ pool->curIndex ];
		mem = pool->current;
		mem->pos = mem->start + length;
		rtm->start = rtm->pos = mem->start;
	}

	rtm->last = rtm->start + length - 1;
	rtm->parent = mem;
	return rtm;

}

yumei_mem_t* yumei_ralloc( yumei_mem_t* mem, int nlength )
{
	yumei_pool_t* pool;
	yumei_mem_t*  parent;
	yumei_mem_t** grandparent;
	yumei_mem_t*  rtm;

	if( !mem || nlength < mem->length )
	{
		return mem;
	}

	pool = mem->pool;
	parent = mem->parent;
	grandparent = mem->grandparent;

	if( !parent )
	{
		rtm = yumei_mem_ralloc( mem, nlength );
		*grandparent = rtm;
		return rtm;
	}
	
	if( parent->pos == mem->start + mem->length )
	{
		if( parent->last >= mem->start + nlength )
		{
			mem->length = nlength;
			mem->last = mem->start + nlength - 1;
			parent->pos = mem->last + 1;
			rtm = mem;
			return rtm;
		}
	}
		
	
	rtm = yumei_malloc( pool, nlength );
	memcpy( rtm->start, mem->start, mem->length );
	yumei_free( mem );

	return rtm;

}

int yumei_free( yumei_mem_t* mem )
{
	if( !mem )
	{
		return -1;
	}

	free( mem );

	return 0;
}

yumei_pool_t* yumei_pool_alloc( int reserved )
{
	yumei_pool_t* pool;
	yumei_mem_t** mems;
	yumei_mem_t*  mem;
	int i;

	yumei_mem_t** large;
	int size;

	size = sizeof( yumei_pool_t );
	
	pool = ( yumei_pool_t* ) malloc( size );
	
	//printf( "pool alloc:%llx\n", pool );

	size = sizeof( yumei_mem_t* );
	mems = ( yumei_mem_t** )malloc( FUFEI_POOL_NUM_RESERVE * size );
	for( i = 0; i < FUFEI_POOL_NUM_RESERVE; ++i )
	{
		mem = yumei_mem_alloc( reserved );
		mems[ i ] = mem;
	}

	pool->mems = mems;
	pool->current = pool->mems[ 0 ];
	pool->reserved = reserved;
	pool->used = 0;
	pool->large = 0;
	pool->large_num = 0;
	pool->large_used = 0;
	pool->raws = 0;
	pool->curIndex = 0;
	pool->num = FUFEI_POOL_NUM_RESERVE;
	pool->max = 0;

	return pool;
}

yumei_mem_t* yumei_mem_alloc( int length )
{
	yumei_mem_t* mem;
	int size;
	yumei_uchar_t* data;

	size = sizeof( yumei_mem_t );

	mem = ( yumei_mem_t* )malloc( size );
	if(!mem)
	{
		printf( "mem alloc wrong-------------\n");
		return 0;
	}

	data = ( yumei_uchar_t*) malloc( length );
	if( !data )
	{
		printf( "mem alloc wrong2-------------\n");
		free( mem );
		return 0;
	}

	mem->length = length;
	mem->start = data;
	mem->pos = data;
	mem->last = data + length - 1;
	mem->io_pos = 0;
	
	return mem;
}

yumei_mem_t* yumei_mem_ralloc( yumei_mem_t* mem, int nlength )
{
	yumei_mem_t* rtm;
	int size;
	int used;
	yumei_uchar_t* data;

	size = sizeof( yumei_mem_t );

	if( nlength <= 0 )
	{
		return mem;
	}

	if( !mem )
	{
		rtm = ( yumei_mem_t* )malloc( size );
	}
	else
	{
		rtm = mem;
	}


	if(!rtm )
	{
		return 0;
	}

	if( !mem )
	{
		data = (yumei_uchar_t*)malloc( nlength );
		used = 0;
	}
	else
	{
		used = mem->pos - mem->start;
		data = ( yumei_uchar_t* ) realloc( mem->start, nlength );
	}

	if( !data )
	{
		free( rtm );
		return 0;
	}

	rtm->start = data;
	rtm->pos = rtm->start + used;
	rtm->last = rtm->start + nlength - 1;
	return rtm;
}

void yumei_pool_free( yumei_pool_t* pool )
{
	int i;

	//printf( "pool free:%llx\n", pool );
	if( !pool )
	{
		return;
	}

	for( i = 0; i < pool->num; ++i )
	{
		yumei_mem_free( pool->mems[ i ] );
	}

	for( i = 0; i < pool->large_used; ++i )
	{
		yumei_mem_free( pool->large[ i ] );
	}

	if( pool->mems )
	{
		free( pool->mems );
	}

	if( pool->large )
	{
		free( pool->large );
	}

	yumei_raw_free( pool->raws );


	free( pool );
}

int yumei_raw_free( yumei_raw_t* raw )
{
	yumei_raw_t* next;
	if( !raw )
	{
		return 0;
	}
	
	while( raw )
	{
		next = raw->next;
		free( raw->data );
		free( raw );
		raw = next;
	}
	
	return 0;
}

void yumei_mem_free( yumei_mem_t* mem )
{
	if( !mem )
	{
		return;
	}

	free( mem->start );
	free( mem );

}
