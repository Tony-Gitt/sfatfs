#ifndef SFATFS_PORT_H 
#define SFATFS_PORT_H

#include "sfatfs_types.h"
#ifdef __cplusplus
extern "C"{
#endif 

typedef void* disk_handle_t;
// 足够抽象的跨平台句柄

typedef struct{
    sector_t total_sectors;
    u16 sector_size=512;
    u8 read_only;
}disk_info_t;
// disk的信息，是否可读需要确定，扇区大小写定为512

// 需要的基本功能
disk_handle_t disk_init(const char* device);
void disk_release(disk_handle_t disk);
fs_result_t disk_read(disk_handle_t disk,sector_t sector,void* buffer,u32 count);
fs_result_t disk_write(disk_handle_t disk,sector_t sector,const void* buffer,u32 count);
fs_result_t disk_get_info(disk_handle_t disk,disk_info_t* info);
// read向buffer中写入，write读取buffer，disk_handle指向disk

#ifdef __cplusplus
}
#endif

#endif