#ifndef __YUMEI_FILE__H__
#define __YUMEI_FILE__H__

typedef struct yumei_file_s yumei_file_t;
typedef struct yumei_file_info_s yumei_file_info_t;
typedef enum yumei_file_type_s yumei_file_type_t;
typedef enum yumei_file_mode_s yumei_file_mode_t;


struct yumei_file_info_s
{
	int inode;
	int size;
	time_t mod;
	time_t start;

	int type;
	
};

struct yumei_file_s
{
	int fd;
	int read_pos;
	int size;
	
	char* path;
	yumei_file_info_t info;	
};

enum yumei_file_type_s
{
	YUMEI_FILE_PHOTO,
	YUMEI_FILE_EXE,
};

enum yumei_file_mode_s
{
	YUMEI_FILE_READ = 0,
	YUMEI_FILE_WRITE = 1,
	YUMEI_FILE_READ_WRITE = 2,
};

yumei_file_t* yumei_open_file( yumei_pool_t* pool, const char* path, int mode );
int yumei_close_file( yumei_file_t* file );
int yumei_file_stat( const char* path, yumei_file_info_t* info );
int yumei_read_file( yumei_file_t* file, char* buffer, int len );

char* yumei_get_path( yumei_pool_t* pool, const char* dir, const char* filename );

#endif //__YUMEI_FILE__H__
