# cd cmake&cmake.exe -G "MinGW Makefiles" ..
cmake_minimum_required(VERSION 3.20)
# 项目信息
project (Fact)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 14)

# gdb调试
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# 包含目录
include_directories(
	. loggers memories 
	tests tests/loggers tests/memories
)
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_ROOT 变量
aux_source_directory(. DIR_ROOT)
aux_source_directory(primes DIR_PRIMES)
aux_source_directory(memories DIR_MEMORIES)
aux_source_directory(tests DIR_TESTS)
aux_source_directory(tests/memories DIR_TESTS_MEMORIES)
aux_source_directory(tests/primes DIR_TESTS_PRIMES)

# 指定生成目标
add_executable(Fact ${DIR_ROOT} 
	${DIR_PRIMES} ${DIR_MEMORIES} ${DIR_TESTS} 
	${DIR_TESTS_MEMORIES}  ${DIR_TESTS_PRIMES} ${DIR_TESTS_PRIMES}
)

# 添加 tests 子目录 链接用
# add_subdirectory(tests)
# add_subdirectory(loggers)
# 添加链接库
# target_link_libraries(Fact Fact-loggers Fact-tests)