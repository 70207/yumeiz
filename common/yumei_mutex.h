#ifndef __YUMEI_MUTEX_H__
#define __YUMEI_MUTEX_H__

typedef struct yumei_mutex_s yumei_mutex_t;

struct yumei_mutex_s
{
	void* pmutex;
};


yumei_mutex_t* yumei_mutex_create( yumei_pool_t* pool );
int yumei_mutex_close( yumei_mutex_t* mutex );
int yumei_mutex_lock( yumei_mutex_t* mutex );
int yumei_mutex_unlock( yumei_mutex_t* mutex );



#endif //__YUMEI_MUTEX_H__