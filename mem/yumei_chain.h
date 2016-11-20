#ifndef __YUMEI_CHAIN_H__
#define __YUMEI_CHAIN_H__

typedef struct yumei_chain_s yumei_chain_t;

struct yumei_chain_s
{
	yumei_uchar_t* buf;
	int length;
	int io_len;
	int io_pos;
	int first_set;
	yumei_chain_t* next;
	yumei_chain_t* prev;
	
};

yumei_chain_t* yumei_chain_insert( yumei_chain_t* chain, int length );
yumei_chain_t* yumei_chain_append( yumei_chain_t* chain, int length );

yumei_chain_t* yumei_chain_link( yumei_chain_t* prev_chain, yumei_chain_t* next_chain );
yumei_chain_t* yumei_chain_get_next( yumei_chain_t* chain );

yumei_chain_t* yumei_chain_alloc();
int yumei_chain_free( yumei_chain_t* chain );
int yumei_chain_clear_io( yumei_chain_t* chain );
int yumei_chain_first_set( yumei_chain_t* chain );

#endif //__YUMEI_CHAIN_H__
