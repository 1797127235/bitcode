@echo off
chcp 65001 > nul
echo 计算器服务器/客户端演示
echo.
echo 使用方法:
echo   服务器: .\build\cal_server.exe 8888
echo   客户端: .\build\cal_client.exe 127.0.0.1 8888
echo.
echo 启动服务器在端口8080...
start "Calculator Server" cmd /k .\build\cal_server.exe 8888
echo.
echo 等待3秒让服务器启动...
timeout /t 3 /nobreak > nul
echo.
echo 启动客户端连接到服务器...
start "Calculator Client" cmd /k .\build\cal_client.exe 127.0.0.1 8888
echo.
echo 两个程序都已在新窗口中启动！
pause 