#ifndef __FUFEI_SOCKET_H_20120118
#define __FUFEI_SOCKET_H_20120118

#include <sys/socket.h>
#include "yumei_listen.h"

typedef struct yumei_socket_s yumei_socket_t;

enum yumei_socket_type
{
	YUMEI_SOCKET_LISTEN = 1,
	YUMEI_SOCKET_TCP = 2,
	YUMEI_SOCKET_UDP = 4,


};

struct yumei_socket_s{
	int fd;
	struct sockaddr addr;
	int socklen;
	int type;
};


int yumei_socket_init( yumei_socket_t* socket, int fd );
int yumei_socket_close( yumei_socket_t* socket );
yumei_socket_t* yumei_socket_create_by_listen( yumei_pool_t* pool, yumei_listen_t* yumei_listen );
yumei_socket_t* yumei_socket_create( yumei_pool_t* pool, int fd );
int yumei_socket_no_block( yumei_socket_t* socket );


#endif // __FUFEI_SOCKET_H_20120118
