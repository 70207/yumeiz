#ifndef __YUMEI_LISTEN_H__20120120
#define __YUMEI_LISTEN_H__20120120

struct yumei_listen_s
{
	int ip;
	int port;
	int thread_num;
};

typedef struct yumei_listen_s yumei_listen_t;



int yumei_listen_init();
int yumei_listen_get_num();
yumei_listen_t* yumei_listen_get( int item );
int yumei_listen_append_c( const char* ip, int port, int thread_num );
int yumei_listen_append( int ip, int port, int thread_num );



#endif //__YUMEI_LISTEN_H__20120120
