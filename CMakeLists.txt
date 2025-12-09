cmake_minimum_required(VERSION 3.10)
project(sfatfs VERSION 0.1.0)

# C 标准设置
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/core/include
    ${CMAKE_CURRENT_SOURCE_DIR}/api
)

# 可执行文件：磁盘镜像创建工具
add_executable(mkfatimg tools/mkfatimg.c)

# 库：核心文件系统
add_library(fatfs_core STATIC
    core/src/metadata.c
    core/src/fattable.c
    core/src/cluster.c
    core/src/file.c
    core/src/dir.c
    core/src/path.c
)

# 库：POSIX 移植层
add_library(fatfs_port_posix STATIC
    port/posix/diskio.c
    port/posix/memory.c
    port/posix/time.c
)

# 库：API 层
add_library(fatfs_api STATIC
    api/fatfs_api.c
)

# 示例程序
add_executable(example_basic examples/basic.c)
target_link_libraries(example_basic 
    fatfs_api 
    fatfs_core 
    fatfs_port_posix
)

# 安装规则（可选）
install(TARGETS mkfatimg DESTINATION bin)
install(TARGETS example_basic DESTINATION bin)