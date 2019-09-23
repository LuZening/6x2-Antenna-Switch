/*
 * FS.h
 *
 *  Created on: Sep 12, 2019
 *      Author: Zening
 */

#ifndef FS_H_
#define FS_H_
#include <stddef.h>

typedef unsigned char UINT8;
typedef unsigned int UINT32;
#ifndef BOOL
#define BOOL UINT8
#define TRUE 1
#define FALSE 0
#endif
#define MAX_ADDR 0x08004000L // 16KB
#define ROOM_RESERVED 0x1000L // 4KB

typedef struct {
	char* path;
	UINT8 *p_content;
	UINT32 size;
}FSfile_typedef;

typedef struct {
	UINT32* addr_base;
	UINT32 n_files;
}FS_typedef;

BOOL FS_begin(FS_typedef* pFS, UINT32* addr_base);
FSfile_typedef FS_open(FS_typedef* pFS, const char* path);
int FS_size(FS_typedef* pFS, const char* path);
BOOL FS_exists(FS_typedef* pFS, const char* path);
extern FS_typedef FS;

#endif /* FS_H_ */
