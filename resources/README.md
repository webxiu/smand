# resources —— 图片资源目录

本目录存放 smand 的全部图片资源，经 **Qt 资源系统（rcc）编译进 `smand.exe` 内部**，
运行时不依赖任何外部文件，保持项目「轻量、免安装、单文件分发」的定位。

```
resources/
├── smand.qrc      # 资源清单：新增图片必须在此登记一行（已接入构建，无需改 CMake）
├── smand.rc       # 把 app.ico 以 win32 资源嵌入 exe，作为文件图标
├── app.ico        # exe 文件图标（多尺寸），由 scripts/make_ico.py 生成，勿手改
├── logo/          # 品牌 Logo 多倍图：关于页大图、窗口/托盘图标、exe 图标共用
│   ├── logo.png       # 16x16
│   ├── logo@2x.png    # 32x32
│   ├── logo@3x.png    # 64x64
│   ├── logo@4x.png    # 128x128
│   └── logo@5x.png    # 256x256（qrc 登记的就是它）
├── icons/         # UI 功能图标：菜单与按钮小图标，用于逐步替代 emoji 渲染
└── img/           # 文档配图（如 README 演示动图 view.gif），不编进 exe
```

## 当前 Logo 的接入情况

`logo/logo@5x.png`（256×256 透明底）已接入以下四处：

| 位置 | 代码 / 文件 | 说明 |
|------|-------------|------|
| 关于页大图 | `src/pages/aboutpage.cpp` | 256 源图 `scaled` 到 96 显示，平滑变换 |
| 窗口 / 任务栏 / 标题栏图标 | `src/main.cpp` | `app.setWindowIcon(":/logo/logo@5x.png")` |
| 系统托盘图标 | `src/mainwindow.cpp` | `QSystemTrayIcon(windowIcon(), ...)` 继承窗口图标，自动生效 |
| exe 文件图标 | `resources/smand.rc` | 走 win32 资源（`app.ico`），不走 qrc |

前三处经 `smand.qrc` 登记后以 `:/logo/logo@5x.png` 访问；exe 文件图标由
`CMakeLists.txt` 的 `enable_language(RC)` + `target_sources(resources/smand.rc)` 嵌入。

## 更换 Logo 的流程

1. 用设计稿重新导出 `logo/` 下 5 张多倍图（16/32/64/128/256，PNG-24 透明底，
   命名保持 `logo@Nx.png` 不变）
2. 重跑 `py scripts/make_ico.py` 刷新 `app.ico`
3. 重跑 `scripts/build.cmd`（或 `build-static.cmd`）

无需改任何 CMake / qrc / 源码——qrc 与代码引用的都是 `logo@5x.png` 这个固定文件名。

## 添加其它图片的三步

1. 把图片放进 `logo/` 或 `icons/`
2. 在 `smand.qrc` 的 `<qresource prefix="/">` 节点内登记一行，路径相对本目录、用正斜杠：

   ```xml
   <file>icons/system-settings.png</file>
   ```

3. 重新编译，代码中用 `:/` 前缀访问：`QPixmap(":/icons/system-settings.png")`

`<file>` 指向的文件必须真实存在，否则 rcc 会直接报错终止编译。

## 格式约束

> **优先使用 PNG，不要用 SVG。** 本项目只链接了 `Qt::Widgets`，SVG 解码依赖 `QtSvg`
> 模块与 svg 图像格式插件；静态编译（`scripts/build-static.cmd`）时若未显式导入插件，
> `QPixmap(":/logo/x.svg")` 会静默加载失败得到空图。确实需要 SVG 时，须先在
> `CMakeLists.txt` 的 `find_package` 与 `target_link_libraries` 中补上 `Svg` 组件。

`img/` 下的文件（如 1.6MB 的 `view.gif`）只供文档引用，**不要登记进 qrc**，
否则会被原样编进 exe 使体积暴涨。

## emoji 图标的现状

`src/utils/icons.cpp` 的 `Icons::emoji()` 仍被 `mainwindow`（左侧菜单、视图切换按钮）、
`launcherpage`（功能项列表）、`installedappspage`（应用图标兜底）使用，
在图标全部替换完成前不要删除。
