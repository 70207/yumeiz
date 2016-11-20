#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/epoll.h>

#include "yumei_types.h"
#include "yumei_event_timer.h"
#include "yumei_pipe.h"
#include "yumei_pool.h"
#include "yumei_chain.h"
#include "yumei_conn.h"
#include "yumei_event.h"
#include "yumei_event_epoll.h"



int yumei_event_epoll_task( yumei_event_t* event, int info_index, int type );

int yumei_event_epoll_init( yumei_event_t* event )
{
	int epoll_fd;
	int size;
	yumei_event_info_t* event_info;
	struct epoll_event ee;
	struct epoll_event* events;

	epoll_fd = epoll_create( YUMEI_EVENTS_NUM_MAX );

	event->fd = epoll_fd;

	size = YUMEI_EVENTS_NUM_MAX * sizeof( struct epoll_event );
	events = ( struct epoll_event* ) malloc( size );
	memset( events, 0, size );

	size = YUMEI_EVENTS_NUM_MAX * sizeof( yumei_event_info_t );
	event_info = ( yumei_event_info_t* ) malloc( size );
	memset( event_info, 0, size );
	

	event->event_info = event_info;
	event->data = events;
	event->wait_time =  YUMEI_EVENT_WAIT_INFINITE;	

	return 0;

}

int yumei_event_epoll_do( yumei_event_t* event )
{
	struct epoll_event* events;
	int e_num;
	yumei_msec_t wait_time;
	int info_index;
	int i;
	yumei_msec_t current;

	wait_time = event->wait_time;
	current = yumei_get_current_timer();
	events = ( struct epoll_event* )event->data;
	e_num = epoll_wait( event->fd, events, YUMEI_EVENTS_NUM_MAX, wait_time );
	
	event->wait_time = yumei_event_find_timer( wait_time, e_num );

	for( i = 0; i < e_num; ++i )
	{
		info_index = ( yumei_conn_t* ) events[ i ].data.u32;
		printf( "event %d epoll task\n", event->index );
		yumei_event_epoll_task( event, info_index, events[ i ].events );
		
	}


	return 0;
}

int yumei_event_epoll_task( yumei_event_t* event, int info_index, int type )
{
	int event_type;

	if( type & ( EPOLLHUP | EPOLLERR ) )
	{
		event_type = YUMEI_EVENT_ERROR;
	}
	else 
	{
		if( type & EPOLLOUT )
		{
			event_type = YUMEI_EVENT_OUT;
		}

		if( type & EPOLLIN )
		{
			event_type = YUMEI_EVENT_IN;
		}
	}
	

	yumei_event_task( event, info_index, event_type );

	return 0;
}

int yumei_event_epoll_ctl( int epfd, int ctl, int info_index, yumei_event_info_t* info )
{
	int fd;
	int type;
	int eptype;
	int cttype;
	struct epoll_event ee;


	eptype = 0;
	cttype = 0;
	fd = info->fd;
	type = info->type;

	if( type == YUMEI_EVENT_ACCEPT )
	{
		eptype = EPOLLIN;
	}
	if( type & YUMEI_EVENT_IN )
	{
		eptype |= EPOLLIN;
	}
	if( type & YUMEI_EVENT_OUT )
	{
		eptype |= EPOLLOUT;
	}
	if( type & YUMEI_EVENT_ET )
	{
		eptype |= EPOLLET;
	}


	switch( ctl )
	{
	case YUMEI_EVENT_ADD:
		cttype = EPOLL_CTL_ADD;
		break;
	case YUMEI_EVENT_MOD:
		cttype = EPOLL_CTL_MOD;
		break;
	case YUMEI_EVENT_DEL:
		cttype = EPOLL_CTL_DEL;
		break;
	}

	ee.data.u32 = info_index;
	ee.events = eptype;

	int flag = epoll_ctl( epfd, cttype, fd, &ee );

	return 0;

}
