#ifndef __YUMEI_EVENT_H__20120119
#define __YUMEI_EVENT_H__20120119

#define YUMEI_EVENTS_NUM_MAX 1024
#define YUMEI_EVENTS_MAIN_NUM_MAX 512
#define YUMEI_EVENT_WAIT_INFINITE (yumei_uint64_t)-1
#define YUMEI_EVENT_WAIT_TIME 30000
#define YUMEI_EVENT_WAIT_PER 3000
#define YUMEI_EVENT_WAIT_LEA 15000
#define YUMEI_EVENT_WAIT_BIG 80000

#define YUMEI_EVENT_TIME_OUT 30000
#define YUNEI_EVENT_HANDLE_INTERVAL 300

#include "yumei_listen.h"
#include "yumei_socket.h"
#include "yumei_conn.h"

typedef struct yumei_event_s yumei_event_t;
typedef struct yumei_event_info_s yumei_event_info_t;
typedef struct yumei_event_main_s yumei_event_main_t;
typedef struct yumei_event_pool_s yumei_event_pool_t;

typedef int (*yumei_event_handle)( yumei_event_t* event );

typedef yumei_conn_t* (*yumei_event_accept)( int fd );



struct yumei_event_info_s
{
	int fd;
	int type;
	int iskeepalive;
	int isaccept;
	yumei_conn_t* conn;
	time_t start;
	time_t expire;
	int used;

};


struct yumei_event_s
{
	int  index;
	int  id;
	int  fd;

	int  nums;
	yumei_msec_t  wait_time;
	
	yumei_event_info_t* event_info;
	int empty_info_index;
	void* data;

	yumei_pool_t* pool;
	yumei_mutex_t* mutex;
	yumei_event_accept yumei_accept;

	int  thread_run;
	int  alivenum;
};

struct yumei_event_pool_s
{
	yumei_listen_t* listen;
	yumei_socket_t* socket;
	int event_num;
	yumei_pthread_t* child_thread_id;
	yumei_event_t** events;

};

struct yumei_event_main_s
{
	int listen_num;
	int listen_threads_total_num;

	yumei_event_pool_t** event_pools;
	yumei_pool_t*  mem_pool;
};

enum yumei_event_type_s
{
	YUMEI_EVENT_ERROR = -1,
	YUMEI_EVENT_ACCEPT = 1,
	YUMEI_EVENT_IN = 2,
	YUMEI_EVENT_OUT = 4,
	YUMEI_EVENT_ET = 8,
};

enum yumei_event_control_s
{
	YUMEI_EVENT_ADD,
	YUMEI_EVENT_MOD,
	YUMEI_EVENT_DEL,
};


int yumei_event_init( yumei_event_t* event );
int yumei_event_proc( void* data );



int yumei_event_task( yumei_event_t* event, int info_index, int type );

int yumei_event_add( yumei_event_t* event, yumei_socket_t* socket, int type );
int yumei_event_del( yumei_event_t* event, int info_index );

int yumei_event_add_conn( yumei_event_t* event, yumei_conn_t* conn );


int yumei_event_check_time( yumei_event_t* event );
int yumei_event_check_alive( yumei_event_t* event, int info_index );
int yumei_event_mod_type( yumei_event_t* event, int info_index, int type );
int yumei_event_mod_not_type( yumei_event_t* event, int info_index, int type );
int yumei_event_out_re( yumei_event_t* event, int info_index );


int yumei_event_main_init();

int yumei_event_create_threads();

int yumei_event_create_events();

int yumei_event_wait_threads();



#endif //__YUMEI_EVENT_H__20120119
