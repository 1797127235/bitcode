# 设置 CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)

# 定义项目名称和语言
project(CalServer CXX)

# 设置 C++ 标准为 C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 启用编译器警告
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# 将当前目录添加到头文件搜索路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# --- 查找依赖库 ---

# 查找线程库
find_package(Threads REQUIRED)

# 查找 jsoncpp - 首先尝试使用 CONFIG 模式
find_package(jsoncpp CONFIG QUIET)
if(jsoncpp_FOUND OR TARGET JsonCpp::JsonCpp)
    set(JSONCPP_FOUND TRUE)
    set(JSONCPP_LIBRARIES JsonCpp::JsonCpp)
    message(STATUS "通过 CONFIG 找到 jsoncpp")
else()
    # 手动查找 jsoncpp (vcpkg 安装位置)
    message(STATUS "尝试手动查找 jsoncpp...")
    
    # 查找头文件 - 优先使用MinGW版本
    find_path(JSONCPP_INCLUDE_DIR json/json.h 
              PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../vcpkg/installed/x64-mingw-dynamic/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/../vcpkg/installed/x64-mingw-static/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/../vcpkg/installed/x64-windows/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/../vcpkg_installed/x64-mingw-dynamic/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/../vcpkg_installed/x64-windows/include
                    $ENV{VCPKG_ROOT}/installed/x64-mingw-dynamic/include
                    $ENV{VCPKG_ROOT}/installed/x64-windows/include
              DOC "jsoncpp 头文件目录")
    
    # 查找库文件 - 优先使用MinGW版本
    find_library(JSONCPP_LIBRARY 
                 NAMES jsoncpp jsoncpp_lib json_cpp libjsoncpp
                 PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../vcpkg/installed/x64-mingw-dynamic/lib
                       ${CMAKE_CURRENT_SOURCE_DIR}/../vcpkg/installed/x64-mingw-static/lib
                       ${CMAKE_CURRENT_SOURCE_DIR}/../vcpkg/installed/x64-windows/lib
                       ${CMAKE_CURRENT_SOURCE_DIR}/../vcpkg_installed/x64-mingw-dynamic/lib
                       ${CMAKE_CURRENT_SOURCE_DIR}/../vcpkg_installed/x64-windows/lib
                       $ENV{VCPKG_ROOT}/installed/x64-mingw-dynamic/lib
                       $ENV{VCPKG_ROOT}/installed/x64-windows/lib
                 DOC "jsoncpp 库文件")
    
    if(JSONCPP_INCLUDE_DIR AND JSONCPP_LIBRARY)
        set(JSONCPP_FOUND TRUE)
        set(JSONCPP_LIBRARIES ${JSONCPP_LIBRARY})
        set(JSONCPP_INCLUDE_DIRS ${JSONCPP_INCLUDE_DIR})
        message(STATUS "手动找到 jsoncpp:")
        message(STATUS "  头文件: ${JSONCPP_INCLUDE_DIR}")
        message(STATUS "  库文件: ${JSONCPP_LIBRARY}")
    else()
        message(STATUS "未找到 jsoncpp 头文件或库文件")
        message(STATUS "  头文件搜索结果: ${JSONCPP_INCLUDE_DIR}")
        message(STATUS "  库文件搜索结果: ${JSONCPP_LIBRARY}")
    endif()
endif()

# --- 服务器可执行文件 ---
add_executable(cal_server ServerMain.cc)

# 添加当前目录的json子目录到包含路径
target_include_directories(cal_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# 链接线程库
target_link_libraries(cal_server PRIVATE Threads::Threads)

# Windows 特定链接
if(WIN32)
    target_link_libraries(cal_server PRIVATE ws2_32)
endif()

# --- 客户端可执行文件 ---
add_executable(cal_client ClientMain.cc)

# 添加当前目录的json子目录到包含路径  
target_include_directories(cal_client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# 链接线程库
target_link_libraries(cal_client PRIVATE Threads::Threads)

# Windows 特定链接
if(WIN32)
    target_link_libraries(cal_client PRIVATE ws2_32)
endif()

# --- 如果找到 jsoncpp，则链接它 ---
if(JSONCPP_FOUND)
    message(STATUS "找到 jsoncpp，启用 JSON 支持")
    target_compile_definitions(cal_server PRIVATE ENABLE_JSON=1)
    target_compile_definitions(cal_client PRIVATE ENABLE_JSON=1)
    
    if(TARGET JsonCpp::JsonCpp)
        target_link_libraries(cal_server PRIVATE JsonCpp::JsonCpp)
        target_link_libraries(cal_client PRIVATE JsonCpp::JsonCpp)
    else()
        target_link_libraries(cal_server PRIVATE ${JSONCPP_LIBRARIES})
        target_link_libraries(cal_client PRIVATE ${JSONCPP_LIBRARIES})
        target_include_directories(cal_server PRIVATE ${JSONCPP_INCLUDE_DIRS})
        target_include_directories(cal_client PRIVATE ${JSONCPP_INCLUDE_DIRS})
    endif()
else()
    message(WARNING "未找到 jsoncpp 库，将使用简化的协议")
    target_compile_definitions(cal_server PRIVATE ENABLE_JSON=0)
    target_compile_definitions(cal_client PRIVATE ENABLE_JSON=0)
endif()

# 打印编译信息
message(STATUS "配置完成:")
message(STATUS "  - 服务器: cal_server")
message(STATUS "  - 客户端: cal_client")
message(STATUS "  - C++ 标准: ${CMAKE_CXX_STANDARD}")
message(STATUS "  - jsoncpp 支持: ${JSONCPP_FOUND}")

# 添加自定义命令显示如何运行
add_custom_command(TARGET cal_server POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E echo "编译完成! 使用方法:")
add_custom_command(TARGET cal_server POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E echo "  服务器: ./cal_server <port>")
add_custom_command(TARGET cal_client POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E echo "  客户端: ./cal_client <server_ip> <server_port>")

# 安装目标
install(TARGETS cal_server cal_client DESTINATION bin) 