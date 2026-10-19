# smand - 系统快捷管理器

基于 **Qt 6.8.3 (MSVC 2022 x64) + CMake** 的 Windows 桌面工具。
左侧菜单分类、右侧功能列表，一键打开系统设置、管理工具窗口和常用快捷操作，
还能列出本机安装的应用（含真实图标），双击即可打开；关闭窗口时最小化到系统托盘。

## 界面结构

```
┌────────────┬──────────────────────────────┐
│ 系统设置   │  🖥️      🔊      🔔      🔋   │
│ 管理工具   │  显示    声音    通知   电源   │
│ 快捷操作   │  💾      🪟      📶      🖨️   │
│ 安装应用   │  存储   多任务   蓝牙   打印机 │
│ 关于       │  ...（右上角按钮可切换宫格/列表）... │
├────────────┴──────────────────────────────┤
│ 状态栏：显示每次操作的反馈                  │
└───────────────────────────────────────────┘
```

## 功能分类

| 菜单 | 内容 |
|------|------|
| 系统设置 | Windows 设置应用各页面（显示/声音/网络/账户/更新等 24 项） |
| 管理工具 | services.msc、任务管理器、注册表、组策略等 MMC 管理窗口 22 项 |
| 快捷操作 | 控制面板、CMD、PowerShell、此电脑、关机/重启/锁屏等 23 项 |
| 安装应用 | 自动扫描注册表列出本机已安装应用（含真实 exe 图标），双击打开 |
| 关于 | 应用功能介绍、作者与联系方式 |

其他特性：

- 右上角按钮可在**宫格 / 列表**两种布局间切换，选择会记住（下次启动沿用）
- 点窗口关闭按钮 = **隐藏到系统托盘**；双击托盘图标恢复窗口，右键托盘图标可退出
- 危险操作（关机/重启/注销）点击后会先弹确认框

## 目录结构

```
smand/
├── CMakeLists.txt          # 构建配置（Qt6 优先，自动兼容 Qt5.15）
├── README.md
├── src/                     # 全部源码
│   ├── main.cpp             # 程序入口 + 全局 QSS 样式 + 应用图标
│   ├── mainwindow.h/cpp     # 主窗口：左侧菜单 + 右侧内容页、布局切换、系统托盘
│   ├── pages/               # 各功能页
│   │   ├── listpage.h/cpp           # 内容页基类：同一份数据支持宫格/列表双视图
│   │   ├── launcherpage.h/cpp       # 静态功能项页面（三分组共用）+ 点击执行逻辑
│   │   ├── installedappspage.h/cpp  # 已安装应用页：注册表扫描 + exe 图标 + 双击打开
│   │   └── aboutpage.h/cpp          # 关于页：功能介绍、作者与联系方式
│   ├── data/
│   │   └── appdata.h/cpp    # 全部功能项的数据表（改这里加功能）
│   └── utils/
│       └── icons.h/cpp      # emoji 渲染成 QPixmap 图标
├── scripts/                 # 构建 / 打包脚本
│   ├── build.cmd            # Qt6 动态编译 → build\smand.exe
│   ├── build-static.cmd     # Qt6 静态编译 → build-static\smand.exe（单文件）
│   ├── build-qt5.cmd        # Qt5.15 兼容版编译 → build-qt5\smand.exe
│   ├── package.cmd          # 打包标准分发 zip → release\
│   └── package-qt5.cmd      # 打包老系统兼容 zip → release\
└── release/                 # 分发 zip 产物（不入库）
```

> 源码内的 `#include "xxx.h"` 无需带子目录前缀——`CMakeLists.txt` 已把 `src`、
> `src/pages`、`src/data`、`src/utils` 全部加入头文件搜索路径（AUTOMOC 同样依赖此设置）。

## 如何编译

### 方式〇：静态编译（产物为单文件 exe，推荐用于分发）

```powershell
& "e:\develop\server\c++\smand\scripts\build-static.cmd"
```

- 产物：`build-static\smand.exe`（约 17MB 单文件，零依赖）
- Qt 与 VC 运行库全部链接进 exe 内部，可单独发送给他人，双击即运行
- 前置条件：本机已编译安装静态版 Qt（见下方「静态 Qt 构建环境」）

### 方式一：Qt Creator

1. 打开 `D:\apps\Qt\Tools\QtCreator\bin\qtcreator.exe`
2. 文件 → 打开文件或项目 → 选择本目录的 `CMakeLists.txt`
3. 若 Kit 未自动检测，到 工具 → 选项 → Kits：
   - Qt Versions 确认有 `Qt 6.8.3 (D:\apps\Qt\6.8.3\msvc2022_64)`
   - 编译器确认有 `Microsoft Visual C++ Compiler 19.44 (amd64)`
