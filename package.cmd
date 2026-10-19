@echo off
rem ============================================================
rem  smand 一键打包脚本：编译 + 部署 Qt 运行库 + 压缩成可分发 zip
rem  用法：直接双击运行，或在命令行执行 package.cmd
rem  产物：smand-vX.X.X-win64.zip —— 发给别人，解压即可运行
rem ============================================================
setlocal

rem Qt 安装位置（windeployqt 在其 bin 目录下）
set "QT_DIR=D:\apps\Qt\6.8.3\msvc2022_64"
rem VC++ 运行库位置（exe 同目录自动加载，对方机器免安装运行库）
set "VC_CRT_DIR=D:\apps\Visual Studio\Commumity\VC\Redist\MSVC\14.44.35112\x64\Microsoft.VC143.CRT"
rem 版本号（与 CMakeLists.txt 的 project VERSION 保持一致）
set "VERSION=1.1.0"
set "ZIP_NAME=smand-v%VERSION%-win64.zip"

if not exist "%QT_DIR%\bin\windeployqt.exe" (
    echo [错误] 找不到 windeployqt: %QT_DIR%\bin\windeployqt.exe
    exit /b 1
)
if not exist "%VC_CRT_DIR%\vcruntime140.dll" (
    echo [错误] 找不到 VC++ 运行库: %VC_CRT_DIR%
    exit /b 1
)

echo [1/5] 先编译最新版 ...
call "%~dp0build.cmd" || exit /b 1

echo [2/5] 清空并重建 dist 目录 ...
if exist "%~dp0dist" rmdir /s /q "%~dp0dist"
mkdir "%~dp0dist"
copy /y "%~dp0build\smand.exe" "%~dp0dist\smand.exe" >nul || (
    echo [错误] 拷贝 smand.exe 失败，程序可能正在运行，请先退出
    exit /b 1
)

echo [3/5] 部署 Qt 运行库 ...
rem --no-translations         不拷 Qt 翻译文件（界面文字已内置）
rem --no-system-dxc-compiler  不拷 dxcompiler.dll/dxil.dll（Widgets 程序用不到）
rem --no-system-d3d-compiler  不拷 d3dcompiler_47.dll（Widgets 程序用不到）
rem --no-opengl-sw            不拷软件 OpenGL 兜底 opengl32sw.dll（省约 20MB）
rem --no-compiler-runtime     不拷 vc_redist 安装器（第 4 步改为直接拷 CRT DLL）
"%QT_DIR%\bin\windeployqt.exe" --release ^
    --no-translations --no-system-dxc-compiler --no-system-d3d-compiler ^
    --no-opengl-sw --no-compiler-runtime ^
    "%~dp0dist\smand.exe" || exit /b 1

echo [4/5] 拷贝 VC++ 运行库（vcruntime140 等，目标机器无需安装）...
copy /y "%VC_CRT_DIR%\*.dll" "%~dp0dist\" >nul || exit /b 1

echo [5/5] 压缩成 zip ...
if exist "%~dp0%ZIP_NAME%" del /q "%~dp0%ZIP_NAME%"
powershell -NoProfile -Command "Compress-Archive -Path '%~dp0dist\*' -DestinationPath '%~dp0%ZIP_NAME%' -Force" || exit /b 1

echo.
echo ============================================
echo  打包完成:
echo    可分发压缩包: %~dp0%ZIP_NAME%
echo    解压后目录:   dist\
echo  发送 zip 给对方，解压后双击 smand.exe 即可运行
echo ============================================
endlocal
