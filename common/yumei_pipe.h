#ifndef __YUMEI_PIPE_H__20120119
#define __YUMEI_PIPE_H__20120119


typedef struct yumei_pipe_s yumei_pipe_t;

typedef enum yumei_pipe_type_s yumei_pipe_type_t;

typedef int (*yumei_pipe_handle)( yumei_pipe_t* _pipe );

struct yumei_pipe_s
{
	int fd;
	int type;
	void* data;
	int idata;
};

enum yumei_pipe_type_s
{
	YUMEI_PIPE_ERROR = -1,
	YUMEI_PIPE_ACCEPT,
	YUMEI_PIPE_HANDLE_BACK,
	YUMEI_PIPE_IN,
	YUMEI_PIPE_OUT,
	YUMEI_PIPE_OUT_CLOSE,
	YUMEI_PIPE_OUT_RE,
	YUMEI_PIPE_NOT_DO,
	YUMEI_PIPE_CLOSE,

};



#endif //__YUMEI_PIPE_H__20120119
