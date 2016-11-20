#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "yumei_types.h"
#include "yumei_pool.h"
#include "yumei_chain.h"
#include "yumei_listen.h"
#include "yumei_socket.h"
#include "yumei_conn.h"
#include "yumei_accept.h"
#include "yumei_event.h"
#include "yumei_read.h"
#include "yumei_write.h"
#include "yumei_http.h"

int yumei_accept( yumei_conn_t* conn )
{
	int fd;
	int socket_fd;
	int sock_len;
	int size;
	time_t ticket;
	struct sockaddr addr;
	struct sockaddr_in addr_in;
	const char* ip;
	int port;
	yumei_socket_t* sock;
	yumei_pool_t* pool;
	yumei_mem_t* mem;
	yumei_event_t* event;
	
	if( !conn )
	{
		return YUMEI_ERROR;
	}

	fd = conn->socket->fd;
	event = conn->event;

	sock_len = sizeof( struct sockaddr );
	socket_fd = accept4( fd, &addr, &sock_len, SOCK_NONBLOCK );
	if( socket_fd <= 0 )
	{
		return -1;
	}

	memcpy( &addr_in, &addr, sock_len );
	ip = inet_ntoa( addr_in.sin_addr );
	port = addr_in.sin_port;
	printf( "accept socket:%d,ip:%s,port:%d\n", socket_fd, ip, port );

	conn = yumei_conn_create();
	pool = conn->pool;

	sock = yumei_socket_create( pool, socket_fd );
	sock->addr = addr;
	sock->socklen = sock_len;
	yumei_socket_no_block( sock );


	printf( "fd:%d accept,pool:%llx,conn %llx\n", socket_fd, pool, conn );	


	conn->ihandle = yumei_http_init;
	conn->ohandle = yumei_http_send;
	conn->rev = yumei_read;
	conn->wev = yumei_write;
	conn->finish = 0;
	conn->socket = sock;

	yumei_event_add_conn( event, conn );

	return 0;

}
