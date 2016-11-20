#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>

#include "yumei_types.h"
#include "yumei_pool.h"
#include "yumei_file.h"


yumei_file_t* yumei_open_file( yumei_pool_t* pool, const char* path, int mode )
{
	int fd;
	int size;
	yumei_file_t* file;
	char* fpath;
	struct stat fstat;
	

	if( !pool || !path )
	{
		return 0;
	}

	fd = open( path, mode );
	if( fd == -1 )
	{
		return 0;
	}

	size = sizeof( yumei_file_t );
	file = ( yumei_file_t* ) yumei_raw_malloc( pool, size );
	memset( file, 0, size );
	
	size = strlen( path ) + 1;
	fpath = ( char* )yumei_raw_malloc( pool, size );

	memcpy( fpath, path, size );
	file->fd = fd;

	stat( path, &fstat );

	file->info.inode = fstat.st_ino;
	file->size = fstat.st_size;
	file->info.mod = fstat.st_mtime;
	file->info.start = fstat.st_ctime;
	file->info.size = fstat.st_size;

	return file;
}

int yumei_close_file( yumei_file_t* file )
{
	if( !file )
	{
		return -1;
	}

	close( file->fd );

	return 0;
}

int yumei_file_stat( const char* path, yumei_file_info_t* info )
{
	struct stat fstat;
	int flag;

	flag = stat( path, &fstat );
	if( flag )
	{
		return flag;
	}

	info->inode = fstat.st_ino;
	info->size = fstat.st_size;
	info->mod = fstat.st_mtime;
	info->start = fstat.st_ctime;
	
	return 0;
}

int yumei_read_file( yumei_file_t* file, char* buffer, int len )
{
	int ret;
	int err;
	if( !file || !buffer  )
	{
		return -1;
	}

	if( len <= 0 )
	{
		return 0;
	}

	ret = read( file->fd, buffer, len );
	if( ret < 0 )
	{
		err = errno;
		printf( "__FUNCTION__ error:%d\n", err );
	}

	return ret;

}

char* yumei_get_path( yumei_pool_t* pool, const char* dir, const char* filename )
{
	yumei_mem_t* mem;
	char* path;
	int size;

	size = strlen( dir );
	size += strlen( filename );
	size += 1;

	mem = yumei_malloc( pool, size );
	path = mem->start;
	yumei_free( mem );

	strcpy( path, dir );
	strcat( path, filename );

	return path;
}
