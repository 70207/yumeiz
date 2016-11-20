#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#include "yumei_types.h"
#include "yumei_str.h"


const char* yumei_str_find( const char* str, char sb )
{
	if( !str )
		return 0;
	while( *str )
	{
		if( sb == *str )
			return str;
		++str;
	}

	return 0;
}

const char* yumei_str_find_not( const char* str, char sb )
{
	if( !str )
		return 0;
	while( *str )
	{
		if( sb != *str )
			return str;
		++str;
	}

	return 0;
}

const char* yumei_str_find_not_len( const char* str, char sb, int len )
{
	int cur;

	if( !str )
		return 0;
	for( cur=0; cur < len; ++cur )
	{
		if( sb != *( str + cur ) )
			return str + cur;
	}

	return 0;
}

const char* yumei_str_find_len( const char* str, char sb, int len )
{
	int cur;

	if( !str )
		return 0;
	for( cur=0; cur < len; ++cur )
	{
		if( sb == *( str + cur ) )
			return str + cur;
	}

	return 0;
}

const char* yumei_str_find_not_last( const char* str, char sb, const char* last )
{
	if( !str )
		return 0;
	while( str != last )
	{
		if( sb != *str )
			return str;
		++str;
	}

	return 0;
}

const char* yumei_str_find_last( const char* str, char sb, const char* last )
{
	if( !str )
		return 0;
	while( str != last )
	{
		if( sb == *str )
			return str;
		++str;
	}

	return 0;
}


int yumei_check_uri( const char* uri )
{
	char first;

	if( !uri )
	{
		return -1;
	}

	first = uri[0] | 32;
	
	if( first < 97 || first > 122 )
	{
		return -1;
	}

	return 0;
}
