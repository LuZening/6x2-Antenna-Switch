/*
 * FS.h
 *
 *  Created on: Sep 12, 2019
 *      Author: Zening
 */

#ifndef FS_H_
#define FS_H_
#include <stddef.h>
#include <stdint.h>

#ifndef BOOL
#define BOOL uint8_t
#define TRUE 1
#define FALSE 0
#endif
#define MAX_ADDR 0x08007fffL // 16KB
#define ROOM_RESERVED 0x4000L // 16KB

typedef struct {
	char* path;
	uint8_t *p_content;
	uint32_t size;
}FSfile_typedef;

typedef struct {
	uint32_t* addr_base;
	uint32_t n_files;
}FS_typedef;

BOOL FS_begin(FS_typedef* pFS, uint32_t* addr_base);
FSfile_typedef FS_open(FS_typedef* pFS, const char* path);
int FS_size(FS_typedef* pFS, const char* path);
BOOL FS_exists(FS_typedef* pFS, const char* path);
extern FS_typedef FS;

#endif /* FS_H_ */
