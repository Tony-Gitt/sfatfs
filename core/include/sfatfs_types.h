#ifndef SFATFS_TYPES
#define SFATFS_TYPES

#include<stdint.h>
#include<stdbool.h>

typedef uint8_t u8;  // 底层就是要强类型
typedef uint16_t u16;
typedef uint32_t u32;
typedef int32_t s32;
// 分别做数据，fat16簇号，fat32簇号，错误码的类型

typedef u32 sector_t;
typedef u32 cluster_t;
typedef u32 file_size_t;
// 扇区号，簇号，文件大小都用u32，要节约后面改

#ifndef FS_PACKED
#define FS_PACKED __attribute__((packed))
#endif 
// 设置gcc的对齐方式(x86 arm不同)
// 需要的数据结构会有FS_PACKED

#define FS_SECTOR_SIZE 512 
#define FS_SECTOR_SHIFT 9
#define FS_MAX_FILE_SIZE (0xFFFFFFFF) //4GB大小限制

#define FS_FAT12_EOF 0x0FF8
#define FS_FAT16_EOF         0xFFF8  
#define FS_FAT32_EOF         0x0FFFFFF8
#define FS_CLUSTER_FREE      0x00000000
#define FS_CLUSTER_BAD       0x0FFFFFF7
#define FS_CLUSTER_EOF_MASK  0x0FFFFFF8
// 定义每种簇的大小，FAT表记录簇的状态

typedef enum{
    FS_OK=0,
    FS_ERROR=-1,
    FS_IO_ERROR=-2,
    FS_NOT_FOUND=-3,
    FS_EXISTS=-4,
    FS_INVALID=-5,
    FS_FULL=-6,
    FS_NOT_READY=-7,
}fs_result_t;
// 操作返回的状态码

typedef struct FS_PACKED{
    u16 second:5;
    u16 minute:6;
    u16 hour:5;
}sfat_time_t;
// u16拆成3个，5位是0-32，两秒为单位
typedef struct FS_PCKED{
    u16 day:5;
    u16 month:4;
    u16 year:7;
}sfat_date_t;

#define FS_READ_ONLY (1<<0)
#define FS_HIDDEN (1<<1)
#define FS_SYSTEM (1<<2)  // 为系统文件
#define FS_VOLUME_ID (1<<3)  // 为卷标
#define FS_DIRECTORY (1<<4)  // 为目录，linux特性
#define FS_ARCHIVE (1<<5)  // 为归档
// u8拆成8位，位掩码表状态
#define FS_SYMLINK (1<<6)  // 软链接
#define FS_ENCRYPTED (1<<7)  // 加密

// 一些宏
#define FS_MIN(a,b) ((a)<(b)?(a):(b))
#define FS_MAX(a,b) ((a)>(b)?(a):(b))

#define FS_SECTOR_2_BYTES(s) ((s)<<FS_SECTOR_SHIFT) // 通过移位避免除法
#define FS_BYTE_2_SECTORS(s) (((b)+(FS_SECTOR_SIZE-1))>>FS_SECTOR_SHIFT)
// 避免除法同时向上取整
#define FS_VALID_CLUSTER(c) ((c)>=2)

#define FS_SUPPORT_READ      1     
#define FS_SUPPORT_WRITE     1     
#define FS_SUPPORT_DIR       1     
#define FS_SUPPORT_FILE      1     
#define FS_USE_SIMPLE_PATHS  1     // 简单路径解析（无相对路径）
// 功能配置

#endif