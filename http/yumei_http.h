#ifndef __YUMEI_HTTP_H__20130204
#define __YUMEI_HTTP_H__20130204

#include "yumei_conn.h"
#include "yumei_smap.h"

typedef struct yumei_http_header_s yumei_http_header_t;
typedef struct yumei_http_request_s yumei_http_request_t;
typedef struct yumei_http_cookie_s yumei_http_cookie_t;
typedef struct yumei_http_s yumei_http_t;
typedef enum yumei_http_type_s yumei_http_type_t;

enum yumei_http_type_s
{
	YUMEI_HTTP_GET,
	YUMEI_HTTP_POST,
};

struct yumei_http_s
{
	yumei_pool_t* pool;
	void* data;
	yumei_chain_t* send_chain;
	yumei_http_header_t* header;
	yumei_http_request_t* request;
	yumei_http_cookie_t* cookie;
	yumei_http_cookie_t* mod_cookie;

	yumei_conn_t* conn;
	int type;
};


struct yumei_http_header_s
{
	const char* header;
	char* host;
	char* connection;
	char* cache_control;
	char* user_agent;
	char* accept;
	char* accept_encoding;
	char* accept_language;
	char* accept_charset;

	char* server;
	char* date;
	char* content_type;
	char* transfer_encoding;
	char* p3p;
	char* expires;
	char* content_encoding;
};

struct yumei_http_request_s
{
	const char* request;
	char* uri;
	int has_uri;
	yumei_smap_t* smap;
};

struct yumei_http_cookie_s
{
	const char* cookie;
	yumei_smap_t* smap;
};


int yumei_http_init( yumei_conn_t* conn );
int yumei_http_read( yumei_conn_t* conn );
int yumei_http_send_init_header( yumei_http_t* http );
int yumei_http_send( yumei_conn_t* conn );

yumei_http_t* yumei_http_alloc( yumei_conn_t* conn );
int yumei_http_free( yumei_http_t* http );



#endif //__YUMEI_HTTP_H__20130204
