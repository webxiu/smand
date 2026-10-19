@echo off
rem ============================================================
rem  smand 一键编译脚本（VS2022 MSVC + Qt 6.8.3）
rem  用法：直接双击运行，或在命令行执行 build.cmd
rem ============================================================
setlocal

rem VS2022 编译环境初始化脚本
set "VCVARS=D:\apps\Visual Studio\Commumity\VC\Auxiliary\Build\vcvars64.bat"
rem Qt 安装位置
set "QT_DIR=D:\apps\Qt\6.8.3\msvc2022_64"
rem VS 自带的 CMake 和 Ninja
set "VS_CMAKE=D:\apps\Visual Studio\Commumity\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin"
set "VS_NINJA=D:\apps\Visual Studio\Commumity\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja"

if not exist "%VCVARS%" (
    echo [错误] 找不到 VS 的 vcvars64.bat: %VCVARS%
    exit /b 1
)
if not exist "%QT_DIR%\bin\qmake.exe" (
    echo [错误] 找不到 Qt: %QT_DIR%
    exit /b 1
)

set "PATH=%VS_CMAKE%;%VS_NINJA%;%PATH%"
call "%VCVARS%" >nul || (echo [错误] VS 环境初始化失败 & exit /b 1)

echo [1/2] 配置 CMake ...
cmake -S "%~dp0." -B "%~dp0build" -G Ninja ^
      -DCMAKE_BUILD_TYPE=Release ^
      -DCMAKE_PREFIX_PATH="%QT_DIR%" || exit /b 1

echo [2/2] 编译中 ...
cmake --build "%~dp0build" || exit /b 1

echo.
echo ============================================
echo  编译完成: %~dp0build\smand.exe
echo ============================================
endlocal
