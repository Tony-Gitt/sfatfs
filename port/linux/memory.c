#include "sfatfs_types.h"
#include "sfatfs_port.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
// 直接套皮采用malloc等方法

void* s_malloc(size_t size){
    if(size==0) return NULL;
    return malloc(size);
}
void s_free(void* ptr){
    free(ptr);
}
void s_calloc(size_t nmemb,size_t size){
    // 分配并初始化内存，乘法溢出防安全漏洞
    if(nmemb==0||size==0) return NULL;
    if(size!=0&&nmemb>SIZE_MAX/size) return NULL;
    void* ptr=malloc(nmemb*size);
    if(ptr==NULL) return NULL;
    memset(ptr,0,nmemb*size);
    return ptr;
}