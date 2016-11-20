#ifndef __YUMEI_EVENT_KQ_H__
#define __YUMEI_EVENT_KQ_H__


int yumei_event_kq_init( yumei_event_t* event );
int yumei_event_kq_do( yumei_event_t* event );

int yumei_event_kq_ctl( int epfd, int ctl, int info_index, yumei_event_info_t* info );



#endif //__YUMEI_EVENT_KQ_H__