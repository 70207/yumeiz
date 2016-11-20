#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "yumei_types.h"
#include "yumei_pool.h"
#include "yumei_chain.h"
#include "yumei_mutex.h"
#include "yumei_conn.h"
#include "yumei_read.h"

#define YUMEI_READ_BUFFER (3*1024)
#define YUMEI_READ_PER 1024


int yumei_read( yumei_conn_t* conn )
{
	int recvd;
	int ret;
	int err;
	yumei_mem_t* mem;
	int length;
	int lenper;
	int fd;
	
	length = YUMEI_READ_BUFFER;
	lenper = YUMEI_READ_PER;

	mem = conn->read_buf;
	fd = conn->socket->fd;

	if( !conn->pending_state )
	{
		mem->pos = mem->start;
	}

	recvd = mem->pos - mem->start;
	
	while( recvd < length )
	{
		ret = recv( conn->socket->fd, mem->pos, lenper, 0 );
		if( ret < 0 )
		{
			
			err = errno;
			if( err == EAGAIN )
			{
				printf( "fd:%d recv read pending\n", fd );
				return YUMEI_CONN_IO_AGAIN;
			}
			else if( err == EINTR )
			{
				printf( "fd:%d recv eintr\n", fd );
				continue;
			}
			
			printf( "fd:%d recv fail\n", fd );
			return YUMEI_CONN_IO_FAIL;

		}

		mem->pos += ret;
		recvd += ret;
		if( ret < lenper )
		{
			printf( "fd:%d last recv len:%d\n", fd, ret );
			break;
		}
	}
	
	printf( "fd:%d,recvd:%d, reserve length:%d\n", fd, recvd,length );
	if( recvd == 0 )
	{
		return YUMEI_CONN_IO_NULL;
	}

	return YUMEI_CONN_IO_OK;
}
