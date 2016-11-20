#ifndef __BUSI_SERVER_H__
#define __BUSI_SERVER_H__

typedef enum yumei_busi_state_s yumei_busi_state_t;
typedef enum yumei_busi_type_s yumei_busi_type_t;

typedef struct yumei_busi_s yumei_busi_t;

typedef int (*yumei_handle_busi)( yumei_busi_t* busi );

struct yumei_busi_s
{
	void* content;
	yumei_conn_t* conn;
	yumei_http_t* http;
	yumei_pool_t* pool;
	int busi_state;
	yumei_handle_busi handle;
	
	int type;
	void* data;

};

enum yumei_busi_state_s
{
	YUMEI_BUSI_OK = 0,
	YUMEI_BUSI_TO_SEND,
	YUMEI_BUSI_TO_HANDLE,
	YUMEI_BUSI_SENT,
};


enum yumei_busi_type_s
{
	YUMEI_BUSI_DYNAMIC,
	YUMEI_BUSI_FILE,

};








#endif //__BUSI_SERVER_H__

