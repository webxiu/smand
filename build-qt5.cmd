@echo off
rem ============================================================
rem  smand 老系统兼容版编译脚本（Qt 5.15.2 + MSVC）
rem  适用：Win7 SP1 / Win8 / Win8.1 / Win10 初版 / Server 2012R2
rem  产物：build-qt5\smand.exe（需配合 package-qt5.cmd 打包分发）
rem ============================================================
setlocal

rem VS2022 编译环境初始化脚本
set "VCVARS=D:\apps\Visual Studio\Commumity\VC\Auxiliary\Build\vcvars64.bat"
rem Qt 5.15.2 安装位置（最后支持 Win7/8 的 LTS 版本）
set "QT5_DIR=D:\apps\Qt\5.15.2\msvc2019_64"
rem VS 自带的 CMake 和 Ninja
set "VS_CMAKE=D:\apps\Visual Studio\Commumity\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin"
set "VS_NINJA=D:\apps\Visual Studio\Commumity\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja"

if not exist "%VCVARS%" (
    echo [错误] 找不到 VS 的 vcvars64.bat: %VCVARS%
    exit /b 1
)
if not exist "%QT5_DIR%\lib\Qt5Core.lib" (
    echo [错误] 找不到 Qt 5.15.2: %QT5_DIR%
    echo 请先安装: python -m aqt install-qt windows desktop 5.15.2 win64_msvc2019_64 --outputdir D:\apps\Qt
    exit /b 1
)

set "PATH=%VS_CMAKE%;%VS_NINJA%;%PATH%"
call "%VCVARS%" >nul || (echo [错误] VS 环境初始化失败 & exit /b 1)

echo [1/2] 配置 CMake（Qt 5.15.2 兼容版）...
cmake -S "%~dp0." -B "%~dp0build-qt5" -G Ninja ^
      -DCMAKE_BUILD_TYPE=Release ^
      -DCMAKE_PREFIX_PATH="%QT5_DIR%" || exit /b 1

echo [2/2] 编译中 ...
cmake --build "%~dp0build-qt5" || exit /b 1

echo.
echo ============================================
echo  编译完成: %~dp0build-qt5\smand.exe
echo  分发请运行 package-qt5.cmd 生成兼容版 zip
echo ============================================
endlocal
