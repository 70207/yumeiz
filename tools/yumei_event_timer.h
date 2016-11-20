#ifndef __YUMEI_EVENT_TIMER_H__
#define __YUMEI_EVENT_TIMER_H__




yumei_msec_t yumei_event_find_timer( yumei_msec_t wait_time, int wait_num );

int yumei_update_timer();

yumei_msec_t yumei_get_current_timer();


#endif //__YUMEI_EVENT_TIMER_H__
