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
file(GLOB SOURCES "*.cc")

# 添加可执行文件
add_executable(${EXECUTABLE_NAME} ${SOURCES})

# 将当前目录添加到头文件搜索路径
target_include_directories(${EXECUTABLE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 查找并链接 jsoncpp 库
find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE JsonCpp::JsonCpp)

# --- 链接平台相关的库 ---

# 为多线程支持链接 Threads 库
find_package(Threads REQUIRED)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE Threads::Threads)

# 如果是 Windows 系统 (WIN32)，链接 winsock 库 (ws2_32)
if(WIN32)
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE ws2_32)
endif()

# 打印一条消息，提示用户如何运行
add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E echo "编译完成! 运行可执行文件: ./${EXECUTABLE_NAME}")

# (可选) 添加安装指令
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin) 