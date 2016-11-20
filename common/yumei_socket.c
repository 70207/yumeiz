#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <malloc.h>


#include "yumei_types.h"
#include "yumei_pool.h"
#include "yumei_listen.h"
#include "yumei_socket.h"

#define YUMEI_SOCKET_SEND_BUF_RESERVE (128*1024)
#define YUMEI_BACK_LOG 511


int yumei_socket_init( yumei_socket_t* socket, int fd )
{
	socket->fd = fd;
	return 0;
}

int yumei_socket_close( yumei_socket_t* socket )
{
	close( socket->fd );
	socket->fd = -1;
}



yumei_socket_t* yumei_socket_create_by_listen( yumei_pool_t* pool, yumei_listen_t*  yumei_listen )
{
	yumei_socket_t* sock;
	struct sockaddr_in addr;
	int fd;
	int size;
	int flag;

	size = sizeof( yumei_socket_t );
	sock = ( yumei_socket_t* ) yumei_raw_malloc( pool, size );

	fd = socket( AF_INET, SOCK_STREAM, 0 );
	addr.sin_family = AF_INET;
	addr.sin_port = htons( yumei_listen->port );
	addr.sin_addr.s_addr = yumei_listen->ip;
	size = sizeof( struct sockaddr );
	
	flag = 1;	
	ioctl( fd, FIONBIO, &flag );

	flag = bind( fd, ( struct sockaddr*)&addr, size );
	if( flag == -1 )
	{
		printf( "error is %s", strerror(errno) );
	}
	flag = listen( fd, YUMEI_BACK_LOG );
	sock->fd = fd;	
	sock->type = YUMEI_SOCKET_LISTEN;
	return sock;
}

yumei_socket_t* yumei_socket_create( yumei_pool_t* pool, int fd )
{
	int size;
	yumei_socket_t* sock;

	size = sizeof( yumei_socket_t );
	
	sock = ( yumei_socket_t* )yumei_raw_malloc( pool, size );
	sock->fd = fd;

	return sock;
}


int yumei_socket_no_block( yumei_socket_t* socket )
{
	int flag;
	int fd;
	int send_buf_size;
	int len;

	fd = socket->fd;
//	flag = 1;	
//	ioctl( fd, FIONBIO, &flag );
	
	flag = fcntl( fd, F_GETFL );
	flag |= O_NONBLOCK;
	fcntl( fd, F_SETFL, flag );

	len = sizeof( send_buf_size );	
	flag = getsockopt( fd, SOL_SOCKET, SO_SNDBUF, &send_buf_size, &len );
	if( send_buf_size < YUMEI_SOCKET_SEND_BUF_RESERVE )
	{
		send_buf_size = YUMEI_SOCKET_SEND_BUF_RESERVE;
		setsockopt( fd, SOL_SOCKET, SO_SNDBUF, &send_buf_size, &len );
	}

	struct timeval time;
	time.tv_sec = 0;
	time.tv_usec = 50;

	setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &time, sizeof(time));
	setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &time, sizeof(time));
	return 0;
}
