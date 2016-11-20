#ifndef __YUMEI_TYPES_H__20131800
#define __YUMEI_TYPES_H__20131800

typedef unsigned char yumei_uchar_t;
typedef char yumei_char_t;
typedef unsigned char yumei_uint8_t;
typedef char yumei_int8_t;
typedef short yumei_int16_t;
typedef unsigned short yumei_uint16_t;
typedef int yumei_int32_t;
typedef unsigned int yumei_uint32_t;
typedef long long yumei_int64_t;
typedef unsigned long long yumei_uint64_t;
typedef unsigned long long yumei_msec_t;
typedef unsigned long long yumei_pthread_t;

typedef enum yumei_event_type_s yumei_event_type_t;
typedef enum yumei_event_control_s yumei_event_control_t;


enum yumei_type
{
	YUMEI_ERROR = -1,
	YUMEI_OK,

};

#define YUMEI_EVENTS_NUM_MAX 1024
#define YUMEI_EVENTS_MAIN_NUM_MAX 512
#define YUMEI_EVENT_WAIT_INFINITE (yumei_uint64_t)-1
#define YUMEI_EVENT_WAIT_TIME 30000
#define YUMEI_EVENT_WAIT_PER 3000
#define YUMEI_EVENT_WAIT_LEA 15000
#define YUMEI_EVENT_WAIT_BIG 80000

#define YUMEI_EVENT_TIME_OUT 30000
#define YUNEI_EVENT_HANDLE_INTERVAL 300


#endif //__YUMEI_TYPES_H__20131800
