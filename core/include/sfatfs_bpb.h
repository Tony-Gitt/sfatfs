#ifndef SFATFS_BPB_H
#define SFATFS_BPB_H
#include "sfatfs_types.h"

#define SFATFS_BPB_VER 1

#if SFATFS_BPB_VER ==0
typedef struct FS_PACKED{
    u8 jump_boot[3];
    u8 oem_name[8];
    u16 byte_p_sector;
    u8 sector_p_cluster;
    u16 reserved_sectors;
    u8 num_fats;
    u16 root_entries;
    u16 total_sector16;
    u8 media;
    u16 sector_p_fat16;
    u8 code[486];
    u16 boot_sig;
}sfat_bpb_t;
#endif 

#if SFATFS_BPB_VER==1
typedef struct FS_PACKED{
    u8 jump_boot[3];
    u8 oem_name[8];
    u16 byte_p_sector;
    u8 sector_p_cluster;
    u16 reserved_sectors;
    u8 num_fats;
    u16 root_entries;
    u16 total_sector16;
    u8 media;
    u16 sector_p_fat16;
    u16 sector_p_track;
    u16 num_heads;
    u32 hidden_sector;
    u32 total_sector32;
    u32 sector_per_fat32;
    u16 ext_flag;
    u16 fs_version;
    u32 root_cluster;
    u16 fs_info;
    u16 backup_boot_sector;
    u8 drive_number;
    u8 reserved1;
    u8 boot_signature;
    u32 volume_id;
    u8 volume_label[11];
    u8 fs_type[8];
    u8 code[432];
    u16 boot_sig;
}sfat_bpb_t;
#endif

#endif