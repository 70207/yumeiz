#include <stdio.h>
#include <stdlib.h>

#include "yumei_types.h"
#include "yumei_str.h"
#include "yumei_pool.h"
#include "yumei_chain.h"
#include "yumei_conn.h"
#include "yumei_http.h"
#include "yumei_http_parse.h"

int yumei_http_parse_header( yumei_http_t* http );
int yumei_http_parse_request( yumei_http_t* http );
int yumei_http_parse_cookie( yumei_http_t* http );
int yumei_http_parse_param( yumei_http_t* http, const char* param, const char* last );


int yumei_http_parse( yumei_http_t* http )
{
 	printf( "FUNC:%s\n", __FUNCTION__ );	

	yumei_http_parse_request( http );
	yumei_http_parse_header( http );
	yumei_http_parse_cookie( http );

	return http;
}

int yumei_http_parse_request( yumei_http_t* http )
{
	yumei_http_request_t* request;
	yumei_pool_t*  pool;
	yumei_mem_t* mem;
	yumei_conn_t* conn;
	const char* r_buf;
	int r_len;
	const char* cur;
	const char* nc;
	const char* nc2;
	const char* last;
	char buffer[ 32 ];
	int i;
	int size;

	conn = http->conn;
	pool = http->pool;
	request = http->request;
	r_buf = conn->read_buf->start;
	r_len = conn->read_buf->last - conn->read_buf->start;
	last = conn->read_buf->last;

	
 	printf( "FUNC:%s\n", __FUNCTION__ );	
	cur = yumei_str_find_not_last( r_buf, ' ',  last );
	if( !cur )
	{
		return YUMEI_HTTP_PARSE_REQUEST_FAIL;
	}

	
	
	if( ((*cur )| 32) == 'g' )
	{
		http->type = YUMEI_HTTP_GET;
	}
	else
	{
		http->type = YUMEI_HTTP_POST;
	}
	
	
	nc = yumei_str_find_last( cur, ' ', last );  
	if( !nc )
	{
		return YUMEI_HTTP_PARSE_REQUEST_FAIL;
	}

	cur = nc + 2;
	request->request = cur;
	nc = yumei_str_find_last( cur, ' ', last );
	nc2 = yumei_str_find_last( cur, '?', nc );
	if( nc2 )
	{
		request->has_uri = 1;
		size = nc2 - cur;
	}
	else
	{
		size = nc - cur;
	}

	mem = yumei_malloc( pool, size + 1 );
	request->uri = mem->start;
	yumei_free( mem );

	memcpy( request->uri, cur, size );
	request->uri[ size ] = 0;

	if( nc2 )
	{
		++nc2;
		yumei_http_parse_param( http, nc2, nc );
	}


	return 0;
}

int yumei_http_parse_header( yumei_http_t* http )
{
	yumei_http_header_t* header;
	yumei_pool_t*  pool;
	yumei_mem_t* mem;
	yumei_conn_t* conn;
	const char* r_buf;
	int r_len;
	const char* cur;
	const char* nc;
	const char* last;
	char buffer[ 64 ];
	int i;
	int size;

	conn = http->conn;
	pool = http->pool;
	r_buf = conn->read_buf->start;
	r_len = conn->read_buf->last - conn->read_buf->start;
	last = conn->read_buf->last;

	cur = r_buf;
	
 	printf( "FUNC:%s\n", __FUNCTION__ );	

	return 0;

}



int yumei_http_parse_cookie( yumei_http_t* http )
{
	yumei_http_cookie_t* header;
	yumei_pool_t*  pool;
	yumei_mem_t* mem;
	yumei_conn_t* conn;
	const char* r_buf;
	int r_len;
	const char* cur;
	const char* nc;
	const char* last;
	char buffer[ 32 ];
	int i;
	int size;

	conn = http->conn;
	pool = http->pool;
	r_buf = conn->read_buf->start;
	r_len = conn->read_buf->last - conn->read_buf->start;
	last = conn->read_buf->last;


	cur = strstr( r_buf, "Cookie" );

 	printf( "FUNC:%s\n", __FUNCTION__ );	

	return 0;
	
}


int yumei_http_parse_param( yumei_http_t* http, const char* param, const char* last )
{
	yumei_http_request_t* request;
	yumei_pool_t* pool;
	yumei_mem_t* mem;
	yumei_smap_t* smap;
	int len;
	char* nc;
	char* nc2;
	const char* name;
	const char* value;
	

	if( !http || !param || !last )
	{
		return -1;
	}

	pool = http->pool;
	request = http->request;


	len = last - param;
	mem = yumei_malloc( pool, len + 1 );
	memcpy( mem->start, len, 0 );
	memset(mem->start + len, 0, 1);
	param = mem->start;
	last = mem->last;
	
 	printf( "FUNC:%s\n", __FUNCTION__ );	
	

	nc2 = param;
	while( nc2 < last )
	{
		nc = yumei_str_find_last( param, '=', last );
		name = param;
		nc2 = yumei_str_find_last( nc, '&', last );
		if( !nc2 )
		{
			nc2 = last;
		}

		*nc = 0;
		*nc2 = 0;
		value = ++nc;

		yumei_smap_insert( smap, name, value );

	}
	
	yumei_free( mem );

	return 0;
}
