@echo off
cd bin

REM 检查是否包含 -t 参数
setlocal enabledelayedexpansion
set "args=%*"
if "!args!"==" " (
    set "args="
)
echo %args% | findstr /C:"-t" >nul
if %errorlevel%==0 (
    call stress_test
) else (
    call menu
)
