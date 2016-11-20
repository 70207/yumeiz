#include <stdio.h>
#include <stdlib.h>

#include "yumei_types.h"
#include "yumei_pool.h"
#include "yumei_chain.h"
#include "yumei_listen.h"
#include "yumei_socket.h"
#include "yumei_types.h"
#include "yumei_conn.h"
#include "yumei_event.h"
#include "yumei_event_timer.h"

int g_conn_f = 0;

int yumei_conn_finish( yumei_conn_t* conn )
{
	if( conn->finish )
	{
		conn->finish( conn );
	}

	printf( "finish conn:%llx\n", conn );
	
	printf( "FUNC:%s, socket:%d, g_conn:%d\n", __FUNCTION__, conn->socket->fd, g_conn_f++ );

	yumei_mutex_close( conn->mutex );
	yumei_free( conn->read_buf );
	yumei_socket_close( conn->socket );
	yumei_pool_free( conn->pool );
	yumei_chain_free( conn->send_chain );

	free( conn );
	return 0;
}

int yumei_conn_set_time( yumei_conn_t* conn, time_t start, time_t expire )
{
	if( !conn )
	{
		return -1;
	}

	conn->start = start;
	conn->expire = expire;

	return 0;
}

int yumei_conn_keep_alive( yumei_conn_t* conn )
{
		
	conn->state |= YUMEI_CONN_KEEP_ALIVE;
	
	return 0;
}

int yumei_conn_miss_alive( yumei_conn_t* conn )
{
	conn->state &= (~YUMEI_CONN_KEEP_ALIVE);
	return 0;
}

int yumei_conn_check_alive( yumei_conn_t* conn )
{
	yumei_msec_t current;
	yumei_msec_t interval;
	
	return -1;
	current = yumei_get_current_timer();
	interval = current - conn->last_read;
	
	if( conn->iskeepalive )
	{
		if( interval < 10 )
		{
			return -1;
		}
	}
	
	return 0;
}

yumei_conn_t* yumei_conn_create()
{
	int size;
	yumei_mutex_t* mutex;
	yumei_pool_t* pool;
	yumei_mem_t* mem;
	yumei_chain_t* chain;

	size = sizeof( yumei_conn_t );
	yumei_conn_t* conn = ( yumei_conn_t* )malloc( size );
	memset( conn, 0, size );

	pool = yumei_pool_alloc( YUMEI_POOL_RESERVE_SIZE );

	conn->pool = pool;
	mutex = yumei_mutex_create( pool );
	conn->mutex = mutex;

	mem = yumei_malloc( pool, YUMEI_CONN_READ_BUFFER );
	conn->read_buf = mem;

	conn->send_chain = 0;

	return conn;
}



