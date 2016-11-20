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
#include "yumei_busi_dyc.h"
#include "yumei_busi_file.h"

int yumei_busi_check_type( yumei_busi_t* busi );

int yumei_busi_request( yumei_busi_t* busi )
{

	printf( "FUNC:%s\n", __FUNCTION__ );

	yumei_busi_check_type( busi );

	switch( busi->type )
	{
	case YUMEI_BUSI_DYNAMIC:
		busi->handle = yumei_busi_dynamic;
		break;
	case YUMEI_BUSI_FILE:
		busi->handle = yumei_busi_file;
		break;
	default:
		busi->handle = yumei_busi_dynamic;
	}
	

	return 0;
}



int yumei_busi_check_type( yumei_busi_t* busi )
{
	busi->type = YUMEI_BUSI_FILE;
	return 0;
}

int yumei_busi_404( yumei_busi_t* busi )
{
	yumei_pool_t* pool;
	yumei_conn_t* conn;
	yumei_mem_t* mem;
	yumei_http_t* http;
	yumei_chain_t* chain;
	char buffer[ 128 ];
	int len;

	conn = busi->conn;
	pool = busi->pool;
	http = busi->http;

	strcpy( buffer, "hello, this is yumeiz!\r\n\r\n" );
	len = strlen( buffer );

	chain = yumei_chain_append( http->send_chain, len );
	chain->io_len = len;

	memcpy( chain->buf, buffer, len );
	busi->busi_state = YUMEI_BUSI_TO_SEND;

	http->send_chain = chain;
	yumei_http_send_init_header( http );

	return 0;
}



