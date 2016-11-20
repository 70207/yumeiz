#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/event.h>

#include "yumei_types.h"
#include "yumei_event_timer.h"
#include "yumei_pipe.h"
#include "yumei_pool.h"
#include "yumei_chain.h"
#include "yumei_conn.h"
#include "yumei_event.h"
#include "yumei_event_kqueue.h"




int yumei_event_kq_init( yumei_event_t* event )
{
	int kq_fd;
	int size;
	yumei_event_info_t* event_info;
	struct kevent ke;
	struct kevent* events;

	kq_fd = kqueue();

	event->fd = kq_fd;

	
	EV_SET( &ke, event->parent_pipe, EVFILT_READ, EV_ADD, 0, 0, NULL );
	kevent( kq_fd, &ke, 1, NULL, 0, NULL );

	size = YUMEI_EVENTS_NUM_MAX * sizeof( struct kevent );
	events = ( struct kevent* ) malloc( size );
	memset( events, 0, size );

	size = YUMEI_EVENTS_NUM_MAX * sizeof( yumei_event_info_t );
	event_info = ( yumei_event_info_t* ) malloc( size );
	memset( event_info, 0, size );
	

	event->event_info = event_info;
	event->data = events;
	event->wait_time =  YUMEI_EVENT_WAIT_INFINITE;	

	return 0;

}

int yumei_event_kq_do( yumei_event_t* event )
{
	struct kevent* events;
	int e_num;
	yumei_msec_t wait_time;
	int info_index;
	int i;
	yumei_msec_t current;

	wait_time = event->wait_time;
	current = yumei_get_current_timer();
	events = ( struct kevent* )event->data;
	e_num = kevent( event->fd, 0, 0, events, YUMEI_EVENTS_NUM_MAX, wait_time );
	
	event->wait_time = yumei_event_find_timer( wait_time, e_num );

	for( i = 0; i < e_num; ++i )
	{
		if( events[ i ].data.fd == event->parent_pipe )
		{
			printf( "event %d parent task\n", event->index );
			yumei_event_parent_task( event, current );
		}
		else
		{
			info_index = ( yumei_conn_t* ) events[ i ].data.u32;
			printf( "event %d epoll task\n", event->index );
			yumei_event_task( event, info_index, events[ i ].events );
		}

	}



	return 0;
}






int yumei_event_kq_ctl( int epfd, int ctl, int info_index, yumei_event_info_t* info )
{
	int fd;
	int type;
	int eptype;
	int cttype;
	struct event_epoll ee;


	eptype = 0;
	cttype = 0;
	fd = info->fd;
	type = info->state;

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

	epoll_ctl( epfd, cttype, fd, &ee );

	return 0;

}


