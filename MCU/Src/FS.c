/*
 * FS.c
 *
 *  Created on: Sep 12, 2019
 *      Author: Zening
 */
#include "FS.h"

FS_typedef FS;

BOOL FS_begin(FS_typedef* pFS, unsigned char* addr_base)
{
	UINT8* addr = (UINT8*)addr_base;
	if((unsigned int)addr >= MAX_ADDR - ROOM_RESERVED && (unsigned int)addr < MAX_ADDR)
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
	UINT8* addr_node; // pointer to file attribute node: file_size:4B, content_addr:4B, filename:char*
	UINT8* addr_filecontent; // pointer to file content

	FSfile_typedef file;
	file.size = 0;
	file.p_content = NULL;
	file.path = NULL;
	do{
		addr ++;
		i--;
		char* addr_path = (char*)(((UINT32*)*addr) + 2);
		UINT8* addr_content = (UINT8*)(((UINT32*)*addr) + 1);
		if(strcmp(path, addr_path) == 0) // path matched
		{
			file.path = addr_path;
			file.p_content = addr_content;
			file.size = **(UINT32**)addr;
		}
	}while(i > 0 && addr < MAX_ADDR );
	return file;
}

int FS_size(FS_typedef* pFS, const char* path)
{
	UINT32 i;
	UINT32** paddr_filenode = (UINT32**)((UINT32*)pFS->addr_base + 1);
	for(i=0; i<pFS->n_files; ++i)
	{
		char* addr_path = (char*)(*paddr_filenode + 2);
		if(strcmp(path, addr_path) == 0)
		{
			return *(UINT32*)(*paddr_filenode);
		}
		paddr_filenode++;
	}
	return -1;
}

BOOL FS_exists(FS_typedef* pFS, const char* path)
{
	UINT32 i;
	UINT32** paddr_filenode = (UINT32**)((UINT32*)pFS->addr_base + 1);
	for(i=0; i<pFS->n_files; ++i)
	{
		char* addr_path = (char*)(*paddr_filenode + 2);
		if(strcmp(path, addr_path) == 0)
		{
			return TRUE;
		}
		paddr_filenode++;
	}
	return FALSE;
}
