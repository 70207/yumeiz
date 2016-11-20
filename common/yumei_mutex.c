#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "yumei_types.h"
#include "yumei_pool.h"
#include "yumei_mutex.h"


yumei_mutex_t* yumei_mutex_create( yumei_pool_t* pool )
{
	int size;
	yumei_mutex_t* mutex;
	pthread_mutex_t* pmutex;

	size = sizeof( pthread_mutex_t );
	pmutex = ( pthread_mutex_t* ) yumei_raw_malloc( pool, size );
	
	size = sizeof( yumei_mutex_t );
	mutex = ( yumei_mutex_t* ) yumei_raw_malloc( pool, size );
	memset( mutex, 0, size );
	mutex->pmutex = pmutex;

	pthread_mutex_init( pmutex, 0 );

	return mutex;
}

int yumei_mutex_close( yumei_mutex_t* mutex )
{
	if( !mutex )
	{
		return 0;
	}
	pthread_mutex_t* pmutex;
	pmutex = mutex->pmutex;

	pthread_mutex_destroy( pmutex );
	return 0;
}

int yumei_mutex_lock( yumei_mutex_t* mutex )
{
	pthread_mutex_t* pmutex;
	pmutex = mutex->pmutex;
	pthread_mutex_lock( pmutex );
	return 0;
}

int yumei_mutex_unlock( yumei_mutex_t* mutex )
{
	pthread_mutex_t* pmutex;
	pmutex = mutex->pmutex;

	pthread_mutex_unlock( pmutex );
	return 0;
}

