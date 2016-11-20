#ifndef __TOOL_H__
#define __TOOL_H__


#ifndef _SAFE_RELEASE
#define _SAFE_RELEASE( ptr ) \
	if( ptr ) \
	{ \
	   delete ptr; \
	   ptr = 0; \
	} \

#endif

const char* yumei_str_find( const char* str, char sb );
const char* yumei_str_find_not( const char* str, char sb );

const char* yumei_str_find_not_len( const char* str, char sb, int len );
const char* yumei_str_find_len( const char* str, char sb, int len );

const char* yumei_str_find_not_last( const char* str, char sb, const char* last );
const char* yumei_str_find_last( const char* str, char sb, const char* last );

int yumei_check_uri( const char* uri );






#endif //__TOOL_H__
