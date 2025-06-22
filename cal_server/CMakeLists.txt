# 设置 CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)

# 定义项目名称和语言
project(CalServer CXX)

# 设置 C++ 标准为 C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 定义可执行文件名称
set(EXECUTABLE_NAME cal_server_app)

# 查找当前目录下的所有源文件 (.cc)
# 使用 file(GLOB ...) 来自动包含目录中的所有源文件
file(GLOB SOURCES "*.cc")

# 添加可执行文件
add_executable(${EXECUTABLE_NAME} ${SOURCES})

# 将当前目录添加到头文件搜索路径
# 这样 #include "SomeHeader.hpp" 就能正确找到文件
target_include_directories(${EXECUTABLE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# --- 链接平台相关的库 ---

# 为多线程支持链接 Threads 库
find_package(Threads REQUIRED)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE Threads::Threads)

# 如果是 Windows 系统 (WIN32)，链接 winsock 库 (ws2_32)
if(WIN32)
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE ws2_32)
endif()


# (可选) 添加安装指令
# 将安装目录设置为项目根目录下的 bin 文件夹
install(TARGETS ${EXECUTABLE_NAME} DESTINATION ${CMAKE_BINARY_DIR}/bin) 