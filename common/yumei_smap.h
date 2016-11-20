#ifndef __YUMEI_SMAP_H__20130204
#define __YUMEI_SMAP_H__20130204


typedef struct yumei_smap_s yumei_smap_t;

typedef struct yumei_smap_i_s yumei_smap_i_t;
typedef struct yumei_smap_ii_s yumei_smap_ii_t;

struct yumei_smap_i_s
{
	int length;
	int num;
	yumei_smap_ii_t* items;
};

struct yumei_smap_ii_s
{
	char* name;
	char* value;
	int next_t;
	yumei_smap_i_t*  parent;
	yumei_smap_ii_t* prev;
	yumei_smap_ii_t* next;
};

struct yumei_smap_s
{
	int num;
	yumei_pool_t* pool;
	yumei_smap_i_t** items;
};

yumei_smap_t* yumei_smap_alloc( yumei_pool_t* pool );
int yumei_smap_insert( yumei_smap_t* smap, const char* name, const char* value );
yumei_smap_ii_t* yumei_smap_get( yumei_smap_t* smap, const char* name );



#endif //__YUMEI_SMAP_H__20130204