@echo off
setlocal

:: 检查并删除 build 目录
if exist build (
    rmdir /s /q build
)

:: 检查并删除 bin 目录
if exist bin (
    rmdir /s /q bin
)

:: 创建 bin 目录
mkdir bin

:: 创建 build 目录并切换到该目录
mkdir build
cd build

cmake -G "MinGW Makefiles" ..

:: 执行 make 命令
make

endlocal
