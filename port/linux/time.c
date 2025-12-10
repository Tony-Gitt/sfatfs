#include<time.h>
#include<string.h>
#include "sfatfs_types.h"
// 主要实现date、time两个量的转换

fs_result_t s_get_time(sfat_date_t* s_date,sfat_time_t* s_time){
    time_t now;
    now=time(NULL);
    struct tm* tm_info;
    tm_info=localtime(&now);
    if(!s_date || !s_time) return FS_INVALID;
    if(now==(time_t)-1||!tm_info){
        s_date->year=0;
        s_date->month=1;
        s_date->day=1;
        s_time->hour=0;
        s_time->minute=0;
        s_time->second=0;
        return FS_OK;
    }
    s_date->year=(tm_info->tm_year>=1980)?(tm_info->tm_year-1980):0;
    s_date->month=tm_info->tm_mon+1;
    s_date->day=tm_info->tm_mday;
    s_time->hour=tm_info->tm_hour;
    s_time->minute=tm_info->tm_min;
    s_time->second=tm_info->tm_sec/2;
    return FS_OK;
}