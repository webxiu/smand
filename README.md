# smand - 系统快捷管理器

基于 **Qt 6.8.3 (MSVC 2022 x64) + CMake** 的 Windows 桌面工具。
左侧菜单分类、右侧按钮网格，一键打开系统设置、管理工具窗口和常用快捷操作。

## 界面结构

```
┌────────────┬──────────────────────────────┐
│ 系统设置   │  🖥️      🔊      🔔      🔋   │
│ 管理工具   │  显示    声音    通知   电源   │
│ 快捷操作   │  💾      🪟      📶      🖨️   │
│            │  存储   多任务   蓝牙   打印机 │
│            │  ...（滚动查看更多）...        │
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

危险操作（关机/重启/注销）点击后会先弹确认框。

## 如何编译

### 方式一：Qt Creator（推荐）

1. 打开 `D:\apps\Qt\Tools\QtCreator\bin\qtcreator.exe`
2. 文件 → 打开文件或项目 → 选择本目录的 `CMakeLists.txt`
3. 若 Kit 未自动检测，到 工具 → 选项 → Kits：
   - Qt Versions 确认有 `Qt 6.8.3 (D:\apps\Qt\6.8.3\msvc2022_64)`
   - 编译器确认有 `Microsoft Visual C++ Compiler 19.44 (amd64)`
4. 选择 `Desktop Qt 6.8.3 MSVC2022 64bit` Kit，点左下角 ▶ 运行

### 方式二：命令行

```powershell
# 直接双击 build.cmd，或在 PowerShell 里：
& "e:\develop\server\c++\smand\build.cmd"
```

产物在 `build\smand.exe`。

## 如何添加新功能项

编辑 [appdata.cpp](appdata.cpp)，在对应分组的 `items` 里加一行：

```cpp
{ QStringLiteral("按钮文字"), QStringLiteral("😀"),
  QStringLiteral("start xxx"),      // cmd /c 执行的命令
  QStringLiteral("状态栏提示文字") },
```

重新编译即可，无需改动任何界面代码。

## 文件说明

| 文件 | 职责 |
|------|------|
| `main.cpp` | 程序入口 + 全局 QSS 样式 |
| `mainwindow.h/cpp` | 主窗口：左侧菜单 QListWidget + 右侧 QStackedWidget |
| `launcherpage.h/cpp` | 通用按钮网格页 + 点击执行逻辑 |
| `appdata.h/cpp` | 全部功能项的数据表（改这里加功能） |
| `icons.h/cpp` | emoji 渲染成 QPixmap 图标 |
| `build.cmd` | 命令行一键编译脚本 |

## 环境信息（本机）

- Qt 6.8.3：`D:\apps\Qt\6.8.3\msvc2022_64`
- Qt Creator：`D:\apps\Qt\Tools\QtCreator\bin\qtcreator.exe`
- VS 2022 Community（MSVC 编译器）：`D:\apps\Visual Studio\Commumity`
- Windows 11 SDK 26100：`D:\Windows Kits\10`（提供 rc.exe / 系统库）

## 直接运行

`build\smand.exe` 已用 windeployqt 部署 Qt 运行库（build 目录内的 dll），
双击即可运行，无需其他依赖；把整个 build 目录拷到别的 Windows 机器也能跑。
