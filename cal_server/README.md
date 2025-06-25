# 计算器服务器项目

这是一个基于C++和CMake的网络计算器项目，包含服务器和客户端程序。

## 功能特性

- **网络通信**: 使用Windows Socket进行TCP通信
- **JSON协议**: 使用jsoncpp库进行数据序列化
- **多线程**: 服务器支持多客户端并发连接
- **跨平台构建**: 使用CMake和vcpkg进行依赖管理

## 项目结构

```
cal_server/
├── build/                  # 构建输出目录
│   ├── cal_server.exe     # 服务器可执行文件
│   └── cal_client.exe     # 客户端可执行文件
├── CMakeLists.txt         # CMake配置文件
├── ServerMain.cc          # 服务器主程序
├── ClientMain.cc          # 客户端主程序
├── ServerMain.hpp         # TCP服务器类
├── IOServer.hpp           # IO处理类
├── Protocol.hpp           # 通信协议
├── Socket.hpp             # Socket封装
├── Log.hpp                # 日志系统
├── NetCal.hpp             # 网络计算器
├── InetAddr.hpp           # 网络地址
├── ThreadPool.hpp         # 线程池
└── run_example.bat        # 演示脚本
```

## 编译要求

- **编译器**: MinGW-w64 (GCC 14.2.0+) 或 MSVC
- **CMake**: 3.10+
- **依赖库**: jsoncpp (通过vcpkg安装)

## 编译步骤

1. **安装依赖**:
   ```bash
   # 进入vcpkg目录
   cd ../vcpkg
   
   # 安装jsoncpp (MinGW版本)
   ./vcpkg install jsoncpp:x64-mingw-dynamic
   ```

2. **配置和编译**:
   ```bash
   # 回到项目目录
   cd cal_server
   
   # 配置CMake
   cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake -G "MinGW Makefiles"
   
   # 编译项目
   cmake --build build
   ```

## 使用方法

### 手动启动

1. **启动服务器**:
   ```bash
   .\build\cal_server.exe <端口号>
   # 例如: .\build\cal_server.exe 8080
   ```

2. **启动客户端**:
   ```bash
   .\build\cal_client.exe <服务器IP> <端口号>
   # 例如: .\build\cal_client.exe 127.0.0.1 8080
   ```

### 使用演示脚本

运行 `run_example.bat` 脚本会自动启动服务器和客户端。

## 通信协议

项目使用JSON格式进行通信：

**请求格式**:
```json
{
    "x": 10,
    "op": "+",
    "y": 5
}
```

**响应格式**:
```json
{
    "result": 15,
    "code": 0
}
```

## 支持的操作

- `+` 加法
- `-` 减法
- `*` 乘法
- `/` 除法

## 特性说明

1. **多线程服务器**: 每个客户端连接都在独立线程中处理
2. **异常处理**: 完善的错误处理和日志记录
3. **内存管理**: 使用智能指针进行内存管理
4. **协议封装**: 自定义协议格式 `"长度\r\n数据\r\n"`

## 编译问题解决

如果遇到编译问题：

1. **jsoncpp未找到**: 确保安装了正确版本的jsoncpp
2. **链接错误**: 确保使用MinGW版本的jsoncpp库
3. **pragma comment警告**: 已通过条件编译解决

## 许可证

MIT License 