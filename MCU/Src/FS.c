/*
 * FS.c
 *
 *  Created on: Sep 12, 2019
 *      Author: Zening
 */
#include <string.h>
#include "FS.h"
FS_typedef FS;

BOOL FS_begin(FS_typedef* pFS, UINT32* addr_base)
{
	if((UINT32)addr_base >= 0)
	{
		pFS->n_files = *(UINT32*)(addr_base); // read number of files from base address
		pFS->addr_base = addr_base;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

FSfile_typedef FS_open(FS_typedef* pFS, const char* path)
{
	int i = pFS->n_files;
	UINT32* addr = (UINT32*)pFS->addr_base;  // base_addr: number_of_files:4B, addr_file_nodes:4B[n_files]
	UINT32 offset = (UINT32)pFS->addr_base;

	FSfile_typedef file;
	file.size = 0;
	file.p_content = NULL;
	file.path = NULL;
	do{
		addr ++;
		i--;
		char* _path = (char*)((UINT32*)(*addr+offset)+2);
		if(strcmp(path, _path) == 0) // path matched
		{
			file.path = _path;
			file.p_content = (UINT8*)(*((UINT32*)(*addr + offset) + 1)+offset);
			file.size = *(UINT32*)(*addr + offset);
		}
	}while(i > 0);
	return file;
}

int FS_size(FS_typedef* pFS, const char* path)
{
	UINT32 i;
	UINT32* addr = (UINT32*)pFS->addr_base;
	UINT32 offset = (UINT32)pFS->addr_base;
	for(i=0; i<pFS->n_files; ++i)
	{
		addr++;
		char* _path = (char*)((UINT32*)(*addr+offset)+2);
		if(strcmp(path, _path) == 0)
		{
			return *(UINT32*)(*addr + offset);
		}
	}
	return -1;
}

BOOL FS_exists(FS_typedef* pFS, const char* path)
{
	UINT32 i;
	UINT32* addr = (UINT32*)pFS->addr_base;
	UINT32 offset = (UINT32)pFS->addr_base;
	for(i=0; i<pFS->n_files; ++i)
	{
		addr++;
		char* _path = (char*)((UINT32*)(*addr+offset)+2);
		if(strcmp(path, _path) == 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}
