
#include <malloc.h>
#include "yumei_types.h"
#include "yumei_listen.h"

#define YUMEI_LISTEN_MAIN_RESERVE 8

typedef struct yumei_listen_main_s yumei_listen_main_t;

struct yumei_listen_main_s
{
	int num;
	yumei_listen_t** listens;
};

yumei_listen_main_t main_listen;

int yumei_listen_init()
{
	main_listen.num = 0;
	main_listen.listens = 0;

	return 0;
}

int yumei_listen_get_num()
{
	return main_listen.num;
}

yumei_listen_t* yumei_listen_get( int item )
{
	return main_listen.listens[item ];
}

int yumei_listen_append_c( const char* ip, int port, int thread_num )
{
	int _ip;

	_ip = inet_addr( ip );

	return yumei_listen_append( _ip, port, thread_num );
}

int yumei_listen_append( int ip, int port, int thread_num )
{
	int size;
	yumei_listen_t** listens;
	yumei_listen_t*  listen;
	int i;

	if( !main_listen.num )
	{
		size = YUMEI_LISTEN_MAIN_RESERVE;
		listens = ( yumei_listen_t** )malloc( size * sizeof( yumei_listen_t* ) );
		for( i = 0; i < size;++i )
		{
			listens[ i ] = ( yumei_listen_t* ) malloc( sizeof( yumei_listen_t ) );
		}
	
		main_listen.listens = listens;
	}
	else if( main_listen.num % YUMEI_LISTEN_MAIN_RESERVE == 0 )
	{
		size = main_listen.num + YUMEI_LISTEN_MAIN_RESERVE;
		listens = ( yumei_listen_t** ) realloc( main_listen.listens, size * sizeof( yumei_listen_t* ) );
		for( i = main_listen.num; i < size; ++i )
		{
			listens[ i ] = ( yumei_listen_t* ) malloc( sizeof( yumei_listen_t ) );
		}
		
		main_listen.listens = listens;
	}
	else
	{
		listens = main_listen.listens;
	}

	listen = listens[ main_listen.num++ ];
	listen->ip = ip;
	listen->port = port;
	listen->thread_num = thread_num;

	return 0;

}