4. 选择 `Desktop Qt 6.8.3 MSVC2022 64bit` Kit，点左下角 ▶ 运行

### 方式二：命令行（动态编译）

```powershell
# 直接双击 scripts\build.cmd，或在 PowerShell 里：
& "e:\develop\server\c++\smand\scripts\build.cmd"
```

产物在 `build\smand.exe`。

## 如何添加新功能项

编辑 [src/data/appdata.cpp](src/data/appdata.cpp)，在对应分组的 `items` 里加一行：

```cpp
{ QStringLiteral("按钮文字"), QStringLiteral("😀"),
  QStringLiteral("start xxx"),      // cmd /c 执行的命令
  QStringLiteral("状态栏提示文字") },
```

重新编译即可，无需改动任何界面代码。

## 环境信息（本机）

- Qt 6.8.3：`D:\apps\Qt\6.8.3\msvc2022_64`
- Qt 6.8.3 静态版：`D:\apps\Qt\6.8.3\msvc2022_64_static`
- Qt Creator：`D:\apps\Qt\Tools\QtCreator\bin\qtcreator.exe`
- VS 2022 Community（MSVC 编译器）：`D:\apps\Visual Studio\Commumity`
- Windows 11 SDK 26100：`D:\Windows Kits\10`（提供 rc.exe / 系统库）

### 静态 Qt 构建环境（已搭好，一次性）

本机已从 qtbase 6.8.3 源码编译安装静态版 Qt（`-static -static-runtime -release`，
跳过 examples/tests/dbus），供 `scripts\build-static.cmd` 使用。如需重建：

```powershell
# 源码与构建缓存在 D:\Qt-static（约 6GB，静态 Qt 装好后可删掉省空间）
cd D:\Qt-static\build-static
& "D:\apps\Visual Studio\Commumity\Common7\Tools\Launch-VsDevShell.ps1" -Arch amd64
cmake --build . --parallel; cmake --install .
```

## 直接运行 / 分发给他人

**方式一（推荐）：单文件 exe** —— 运行 `scripts\build-static.cmd`，产物
`build-static\smand.exe`（约 17MB）把 Qt 和 VC 运行库全部静态链接进内部，
依赖只剩 Windows 系统 DLL。**直接把这一个 exe 发给对方，双击即运行**，
不用解压、不用安装任何东西。

**方式二：zip 包** —— 动态编译版（`scripts\build.cmd`）不能只发 exe，
需运行 `scripts\package.cmd` 打包：

1. 自动完成：编译 → 部署 Qt 运行库 → 拷贝 VC++ 运行库（vcruntime140 等）→ 压缩 zip
2. 产物：`release\smand-v1.1.0-win64.zip`（约 12MB，目标机器零依赖，无需安装任何东西）
3. 把 zip 发给对方，解压后双击 `smand.exe` 即可运行

若对方下载后仍无法启动，右键文件 → 属性 → 勾选「解除锁定」后再运行
（Windows 会拦截网上下载的文件）。

## 老系统兼容包（Win7 / Win8 / 旧 Win10 / 云电脑环境）

Qt 6 版程序要求 **Windows 10 1607+**。若对方运行报
「无法定位程序输入点 SetThreadDescription」，说明其系统更老
（Win7 / Win8.1 / 2015 年的 Win10 初版，或基于旧内核的云电脑/虚拟机环境），
请改发 Qt 5.15.2 兼容包：

```powershell
& "e:\develop\server\c++\smand\scripts\package-qt5.cmd"
```

- 产物：`release\smand-v1.1.0-win64-legacy.zip`（约 10MB）
- 适用：Win7 SP1 / Win8 / Win8.1 / Win10 全版本 / Server 2012R2+
- 已内置 UCRT（ucrtbase 等 40+ 个 DLL）与 VC++ 运行库，老机器零依赖

三个分发包怎么选：

| 包 | 生成命令 | 适用环境 |
|----|----------|----------|
| 单文件 exe（16MB） | `scripts\build-static.cmd` | Win10 1607+ 及更新系统 |
| 标准包（12MB） | `scripts\package.cmd` | Win10 1607+ 及更新系统 |
| 兼容包（10MB） | `scripts\package-qt5.cmd` | Win7 SP1 ~ Win11 全覆盖 |
