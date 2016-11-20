#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "yumei_types.h"
#include "yumei_str.h"
#include "yumei_pool.h"
#include "yumei_chain.h"
#include "yumei_file.h"
#include "yumei_conn.h"
#include "yumei_http.h"
#include "yumei_event.h"
#include "yumei_busi.h"
#include "yumei_busi_com.h"

#include "yumei_busi_file.h"


int yumei_busi_open_file( yumei_busi_t* busi );
int yumei_busi_close_file( yumei_busi_t* busi );
int yumei_busi_read_file( yumei_busi_t* busi );

int yumei_busi_file( yumei_busi_t* busi )
{
	yumei_busi_open_file( busi );

	if( !busi->data )
	{
		busi->handle = yumei_busi_404;
	}
	else
	{
		busi->handle = yumei_busi_read_file;
	}

	return 0;

}

int yumei_busi_open_file( yumei_busi_t* busi )
{
	yumei_conn_t* conn;
	yumei_pool_t* pool;
	yumei_file_t* file;
	yumei_http_request_t* request;
	char* path;
	int flag;

	conn = busi->conn;
	pool = busi->pool;
	request = busi->http->request;
	flag = yumei_check_uri( request->uri );
	if( flag )
	{
		busi->data = 0;
		return -1;
	}	


	path = yumei_get_path( pool, "/tmp/", request->uri );	

	file = yumei_open_file( pool, path, YUMEI_FILE_READ );
	if( !file )
	{
		busi->data = 0;
		return -1;
	}

	busi->data = file;

	return 0;
}

int yumei_busi_close_file( yumei_busi_t* busi )
{
	yumei_http_t* http;

	http = busi->http;
	yumei_close_file( busi->data );
	busi->busi_state = YUMEI_BUSI_TO_SEND;
	yumei_http_send_init_header( http );
}

int yumei_busi_read_file( yumei_busi_t* busi )
{
	int size;
	int len;
	yumei_conn_t* conn;
	yumei_pool_t* pool;
	yumei_mem_t* mem;
	yumei_file_t* file;
	yumei_http_t* http;
	yumei_chain_t* chain;

	http = busi->http;
	conn = busi->conn;
	pool = busi->pool;
	file = busi->data;

	size = file->size;
	len = ( ( 2 * size + 2 ) / 1024 + 1 ) * 1024;

	chain = yumei_chain_append( http->send_chain, len );
	len = yumei_read_file( file, chain->buf, size );
	if( len < 0 )
	{
		len = 0;
	}

	memcpy( chain->buf + len, "\r\n\r\n", 2 );
	chain->io_len = len + 2;

	http->send_chain = chain;
	busi->handle = yumei_busi_close_file;
	return 0;
}

