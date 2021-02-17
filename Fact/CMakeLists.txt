# cd cmake&cmake.exe -G "MinGW Makefiles" ..
cmake_minimum_required(VERSION 3.20)
# 项目信息
project (Fact)


# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)
# 添加 tests 子目录
add_subdirectory(tests)

# 添加 loggers 子目录
add_subdirectory(loggers)

# gdb调试
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")


# 指定生成目标
add_executable(Fact main.cpp)
# 添加链接库
target_link_libraries(Fact Fact-loggers)
target_link_libraries(Fact Fact-tests)