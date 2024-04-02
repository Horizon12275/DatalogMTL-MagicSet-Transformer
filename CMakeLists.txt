cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)

project(DMT)

# 添加源文件
set(SOURCES 
    main.cpp
    lib/atom.cpp
    lib/interval.cpp
    lib/literal.cpp
    lib/rule.cpp
    lib/term.cpp
)

# 添加头文件目录
include_directories(classes)
include_directories(utils)

# 生成可执行文件
add_executable(DMT ${SOURCES})

# 如果有其他的库文件需要链接，可以使用target_link_libraries命令
# target_link_libraries(your_executable_name library_name)
