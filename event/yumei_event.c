#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#include "yumei_types.h"
#include "yumei_pipe.h"
#include "yumei_event_timer.h"
#include "yumei_pool.h"
#include "yumei_chain.h"
#include "yumei_conn.h"
#include "yumei_accept.h"
#include "yumei_event.h"
#include "yumei_http.h"
#include "yumei_event_epoll.h"




int yumei_event_init( yumei_event_t* event )
{
	return yumei_event_epoll_init( event );
}

int yumei_event_proc( void* data )
{
	yumei_event_t* event = ( yumei_event_t* )data;
	while( event->thread_run )
	{
		yumei_event_epoll_do( event );
		yumei_event_check_time( event );
	}

	return 0;
}



int yumei_event_task( yumei_event_t* event, int info_index, int type )
{
	int flag;
	int rtn;
	int fd;
	int index;
	int pending;
	yumei_conn_t* conn;
	yumei_event_info_t* info;
	
	info = event->event_info + info_index;
	conn = info->conn;
	fd = conn->socket->fd;
	rtn = 0;

	if( info->fd <= 0 )
	{
		return 0;
	}

	if(  info->isaccept )
	{
		conn->ihandle( conn );
		return 0;
	}

	switch( type )
	{
	case YUMEI_EVENT_ERROR:
		printf( "fd:%d,event:%d to close conn\n", fd, index );
		yumei_event_del( event, info_index );
		break;
	case YUMEI_EVENT_OUT:
		printf( "fd: %d, to write\n", fd );
		rtn = conn->ohandle( conn );
		break;
	case YUMEI_EVENT_IN:
		yumei_event_check_alive( event, info_index );
		conn->state = YUMEI_CONN_KEEP_ALIVE;
		rtn = conn->ihandle( conn );
		break;
	}

	switch( rtn )
	{
	case YUMEI_CONN_IO_FAIL:
		yumei_conn_miss_alive( conn );
		info->iskeepalive = 0;
		break;
	case YUMEI_CONN_IO_AGAIN:
		yumei_event_mod_not_type( event, info_index, type );
		break;
	}

	return 0;
}


int yumei_event_add( yumei_event_t* event, yumei_socket_t* socket, int type  )
{
		int empty_info_index;
		int i;
		yumei_event_info_t* event_info;
		yumei_conn_t* conn;
		yumei_msec_t current;

		if( !event || !socket )
		{
			return YUMEI_ERROR;
		}

	
		empty_info_index = event->empty_info_index;
		for( i = empty_info_index + 1; i < YUMEI_EVENTS_NUM_MAX; ++i )
		{
			if( event->event_info[ i ].fd == 0 )
			{
				event->empty_info_index = i;
				break;
			}
		}

		if( i == YUMEI_EVENTS_NUM_MAX )
		{
			printf( "event:%d info not more!\n", event->index );
		}

		current = yumei_get_current_timer();

		conn = yumei_conn_create();
		conn->socket = socket;
		conn->info_index = empty_info_index;
		conn->mutex = event->mutex;
		conn->event = event;

		event_info = event->event_info + empty_info_index;
		event_info->fd = socket->fd;
		event_info->start = current;
		event_info->expire = current + YUMEI_EVENT_TIME_OUT;
		event_info->used = 1;
		event_info->conn = conn;
		event_info->type = type;
		event_info->iskeepalive = 0;
	
		if( YUMEI_SOCKET_LISTEN == socket->type )
		{
			conn->ihandle = yumei_accept;
			event_info->isaccept = 1;
		}
		else
		{
			conn->ihandle = yumei_http_init;
		}

		yumei_event_epoll_ctl( event->fd, YUMEI_EVENT_ADD, empty_info_index, event_info );
		
		return 0;
}

