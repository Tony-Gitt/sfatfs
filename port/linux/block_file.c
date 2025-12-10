#include<fcntl.h> //O_RDONLY等宏
#include<unistd.h> //文件操作相关
#include<sys/stat.h>
#include<errno.h>
#include<stdlib.h>
#include "sfatfs_types.h"
#include "sfatfs_port.h"

typedef struct {
    s32 fd;
    disk_info_t info ;
}posix_disk_t;


// 镜像一直存在，init deinit只是取句柄
disk_handle_t disk_init(const char* device){
    s32 fd;
    struct stat st;
    posix_disk_t* disk;
    u8 readonly=0;
    s32 st_size=16*1024*1024;

    // 尝试打开
    if(!device ||!device[0]) return NULL;
    fd=open(device,O_RDWR);
    if(fd<0){
        fd=open(device,O_RDONLY);
        if(fd<0){
            fd = open(device, O_RDWR | O_CREAT, 0644);
            if (fd < 0) return NULL;
        }
        else readonly=1;
    }
    if(fstat(fd,&st)<0){
        close(fd);
        return NULL;
    }

    // size有问题时处理
    if(st.st_size==0){
        if (!readonly) {
            if (ftruncate(fd, st_size) < 0) {
                close(fd);
                return NULL;
            }
            st.st_size = st_size;
        } else {
            close(fd);
            return NULL;
        }
    }
    if (st.st_size % FS_SECTOR_SIZE != 0) {
        if (readonly) {
            close(fd);
            return NULL;
        } else {
            u32 new_size = ((st.st_size + FS_SECTOR_SIZE-1) / FS_SECTOR_SIZE) * FS_SECTOR_SIZE;
            if (ftruncate(fd, new_size) < 0) {
                close(fd);
                return NULL;
            }
            st.st_size = new_size;
        }
    }

    // 初始化
    disk = malloc(sizeof(posix_disk_t));
    if (!disk) {
        close(fd);
        return NULL;
    }
    disk->fd = fd;
    disk->info.total_sectors = st.st_size / FS_SECTOR_SIZE;
    disk->info.sector_size = FS_SECTOR_SIZE;
    disk->info.read_only = readonly;
    lseek(fd, 0, SEEK_SET); 
    
    return (disk_handle_t)disk;
}

void disk_release(disk_handle_t disk){
    posix_disk_t* d=(posix_disk_t*) disk;
    if(!d) return ;
    close(d->fd);
    free(d);
}

fs_result_t disk_read(disk_handle_t disk,sector_t sector,void* buffer,u32 count){
    posix_disk_t* const d=(posix_disk_t*)disk;
    if(!d||!buffer||count==0)return FS_INVALID;
    const u32 end=sector+count;
    if(end>d->info.total_sectors)
        return FS_INVALID;
    const u32 offset=sector<<FS_SECTOR_SIZE;
    const u32 total_bytes=count<<FS_SECTOR_SIZE;
    // 计算实际偏移与写入的bytes数
    u32 bytes=pread(d->fd,buffer,total_bytes,(off_t)offset);
    if(bytes!=total_bytes)
        return FS_ERROR;
    return FS_OK;
}

fs_result_t disk_write(disk_handle_t disk,sector_t sector,const void* buffer,u32 count){
    posix_disk_t* const d=(posix_disk_t*)disk;
    if(!d||!buffer||count==0)return FS_INVALID;
    if(d->info.read_only) return FS_ERROR;
    const u32 end=sector+count;
    if(end>d->info.total_sectors)
        return FS_INVALID;
    const u32 offset=sector<<FS_SECTOR_SIZE;
    const u32 total_bytes=sector<<FS_SECTOR_SIZE;
    const u32 bytes=pwrite(d->fd,buffer,total_bytes,(off_t)offset);
    if(bytes!=total_bytes)
        return FS_ERROR;
    return FS_OK;
}

fs_result_t disk_get_info(disk_handle_t disk,disk_info_t* info){
    posix_disk_t* const d=(posix_disk_t*) disk;
    if(d==NULL||info==NULL)
        return FS_INVALID;
    info->total_sectors=d->info.total_sectors;
    info->sector_size=d->info.sector_size;
    info->read_only=d->info.read_only;
    return FS_OK;

}