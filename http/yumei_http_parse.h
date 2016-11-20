#ifndef __YUMEI_HTTP_PARSE__H_20130204
#define __YUMEI_HTTP_PARSE__H_20130204

typedef enum yumei_http_parse_type_s yumei_http_parse_type_t;

enum yumei_http_parse_type_s
{
	YUMEI_HTTP_PARSE_OK,
	YUMEI_HTTP_PARSE_REQUEST_FAIL,
	YUMEI_HTTP_PARSE_HEADER_FAIL,
	YUMEI_HTTP_PARSE_COOKIE_FAIL,
};

int yumei_http_parse( yumei_http_t* http );

#endif //__YUMEI_HTTP_PARSE__H_20130204