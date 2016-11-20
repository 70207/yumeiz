#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "yumei_types.h"
#include "yumei_pool.h"
#include "yumei_chain.h"
#include "yumei_mutex.h"
#include "yumei_listen.h"
#include "yumei_socket.h"
#include "yumei_conn.h"
#include "yumei_event.h"

int yumei_send( int fd, yumei_uchar_t* buffer, int len, int* sent );
int yumei_send_conn_chain( int fd, yumei_chain_t* chain );

int yumei_write( yumei_conn_t* conn )
{
	int ret;
	int fd;
	yumei_chain_t* chain;

	fd = conn->socket->fd;

	printf( "fd:%d to write,chain:%llx\n", fd, conn->send_chain );
	yumei_mutex_lock( conn->mutex );
	chain = yumei_chain_get_next( conn->send_chain );
	conn->send_chain = chain;
	yumei_mutex_unlock( conn->mutex );

	if( !chain )
	{
		printf( "func:%s, fd:%d not chain\n",__FUNCTION__, fd );
		return -1;
	}


	while( chain )
	{
		if( chain->first_set )
		{
			break;
		}

		ret = yumei_send_conn_chain(  fd, chain );
		if( ret )
		{
			if( ret < 0 )
			{
				printf( "fd:%d write back wrong to close\n", fd );
				conn->state = YUMEI_CONN_CLOSE;
				yumei_event_del( conn->event, conn->info_index );
			}

			return ret;
		}

		chain = chain->next;
		
	}

	
	
	return 0;
}

int yumei_send_conn_chain( int fd, yumei_chain_t* chain )
{

	int len;
	int ret;
	int sent;
	int io_pos;

	io_pos = chain->io_pos;
	len = chain->io_len;
	len -= io_pos;
	if( len <= 0 )
	{
		return YUMEI_CONN_IO_OK;
	}

	printf( "fd:%d, %s,len:%d\n", fd, __FUNCTION__, len );
	sent = 0;
	ret = yumei_send( fd, chain->buf + io_pos,  len, &sent );
	switch( ret )
	{
	case YUMEI_CONN_IO_AGAIN:
		io_pos += sent;
		ret = 0;
		break;
	case YUMEI_CONN_IO_OK:
		ret = 0;
	case YUMEI_CONN_IO_FAIL:
		io_pos += sent;
		break;
	}

	chain->io_pos = io_pos;

	return ret;
}

int yumei_send( int fd, yumei_uchar_t* buffer, int len, int* sent )
{
	int ret;
	int err;
	int lsent;

	lsent = 0;
	while( lsent < len )
	{
	   printf( "yumei_send\n");
	   ret =  send( fd, buffer + lsent, len - lsent, 0 );
	   if( ret < 0 )
	   {
		
		err = errno;
		if( err == EAGAIN )
		{
			printf( "fd:%d send again\n", fd);
			return YUMEI_CONN_IO_AGAIN;
		}
		else if( err == EINTR )
		{
			printf( "fd:%d send intr\n", fd);
			continue;
		}
		else
		{
			printf( "fd:%d send wrong:%d\n", fd, err);
		        return YUMEI_CONN_IO_FAIL;
		}
	  }

	  printf( "fd:%d, send len:%d\n", fd, ret );
	
	  lsent += ret;
	  *sent = lsent;
	}
	
	return YUMEI_CONN_IO_OK;
}

