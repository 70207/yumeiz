#include <malloc.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <assert.h>

#include "yumei_types.h"
#include "yumei_pool.h"
#include "yumei_chain.h"
#include "yumei_listen.h"
#include "yumei_conn.h"
#include "yumei_accept.h"
#include "yumei_conn.h"
#include "yumei_pipe.h"
#include "yumei_mutex.h"

#include "yumei_event.h"


#define YUMEI_EVENT_THREAD_STATCK_SIZE (256*1024)


int yumei_event_main_events_create();

yumei_event_main_t main_event;

int yumei_event_main_init()
{
	int size;
	yumei_pool_t* pool;

	pool = yumei_pool_alloc( YUMEI_POOL_RESERVE_SIZE );

	size = sizeof( yumei_event_main_t );
	memset( &main_event, 0, size );

	main_event.mem_pool = pool;

	return 0;
}



int yumei_event_create_threads()
{
	yumei_event_pool_t* pool;
	pthread_attr_t attr;
	int i,j;
	
	pthread_attr_init( &attr );

	pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );
	pthread_attr_setstacksize( &attr, YUMEI_EVENT_THREAD_STATCK_SIZE );

	sigset_t signal_mask;
	sigemptyset (&signal_mask);
	sigaddset (&signal_mask, SIGPIPE);
	pthread_sigmask (SIG_BLOCK, &signal_mask, NULL);

		
	for( i = 0; i < main_event.listen_num; ++i )
	{
		pool = main_event.event_pools[ i ];
		for( j = 0; j < pool->event_num; ++j )
		{		
			pool->events[ j ]->thread_run = 1;
			pthread_create( pool->child_thread_id + j, &attr, yumei_event_proc, pool->events[j] );
		}
	}

	pthread_attr_destroy( &attr );


	return 0;
}

int yumei_event_wait_threads()
{
	int i,j;
	yumei_event_pool_t* pool;
	
	for( i = 0; i < main_event.listen_num; ++i )
	{
		pool = main_event.event_pools[ i ];
		for( j = 0; j < pool->event_num; ++j )
		{
			pthread_join( pool->child_thread_id[ j ], 0 );
		}
	}
	return 0;
}

int yumei_event_create_events()
{
	yumei_event_t** events;
	yumei_event_t*  event;
	yumei_event_pool_t* pool;
	int i,j;

	yumei_event_main_events_create();

	
	for( i = 0; i < main_event.listen_num; ++i )
	{
		pool = main_event.event_pools[ i ];
		events = pool->events;
		for( j = 0; j < pool->event_num; ++j )
		{
			event = events[ j ];
			yumei_event_init( event );
			yumei_event_add( event, pool->socket, YUMEI_EVENT_ACCEPT );
		}
	}
	
	return 0;
}



int yumei_event_main_events_create()
{
	int listen_num;
	int size;
	yumei_listen_t*		listen;
	yumei_socket_t*		 socket;
	yumei_event_pool_t** pools;
	yumei_event_pool_t*  pool;
	yumei_pool_t*        m_pool;
	yumei_pool_t*		 e_pool;
	yumei_event_t**      events;
	yumei_event_t*       event;
	yumei_mutex_t*		 mutex;

	int id;
	int i,j;

	m_pool = main_event.mem_pool;

	listen_num = yumei_listen_get_num();
	if( !listen_num )
	{
		return -1;
	}

	size = listen_num * sizeof( yumei_event_pool_t* );
	pools = ( yumei_event_pool_t** ) yumei_raw_malloc( m_pool, size );

	id = 0;
	for( i = 0; i < listen_num; ++i )
	{
		listen = yumei_listen_get( i );
		size = sizeof( yumei_event_pool_t );
		pool = ( yumei_event_pool_t* ) yumei_raw_malloc( m_pool, size );

		socket = yumei_socket_create_by_listen( m_pool, listen );
		pool->event_num = listen->thread_num;
		pool->socket = socket;
		pool->listen = listen;

		size = sizeof( yumei_pthread_t ) * pool->event_num;
		pool->child_thread_id = ( yumei_pthread_t* )yumei_raw_malloc( m_pool, size );
		pools[ i ] = pool;
	
		size = sizeof( yumei_event_t* ) * pool->event_num;
		events = ( yumei_event_t** )yumei_raw_malloc( m_pool, size );
		size = sizeof( yumei_event_t );
		for( j = 0; j < pool->event_num; ++j )
		{
			e_pool = yumei_pool_alloc( YUMEI_POOL_RESERVE_SIZE );
			event = ( yumei_event_t* )yumei_raw_malloc( e_pool, size );
			memset( event, 0, size );
			event->pool = e_pool;

			event->index = j;
			event->id = id++;
			event->yumei_accept = yumei_accept;
			events[ j ] = event;

			mutex = yumei_mutex_create( e_pool );
			event->mutex = mutex;

		}

		pool->events = events;

	}
	main_event.listen_num = listen_num;
	main_event.event_pools = pools;

	return 0;
}

