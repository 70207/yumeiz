#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "yumei_types.h"
#include "yumei_str.h"
#include "yumei_pool.h"
#include "yumei_chain.h"
#include "yumei_file.h"
#include "yumei_conn.h"
#include "yumei_http.h"
#include "yumei_event.h"
#include "yumei_busi.h"
#include "yumei_busi_com.h"

#include "yumei_busi_dyc.h"


int yumei_busi_request_do1( yumei_busi_t* busi );
int yumei_busi_request_end( yumei_busi_t* busi );


int yumei_busi_dynamic( yumei_busi_t* busi )
{

	printf( "FUNC:%s\n", __FUNCTION__ );
	busi->handle = yumei_busi_request_do1;
	return 0;
}

int yumei_busi_request_do1( yumei_busi_t* busi )
{

	printf( "FUNC:%s\n", __FUNCTION__ );
	busi->handle = yumei_busi_request_end;
	return 0;
}

int yumei_busi_request_end( yumei_busi_t* busi )
{
	printf( "FUNC:%s\n", __FUNCTION__ );

	yumei_busi_404( busi );
	return 0;
}
