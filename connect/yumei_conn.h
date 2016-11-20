#ifndef __FUFEI_CONN_H__20120111
#define __FUFEI_CONN_H__20120111

#include "yumei_mutex.h"
#include "yumei_socket.h"

#define YUMEI_CONN_READ_BUFFER (3*1024)

typedef enum yumei_conn_state_s yumei_conn_state_t;
typedef enum yumei_conn_io_s yumei_conn_io_t;
typedef struct yumei_conn_s yumei_conn_t;

typedef int (*conn_handle)( yumei_conn_t* conn );
typedef int (*conn_handle)( yumei_conn_t* conn );



struct yumei_conn_s{
	yumei_socket_t* socket;
	void* data;

	yumei_pool_t*  pool;
	yumei_mem_t*   read_buf;
	yumei_chain_t*  send_chain;

	yumei_mutex_t* mutex;

	yumei_msec_t start;
	yumei_msec_t expire;
	yumei_msec_t last_read;	

	int state;
	int iskeepalive;

	conn_handle ihandle;
	conn_handle ohandle;
	conn_handle rev;
	conn_handle wev;

	conn_handle finish;

	int event_pipe;
	int event_fd;
	int event_index;
	int info_index;
	int pending_state;

	void* event;
	int nullreadtimes;
};

enum yumei_conn_state_s
{
	YUMEI_CONN_EMPTY = 0,
	YUMEI_CONN_ACCEPT = 1,
	YUMEI_CONN_HANDLE = 2,
	YUMEI_CONN_KEEP_ALIVE = 4,
	YUMEI_CONN_CLOSE = 8,
	YUMEI_CONN_TO_SEND = 16,
};

enum yumei_conn_io_s
{
	YUMEI_CONN_IO_FAIL = -1,
	YUMEI_CONN_IO_OK = 0,
	YUMEI_CONN_IO_AGAIN = 1,
	YUMEI_CONN_IO_NULL,
};

int yumei_conn_finish( yumei_conn_t* conn );
int yumei_conn_set_time( yumei_conn_t* conn, time_t start, time_t expire );
int yumei_conn_keep_alive( yumei_conn_t* conn );
int yumei_conn_check_alive( yumei_conn_t* conn );
int yumei_conn_miss_alive( yumei_conn_t* conn );


yumei_conn_t* yumei_conn_create();


#endif //__FUFEI_CONN_H__20120111
