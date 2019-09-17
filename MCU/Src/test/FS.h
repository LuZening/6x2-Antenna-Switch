/*
 * FS.h
 *
 *  Created on: Sep 12, 2019
 *      Author: Zening
 */

#ifndef FS_H_
#define FS_H_

typedef unsigned char UINT8;
typedef unsigned int UINT32;
ifndef bool
#define MAX_ADDR 0x08004000L // 16KB
#define ROOM_RESERVED 0x1000L // 4KB

typedef struct {
	char* path;
	UINT8 *p_content;
	unsigned int size;
}FSfile_typedef;

typedef struct {
	unsigned char* addr_base;
	unsigned int n_files;
}FS_typedef;

BOOL FS_begin(FS_typedef* pFS, unsigned char* addr_base);
FSfile_typedef FS_open(FS_typedef* pFS, const char* path);
int FS_size(FS_typedef* pFS, const char* path);
BOOL FS_exists(FS_typedef* pFS, const char* path);
extern FS_typedef FS;

#endif /* FS_H_ */
