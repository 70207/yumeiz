#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "yumei_types.h"
#include "yumei_pool.h"
#include "yumei_chain.h"
#include "yumei_conn.h"
#include "yumei_event.h"
#include "yumei_write.h"
#include "yumei_http.h"
#include "yumei_busi.h"

#define YUMEI_HTTP_SEND_HEADER_RESERVE 1024

const char* default_http_send_header = "HTTP/1.1 200 OK\r\nCache-Control: private\r\nContent-type: text/html\r\nContent-Encoding: plain\r\nContent-Length: %d\r\nServer: yumeiz/1.0\r\nConnection: Keep-Alive\r\n\r\n";

int yumei_http_send_init_header( yumei_http_t* http )
{
	yumei_conn_t* conn;
	yumei_pool_t* pool;
	yumei_mem_t* mem;
	yumei_uchar_t* header_buf;
	yumei_uchar_t* content_buf;

	yumei_chain_t* chain;

	int content_len;
	int head_len;
	int send_len;


	conn = http->conn;
	pool = http->pool;
	
	content_len = http->send_chain->io_len - 2;
	chain = yumei_chain_insert( http->send_chain, YUMEI_HTTP_SEND_HEADER_RESERVE );
	
	head_len = sprintf( chain->buf, default_http_send_header, content_len );
	
	chain->io_len = head_len;

	yumei_chain_first_set( chain );

	yumei_mutex_lock( conn->mutex );
	conn->send_chain = yumei_chain_link( conn->send_chain, chain );
	yumei_mutex_unlock( conn->mutex );

	yumei_http_free( http );
	return 0;

}

int yumei_http_send( yumei_conn_t* conn )
{
	return conn->wev( conn );
}
