@echo off
rem ============================================================
rem  smand 老系统兼容版打包脚本（Qt 5.15.2）
rem  产物：release\smand-v1.1.0-win64-legacy.zip
rem  适用：Win7 SP1 / Win8 / Win8.1 / Win10 初版 / Server 2012R2
rem  （新系统请用 package.cmd 或 build-static.cmd 的产物）
rem ============================================================
setlocal

rem 项目根目录（本脚本位于 scripts\ 子目录，回退一级即根目录）
pushd "%~dp0.."
set "ROOT=%CD%"
popd

rem Qt 5.15.2 安装位置
set "QT5_DIR=D:\apps\Qt\5.15.2\msvc2019_64"
rem VC++ 运行库位置（exe 同目录自动加载，对方机器免安装运行库）
set "VC_CRT_DIR=D:\apps\Visual Studio\Commumity\VC\Redist\MSVC\14.44.35112\x64\Microsoft.VC143.CRT"
rem 版本号（与 CMakeLists.txt 的 project VERSION 保持一致）
set "VERSION=1.1.0"
set "ZIP_NAME=smand-v%VERSION%-win64-legacy.zip"

if not exist "%QT5_DIR%\bin\windeployqt.exe" (
    echo [错误] 找不到 Qt5 windeployqt: %QT5_DIR%\bin\windeployqt.exe
    exit /b 1
)
if not exist "%VC_CRT_DIR%\vcruntime140.dll" (
    echo [错误] 找不到 VC++ 运行库: %VC_CRT_DIR%
    exit /b 1
)

echo [1/5] 编译老系统兼容版（Qt 5.15.2）...
call "%~dp0build-qt5.cmd" || exit /b 1

echo [2/5] 清空并重建 dist-qt5 目录 ...
if exist "%ROOT%\dist-qt5" rmdir /s /q "%ROOT%\dist-qt5"
mkdir "%ROOT%\dist-qt5"
copy /y "%ROOT%\build-qt5\smand.exe" "%ROOT%\dist-qt5\smand.exe" >nul || (
    echo [错误] 拷贝 smand.exe 失败，程序可能正在运行，请先退出
    exit /b 1
)

echo [3/5] 部署 Qt5 运行库 ...
rem --no-translations        不拷 Qt 翻译文件
rem --no-angle               不拷 ANGLE 库（libEGL/libGLESv2）
rem --no-system-d3d-compiler 不拷 d3dcompiler_47.dll
rem --no-opengl-sw           不拷软件 OpenGL 兜底 opengl32sw.dll
"%QT5_DIR%\bin\windeployqt.exe" --release ^
    --no-translations --no-angle --no-system-d3d-compiler --no-opengl-sw ^
    "%ROOT%\dist-qt5\smand.exe" || exit /b 1

echo [4/5] 拷贝 VC++ 运行库（vcruntime140 等，目标机器无需安装）...
copy /y "%VC_CRT_DIR%\*.dll" "%ROOT%\dist-qt5\" >nul || exit /b 1

rem UCRT：Win10 自带；Win7 SP1/Win8.1 不自带，须 app-local 部署（ucrtbase + api-ms-win-crt-*）
copy /y "D:\Windows Kits\10\Redist\10.0.26100.0\ucrt\DLLs\x64\*.dll" "%ROOT%\dist-qt5\" >nul || exit /b 1

echo [5/5] 压缩成 zip ...
if not exist "%ROOT%\release" mkdir "%ROOT%\release"
if exist "%ROOT%\release\%ZIP_NAME%" del /q "%ROOT%\release\%ZIP_NAME%"
powershell -NoProfile -Command "Compress-Archive -Path '%ROOT%\dist-qt5\*' -DestinationPath '%ROOT%\release\%ZIP_NAME%' -Force" || exit /b 1

echo.
echo ============================================
echo  打包完成:
echo    老系统兼容包: %ROOT%\release\%ZIP_NAME%
echo  适用 Win7 SP1 / Win8 / Win8.1 / Win10 全版本
echo ============================================
endlocal
