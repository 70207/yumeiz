#ifndef __YUMEI_EVENT_EPOLL_H__
#define __YUMEI_EVENT_EPOLL_H__


int yumei_event_epoll_init( yumei_event_t* event );
int yumei_event_epoll_do( yumei_event_t* event );

int yumei_event_epoll_ctl( int epfd, int ctl, int info_index, yumei_event_info_t* info );





#endif //__YUMEI_EVENT_EPOLL_H__