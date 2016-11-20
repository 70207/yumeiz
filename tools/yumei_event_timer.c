#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include "yumei_types.h"
#include "yumei_event_timer.h"

volatile yumei_msec_t current = 0;

yumei_msec_t yumei_event_find_timer( yumei_msec_t wait_time, int wait_num )
{
	if( wait_num <= 0 )
	{
		if( wait_time < YUMEI_EVENT_WAIT_INFINITE )
		{
			wait_time += YUMEI_EVENT_WAIT_PER;
			if( wait_time >= YUMEI_EVENT_WAIT_BIG )
			{
				wait_time = YUMEI_EVENT_WAIT_INFINITE;
			}
		}
	}
	else
	{
		if( wait_time >= YUMEI_EVENT_WAIT_BIG )
		{
			wait_time = YUMEI_EVENT_WAIT_TIME;
		}
		
		wait_time -= wait_num * YUMEI_EVENT_WAIT_PER;
		
		if( wait_time < YUMEI_EVENT_WAIT_LEA )
		{
			wait_time = YUMEI_EVENT_WAIT_LEA;
		}
	}

	return wait_time;
}


int yumei_update_timer()
{
	yumei_msec_t timer;
	timer = current;
	timer += 1;
	//struct timeval tv;

	//gettimeofday( &tv, 0 );
	//timer = tv.tv_sec * 1000000 + tv.tv_usec;

	//printf( "update timer, sec:%llu,usec:%llu\n", tv.tv_sec, tv.tv_usec );
	//printf( "current:%llu\n", timer );
	current = timer;
	return 0;
}

yumei_msec_t yumei_get_current_timer()
{
	return current;
}

