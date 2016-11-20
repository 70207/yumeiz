#ifndef __YUMEI_POOL_H_20120118
#define __YUMEI_POOL_H_20120118

#define YUMEI_POOL_RESERVE_SIZE (3*1024)

typedef struct yumei_pool_s yumei_pool_t;
typedef struct yumei_mem_s yumei_mem_t;
typedef struct yumei_raw_s yumei_raw_t;

struct yumei_pool_s
{
	int			  num;
	int			  used;
	int			  reserved;
	int			  max;
	
	yumei_mem_t** mems;
	yumei_mem_t*  current;
	int  curIndex;

	int			  large_num;
	int			  large_used;

	yumei_mem_t** large;

	yumei_raw_t* raws;
};

struct yumei_mem_s
{
	yumei_pool_t* pool;
	yumei_mem_t** grandparent;
	yumei_mem_t*  parent;

	int length;
	yumei_uchar_t* start;
	yumei_uchar_t* pos;
	yumei_uchar_t* last;
	
	int io_pos;
};

struct yumei_raw_s
{
	yumei_uchar_t* data;
	int length;
	yumei_raw_t* next;
};

void* yumei_raw_malloc( yumei_pool_t* pool, int length );
yumei_mem_t* yumei_malloc( yumei_pool_t* pool, int length );
yumei_mem_t* yumei_ralloc( yumei_mem_t* mem, int nlength );

int yumei_free( yumei_mem_t* mem );

yumei_pool_t* yumei_pool_alloc( int reserved );

void yumei_pool_free( yumei_pool_t* pool );




#endif//__FUFEI_POOL_H_20120118
