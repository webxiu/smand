@echo off
rem ============================================================
rem  smand 静态编译脚本：产出零依赖单文件 exe（约 17MB）
rem
rem  用法（任选其一）：
rem    1. 直接双击本文件运行（最方便），跑完按任意键关闭窗口
rem    2. 命令行执行: scripts\build-static.cmd
rem  产物：build-static\smand.exe
rem    分发：就这一个文件，发给对方即可双击运行
rem
rem  前置条件：本机已编译安装静态 Qt
rem    （D:\apps\Qt\6.8.3\msvc2022_64_static 已就绪）
rem ============================================================
setlocal

rem 项目根目录（本脚本位于 scripts\ 子目录，回退一级即根目录）
pushd "%~dp0.."
set "ROOT=%CD%"
popd

rem VS2022 编译环境初始化脚本
set "VCVARS=D:\apps\Visual Studio\Commumity\VC\Auxiliary\Build\vcvars64.bat"
rem 静态版 Qt 安装位置
set "QT_STATIC_DIR=D:\apps\Qt\6.8.3\msvc2022_64_static"
rem VS 自带的 CMake 和 Ninja
set "VS_CMAKE=D:\apps\Visual Studio\Commumity\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin"
set "VS_NINJA=D:\apps\Visual Studio\Commumity\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja"

if not exist "%VCVARS%" (
    echo [错误] 找不到 VS 的 vcvars64.bat: %VCVARS%
    goto :fail
)
if not exist "%QT_STATIC_DIR%\lib\Qt6Core.lib" (
    echo [错误] 找不到静态 Qt: %QT_STATIC_DIR%
    echo 请参考 README 重新编译安装静态 Qt
    goto :fail
)

echo [1/3] 初始化 VS 编译环境 ...
set "PATH=%VS_CMAKE%;%VS_NINJA%;%PATH%"
call "%VCVARS%" >nul
if errorlevel 1 (
    echo [错误] VS 环境初始化失败
    goto :fail
)

echo [2/3] 配置 CMake（静态 Qt + /MT 静态运行库）...
cmake -S "%ROOT%" -B "%ROOT%\build-static" -G Ninja ^
      -DCMAKE_BUILD_TYPE=Release ^
      -DCMAKE_PREFIX_PATH="%QT_STATIC_DIR%" ^
      -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded || goto :fail

echo [3/3] 编译中 ...
cmake --build "%ROOT%\build-static" || goto :fail

echo.
echo ============================================
echo  编译产物: %ROOT%\build-static\smand.exe
echo  单文件，约 17MB，可直接发给对方双击运行
echo ============================================
goto :done

:fail
echo.
echo [失败] 编译未完成，请查看上方错误信息。

:done
rem 双击运行时停留窗口查看结果，命令行传 --nopause 可跳过
if "%~1"=="" pause
endlocal
