#include <sys/epoll.h>

#include "yumei_types.h"
#include "yumei_pool.h"
#include "yumei_chain.h"
#include "yumei_event_timer.h"
#include "yumei_pipe.h"
#include "yumei_conn.h"
#include "yumei_event.h"
#include "yumei_event_main.h"
#include "yumei_event_epoll_main.h"


struct kevent main_events[ YUMEI_EVENTS_MAIN_NUM_MAX ];

int yumei_event_epoll_main_init( yumei_event_main_t* event_main )
{
	int kqfd;
	int socket_fd;
	yumei_event_pool_t** pools;
	yumei_event_pool_t*  pool;
	int i;

	pools = event_main->pools;

	kqfd = kqueue();

	event_main->fd = kqfd;

	for( i = 0; i < event_main->listen_num; ++i )
	{
		pool = pools[ i ];
		ee.data.ptr = pool;
		socket_fd = pool->socket->fd;
		
		EV_SET( main_events + i, socket_fd, EVFILT_READ, EV_ADD, 0, 0, NULL );
	}

	kevent( kqfd, main_events, i, 0, 0, 0 );

	event_main->wait_time = YUMEI_EVENT_WAIT_INFINITE;
	return 0;

}

int yumei_event_epoll_main_do( yumei_event_main_t* event_main )
{
	int flag;
	int e_num;
	yumei_msec_t wait_time;
	int event_pipe;
	int i;
	yumei_event_pool_t* pool;
	yumei_pipe_t yp;
	yumei_conn_t* conn;
	int index;
	int event_num;
	int fd;

	
	wait_time = 60000;//event_main->wait_time;
	e_num = //epoll_wait( event_main->fd, main_events, YUMEI_EPOLL_MAIN_EVENTS_MAX, wait_time );
	kevent( event_main->fd, 0, 0, main_events, YUMEI_EVENTS_MAIN_NUM_MAX, wait_time );

	event_main->wait_time = yumei_event_find_timer( wait_time, e_num );

	for( i = 0; i < e_num; ++i )
	{
		pool = ( yumei_event_pool_t* ) main_events[ i ].data.ptr;
		event_num = pool->event_num;
		conn = event_main->yumei_accept( pool->socket->fd );
		fd = conn->socket->fd;
		index = fd % event_num;
		event_pipe = pool->child_pipe[ index ];
		conn->event_pipe = event_pipe;
		yp.fd = pool->socket->fd;
		yp.data = conn;
		yp.type = YUMEI_PIPE_ACCEPT;
		yp.idata = -1;	
		
		write( event_pipe, &yp, sizeof( yumei_pipe_t ) );
	}


	return 0;

}
