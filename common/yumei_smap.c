#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "yumei_types.h"
#include "yumei_pool.h"
#include "yumei_smap.h"

#define YUMEI_SMAP_RESERVE 32

int yumei_smap_insert_item( yumei_smap_i_t* i_item, const char* name, const char* value, int lenN, int lenV );
yumei_smap_ii_t* yumei_smap_get_item( yumei_smap_i_t* i_item, const char* name );

yumei_smap_t* yumei_smap_alloc( yumei_pool_t* pool )
{
	yumei_smap_t* smap;
	yumei_smap_i_t** items;
	yumei_smap_i_t* item;
	int size;
	int i;

	size = sizeof( yumei_smap_t );
	smap = ( yumei_smap_t* ) yumei_raw_malloc( pool, size );
	memset( smap, 0, size );

	size = sizeof( yumei_smap_i_t* ) * YUMEI_SMAP_RESERVE;
	items = ( yumei_smap_i_t** ) yumei_raw_malloc( pool, size );
	size = sizeof( yumei_smap_i_t );

	for( i = 0; i < YUMEI_SMAP_RESERVE; ++i )
	{
		item = ( yumei_smap_i_t* )yumei_raw_malloc( pool, size );
		item->length = i;
		item->num = 0;
		items[ i ] = item;
	}

	smap->items = items;
	return smap;
}

int yumei_smap_insert( yumei_smap_t* smap, const char* name, const char* value )
{
	yumei_pool_t* pool;
	yumei_smap_i_t** i_items;
	yumei_smap_i_t*  i_item;
	int size;
	int lenN;
	int lenV;
	
	if( !smap || !name || !value )
	{
		return 0;
	}

	pool = smap->pool;
	i_items = smap->items;
	lenN = strlen( name );
	lenV = strlen( value );
	i_item = i_items[ lenN - 1 ];
	smap->num++;

	return yumei_smap_insert_item( i_item, name, value, lenN, lenV );
	
}

yumei_smap_ii_t* yumei_smap_get( yumei_smap_t* smap, const char* name )
{
	int lenN;
	yumei_smap_i_t* i_item;

	if( !smap || !name )
	{
		return 0;
	}

	lenN = strlen( name );
	i_item = smap->items[ lenN - 1 ];
	if( !i_item->num )
	{
		return 0;
	}


	return yumei_smap_get_item( i_item, name );
}


int yumei_smap_insert_item( yumei_smap_i_t* i_item, const char* name, const char* value, int lenN, int lenV )
{
	yumei_smap_ii_t* ii_item;
	yumei_smap_ii_t* ii_item_prev;
	yumei_smap_ii_t* ii_item_next;
	yumei_smap_ii_t* ii_item_new;
	yumei_pool_t* pool;
	yumei_mem_t* mem;
	const char* cur;
	int next_t;
	int size;
	size = sizeof( yumei_smap_ii_t );

	ii_item_new = ( yumei_smap_ii_t* )yumei_malloc( pool, size );
	ii_item_new->name = ( char* )yumei_malloc( pool, lenN + 1 );
	ii_item_new->value = ( char* )yumei_malloc( pool, lenV + 1 );
	memcpy( ii_item_new->name, name, lenN + 1 );
	memcpy( ii_item_new->value, value, lenV + 1 );
	ii_item_new->parent = i_item;
	ii_item_new->prev = ii_item_new->next = 0;
	ii_item_new->next_t = 0;

	i_item->num++;

	if( !i_item->items )
	{
		i_item->items = ii_item_new;
		return 0;
	}

	ii_item = i_item->items;
	ii_item_prev = 0;
	next_t = 0;
	while( ii_item )
	{
		if( ii_item_prev )
		{
			if( ii_item_prev->next_t > next_t )
			{
				if( !ii_item_next )
				{
					ii_item->next = ii_item_new;
					ii_item_new->prev = ii_item;
					break;
				}

				ii_item = ii_item->next;
				ii_item_prev = ii_item;
				ii_item_next = ii_item->next;
				continue;
			}
			else if( ii_item_prev->next_t < next_t )
			{
				ii_item_prev->next = ii_item;
				ii_item->prev = ii_item_prev;
				ii_item_new->name = ii_item;
				ii_item->prev = ii_item_new;
				ii_item_new->next_t = ii_item_prev->next_t;
				break;
			}
		}

		for( ;next_t < lenN; ++next_t )
		{
			if( ii_item->name[ next_t ] > name[ next_t ] )
			{
				ii_item_new->next_t = next_t;
				ii_item_new->next = ii_item;
				if( !ii_item->prev )
				{
					i_item->items = ii_item_new;
				}
				else
				{
					ii_item->prev->next = ii_item_new;
					ii_item_new->prev = ii_item->prev;
				}

				ii_item->prev = ii_item_new;
			}
			else if( ii_item->name[ next_t ] < name[ next_t ] )
			{
				if( !ii_item->next )
				{
					ii_item->next = ii_item_new;
					ii_item_new->prev = ii_item;
					ii_item->next_t = next_t;
				}
				else
				{
					ii_item_prev = ii_item;
					ii_item = ii_item->next;
					ii_item_next = ii_item->next;
					break;
				}
			}
		}
		if( next_t == lenN )
		{
			break;
		}
	}

	return 0;

}


yumei_smap_ii_t* yumei_smap_get_item( yumei_smap_i_t* i_item, const char* name )
{
	yumei_smap_ii_t* ii_item;
	yumei_smap_ii_t* ii_item_prev;
	yumei_smap_ii_t* ii_item_next;
	yumei_smap_ii_t* ii_item_new;
	yumei_pool_t* pool;
	yumei_mem_t* mem;
	const char* cur;
	int next_t;
	int size;

	if( !i_item->items )
	{
		return 0;
	}

	ii_item = i_item->items;
	next_t = 0;

	while( ii_item )
	{
		for( ;next_t < i_item->length; ++next_t )
		{
			if( ii_item->name[ next_t ] > name[ next_t ] )
			{
				return 0;
			}
			else if( ii_item->name[ next_t ] < name[ next_t ] )
			{
				ii_item_prev = ii_item;
				ii_item = ii_item->next;
				break;
			}
		}

		if( next_t == i_item->length )
		{
			return ii_item;
		}
	}


	return 0;
}
