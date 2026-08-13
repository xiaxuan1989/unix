#[[
    Lib 构造文件
]]

cmake_minimum_required(VERSION 3.20)


string(TIMESTAMP build_time_iso8601 UTC)  # example: 2024-04-19T13:39Z
string(TIMESTAMP build_time_human_readable "%B %d, %Y %H:%M:%S %Z")
set(BUILD_TIME_ISO8601 ${build_time_iso8601})
set(BUILD_TIME_HUMAN_READABLE ${build_time_human_readable})

project(unix_this)


# 指定使用 C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


# 头文件路径。
include_directories("${PROJECT_SOURCE_DIR}/../include")
include_directories("${PROJECT_SOURCE_DIR}")


# GCC(g++) 编译选项。
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nostartfiles")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-builtin")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nostdinc")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-pic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-pie")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -no-pie")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nostdlib")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-stack-protector")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mno-red-zone")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mno-mmx")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mno-sse")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mno-sse2")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable")

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcmodel=large")  # 编译64位内核时需启用

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS}")

#[[ 
    项目构造产物
]]
file(GLOB_RECURSE CPP_SOURCE_FILES *.cpp)
file(GLOB_RECURSE C_SOURCE_FILES *.c)
add_library(
    unix_runtime STATIC
    
    ${CPP_SOURCE_FILES}
    ${C_SOURCE_FILES}
)
set_target_properties(
    unix_runtime PROPERTIES 
    COMPILE_FLAGS "-m32" 
    LINK_FLAGS "-m32"
)


