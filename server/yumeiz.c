#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "yumei_types.h"
#include "yumei_pipe.h"
#include "yumei_listen.h"
#include "yumei_pool.h"
#include "yumei_chain.h"
#include "yumei_conn.h"
#include "yumei_event.h"
#include "yumei_signal.h"
#include "yumei_event_timer.h"

int main(int argc, char *const *argv)
{
	yumei_signal_init();
	yumei_listen_init();
	yumei_event_main_init();

	yumei_listen_append_c( "0.0.0.0", 80, 1 );


	yumei_update_timer();

	yumei_event_create_events();
	yumei_event_create_threads();


	while(1)
	{
		yumei_update_timer();
		usleep(1000);
	}

	return 0;
	

}
