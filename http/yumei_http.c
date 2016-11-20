#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "yumei_types.h"
#include "yumei_pool.h"
#include "yumei_chain.h"
#include "yumei_pipe.h"
#include "yumei_conn.h"
#include "yumei_read.h"
#include "yumei_http.h"
#include "yumei_http_parse.h"
#include "yumei_busi.h"
#include "yumei_busi_com.h"

int yumei_http_do( yumei_http_t* http );


int yumei_http_init( yumei_conn_t* conn )
{
	int rtn;
	int i;
	yumei_http_t* http;
	yumei_mem_t* mem;
	int size;
	int flag;

	printf( "FUNC:%s\n", __FUNCTION__);
	
	rtn = conn->rev( conn );
	switch( rtn )
	{
	case YUMEI_CONN_IO_FAIL:
		 printf( "conn read fail, %llx\n", conn );
		return rtn;
	case YUMEI_CONN_IO_AGAIN:
		conn->pending_state = 1;
		return rtn;
	case YUMEI_CONN_IO_OK:
		conn->pending_state = 0;
		break;
	case YUMEI_CONN_IO_NULL:
		conn->state = YUMEI_CONN_CLOSE;
		yumei_event_del( conn->event, conn->info_index );
		return YUMEI_CONN_IO_OK;
	}

	printf( "end of read\n" );

	http = yumei_http_alloc( conn );
	yumei_http_parse( http );
	yumei_http_do( http );

	return YUMEI_CONN_IO_OK;
	
}


int yumei_http_do( yumei_http_t* http )
{
	yumei_conn_t* conn;
	yumei_pool_t* pool;
	yumei_mem_t* mem;
	yumei_busi_t* busi;
	yumei_pipe_t pp;
	int size;


	conn = http->conn;
	pool = http->pool;

	busi = ( yumei_busi_t* )http->data;

	busi->busi_state = YUMEI_BUSI_TO_HANDLE;
	busi->handle = yumei_busi_request;

	for( ;; )
	{
		if( busi->busi_state == YUMEI_BUSI_TO_SEND )
		{
			conn->wev( conn );
			break;
		}

		if( busi->busi_state != YUMEI_BUSI_TO_HANDLE )
		{
			break;
		}

		busi->handle( busi );
	}
	
	return 0;
}


yumei_http_t* yumei_http_alloc( yumei_conn_t* conn )
{
	yumei_pool_t* pool;
	yumei_mem_t* mem;
	yumei_http_t* http;
	yumei_http_request_t* request;
	yumei_http_header_t* header;
	yumei_http_cookie_t* cookie;
	yumei_http_cookie_t* mod_cookie;
	yumei_smap_t* smap;
	yumei_busi_t* busi;
	int size;

	pool = yumei_pool_alloc( YUMEI_POOL_RESERVE_SIZE );

	size = sizeof( yumei_http_t );
	mem = yumei_malloc( pool, size );
	http = ( yumei_http_t* )mem->start;
	memset( http, 0, size );
	yumei_free( mem );


	size = sizeof( yumei_busi_t );
	mem = yumei_malloc( pool, size );
	busi = ( yumei_busi_t* )mem->start;
	memset( busi, 0, size );
	yumei_free( mem );


	size = sizeof( yumei_http_request_t );
	mem = yumei_malloc( pool, size );
	request = ( yumei_http_request_t* ) mem->start;
	memset( request, 0, size );
	yumei_free( mem );

	size = sizeof( yumei_http_header_t );
	mem = yumei_malloc( pool, size );
	header = ( yumei_http_header_t* )mem->start;
	memset( header, 0, size );
	yumei_free( mem );

	size = sizeof( yumei_http_cookie_t );
	mem = yumei_malloc( pool, size );
	cookie = ( yumei_http_cookie_t* )mem->start;
	memset( cookie, 0, size );
	yumei_free( mem );

	size = sizeof( yumei_http_cookie_t );
	mem = yumei_malloc( pool, size );
	mod_cookie = ( yumei_http_cookie_t* )mem->start;
	memset( mod_cookie, 0, size );
	yumei_free( mem );

	smap = yumei_smap_alloc( pool );
	request->smap = smap;

	smap = yumei_smap_alloc( pool );
	cookie->smap = smap;

	smap = yumei_smap_alloc( pool );
	mod_cookie->smap = smap;

	busi->conn = conn;
	http->conn = conn;
	busi->pool = pool;
	http->pool = pool;
	busi->http = http;
	http->data = busi;
	http->request = request;
	http->header = header;
	http->cookie = cookie;
	http->mod_cookie = mod_cookie;

	return http;
}

int yumei_http_free( yumei_http_t* http )
{
	yumei_pool_t* pool;
	yumei_mem_t* mem;

	pool = http->pool;
	yumei_pool_free( pool );

	return 0;
}
