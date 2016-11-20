#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


#include "yumei_signal.h"


int yumei_signal_init()
{
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigaction( SIGPIPE, &sa, 0 );
	
	return 0;
}