int yumei_event_add_conn( yumei_event_t* event, yumei_conn_t* conn )
{
		int empty_info_index;
		int i;
		yumei_event_info_t* event_info;
		yumei_msec_t current;
		int fd;

		if( !event || !conn )
		{
			return YUMEI_ERROR;
		}

	
		empty_info_index = event->empty_info_index;
		for( i = empty_info_index + 1; i < YUMEI_EVENTS_NUM_MAX; ++i )
		{
			if( event->event_info[ i ].fd == 0 )
			{
				event->empty_info_index = i;
				break;
			}
		}

		if( i == YUMEI_EVENTS_NUM_MAX )
		{
			printf( "event:%d info not more!\n", event->index );
		}

		current = yumei_get_current_timer();
		fd = conn->socket->fd;
		conn->info_index = empty_info_index;
		conn->mutex = event->mutex;
		conn->event = event;
		event_info = event->event_info + empty_info_index;
		event_info->fd = fd;
		event_info->start = current;
		event_info->expire = current + YUMEI_EVENT_TIME_OUT;
		event_info->used = 1;
		event_info->conn = conn;
		event_info->type = YUMEI_EVENT_IN | YUMEI_EVENT_ET;
		event_info->iskeepalive = 0;
	
		
		yumei_event_epoll_ctl( event->fd, YUMEI_EVENT_ADD, empty_info_index, event_info );
		
		return 0;
}

int yumei_event_del( yumei_event_t* event,  int info_index )
{
	
	int flag;
	int fd;
	yumei_conn_t* conn;
	yumei_event_info_t* event_info;

	event_info = event->event_info + info_index;
	if( event_info->fd <= 0 )
	{
		return 0;
	}

	conn = event_info->conn;
	
	if( YUMEI_CONN_CLOSE != conn->state )
	{
		yumei_event_epoll_ctl( event->fd, YUMEI_EVENT_DEL,  info_index, event_info );
	}
	printf( "fd:%d to del from event:%d, flag:%d\n", fd, event->index,flag );

	event_info->fd = 0;
	if( info_index < event->empty_info_index )
	{
		event->empty_info_index = info_index;
	}

	yumei_conn_finish( conn );
	return 0;
}

int yumei_event_check_time( yumei_event_t* event )
{
	yumei_event_info_t* event_info;
	int i;
	yumei_msec_t current;

	current = yumei_get_current_timer();
	
	for( i = 0; i < YUMEI_EVENTS_NUM_MAX; ++i )
	{
		event_info = event->event_info + i;
		if( event_info->isaccept )
		{
			continue;
		}

		if( !event_info->fd )
		{
			continue;
		}
		
		if( event_info->iskeepalive )
		{
			continue;
		}


		if( current > event_info->expire )
		{
			printf( "fd:%d to del, current:%llu, expire:%llu\n", event_info->fd, current, event_info->expire );
			yumei_event_del( event, i );
		}

	}

	return 0;
}

int yumei_event_check_alive( yumei_event_t* event, int info_index )
{
	yumei_event_info_t* info;
	yumei_conn_t* conn;

	info = event->event_info + info_index;
	conn = info->conn;

	if( !(conn->state & YUMEI_CONN_KEEP_ALIVE ) ) 
	{
		info->iskeepalive = 0;
		return 0;
	}

	
	if( conn->iskeepalive )
	{
		return 0;
	}
	
	if( event->alivenum > 29 )
	{
		return 0;
	}	
	
	++event->alivenum;
	info->iskeepalive = 1;
	conn->iskeepalive = 1;

	return 0;
}

int yumei_event_mod_type( yumei_event_t* event, int info_index, int type )
{
	yumei_event_info_t* event_info;
	event_info = event->event_info + info_index;
 	event_info->type |= type;

	yumei_event_epoll_ctl( event->fd, YUMEI_EVENT_MOD, info_index, event_info );

	return 0;
}


int yumei_event_mod_not_type( yumei_event_t* event, int info_index, int type )
{
	
	yumei_event_info_t* event_info;

	event_info = event->event_info + info_index;
 	event_info->type &= (~type);

	yumei_event_epoll_ctl( event->fd, YUMEI_EVENT_MOD,  info_index, event_info );

	return 0;
}

int yumei_event_out_re( yumei_event_t* event, int info_index )
{
	struct epoll_event ee;
	yumei_event_info_t* event_info;
	yumei_conn_t* conn;
	yumei_chain_t* chain;
	int fd;
	
	event_info = event->event_info + info_index;
	if( !event_info->fd )
	{
		return -1;
	}

	conn = event_info->conn;
	chain = conn->send_chain;

	yumei_chain_clear_io( chain );

	return yumei_event_mod_type( event, info_index, YUMEI_EVENT_OUT );
}
