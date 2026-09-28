#include "appdata.h"

namespace AppData {

QList<MenuEntry> menus()
{
    QList<MenuEntry> list;

    // ============ 1. 系统设置（Windows 10/11 设置应用的各页面） ============
    MenuEntry settings;
    settings.title = QStringLiteral("系统设置");
    settings.emoji = QStringLiteral("🛠️");
    settings.items = {
        { QStringLiteral("显示"),           QStringLiteral("🖥️"), QStringLiteral("start ms-settings:display"),                QStringLiteral("打开：设置 - 显示") },
        { QStringLiteral("声音"),           QStringLiteral("🔊"), QStringLiteral("start ms-settings:sound"),                  QStringLiteral("打开：设置 - 声音") },
        { QStringLiteral("通知"),           QStringLiteral("🔔"), QStringLiteral("start ms-settings:notifications"),         QStringLiteral("打开：设置 - 通知") },
        { QStringLiteral("电源和电池"),     QStringLiteral("🔋"), QStringLiteral("start ms-settings:powersleep"),            QStringLiteral("打开：设置 - 电源和电池") },
        { QStringLiteral("存储"),           QStringLiteral("💾"), QStringLiteral("start ms-settings:storagesense"),          QStringLiteral("打开：设置 - 存储") },
        { QStringLiteral("多任务"),         QStringLiteral("🪟"), QStringLiteral("start ms-settings:multitasking"),          QStringLiteral("打开：设置 - 多任务") },
        { QStringLiteral("蓝牙和其他设备"), QStringLiteral("📶"), QStringLiteral("start ms-settings:bluetooth"),             QStringLiteral("打开：设置 - 蓝牙和其他设备") },
        { QStringLiteral("打印机和扫描仪"), QStringLiteral("🖨️"), QStringLiteral("start ms-settings:printers"),              QStringLiteral("打开：设置 - 打印机和扫描仪") },
        { QStringLiteral("手机连接"),       QStringLiteral("📱"), QStringLiteral("start ms-settings:mobile-devices"),        QStringLiteral("打开：设置 - 手机连接") },
        { QStringLiteral("网络和 Internet"), QStringLiteral("🌐"), QStringLiteral("start ms-settings:network"),               QStringLiteral("打开：设置 - 网络和 Internet") },
        { QStringLiteral("个性化"),         QStringLiteral("🎨"), QStringLiteral("start ms-settings:personalization"),       QStringLiteral("打开：设置 - 个性化") },
        { QStringLiteral("背景"),           QStringLiteral("🖼️"), QStringLiteral("start ms-settings:personalization-background"), QStringLiteral("打开：设置 - 背景") },
        { QStringLiteral("应用"),           QStringLiteral("📦"), QStringLiteral("start ms-settings:appsfeatures"),          QStringLiteral("打开：设置 - 应用") },
        { QStringLiteral("默认应用"),       QStringLiteral("🔗"), QStringLiteral("start ms-settings:defaultapps"),            QStringLiteral("打开：设置 - 默认应用") },
        { QStringLiteral("账户"),           QStringLiteral("👤"), QStringLiteral("start ms-settings:yourinfo"),               QStringLiteral("打开：设置 - 账户") },
        { QStringLiteral("登录选项"),       QStringLiteral("🗝️"), QStringLiteral("start ms-settings:signin"),                 QStringLiteral("打开：设置 - 登录选项") },
        { QStringLiteral("时间和语言"),     QStringLiteral("🕐"), QStringLiteral("start ms-settings:dateandtime"),           QStringLiteral("打开：设置 - 时间和语言") },
        { QStringLiteral("语言和区域"),     QStringLiteral("🌍"), QStringLiteral("start ms-settings:regionlanguage"),        QStringLiteral("打开：设置 - 语言和区域") },
        { QStringLiteral("游戏"),           QStringLiteral("🎮"), QStringLiteral("start ms-settings:gaming-home"),           QStringLiteral("打开：设置 - 游戏") },
        { QStringLiteral("辅助功能"),       QStringLiteral("♿"), QStringLiteral("start ms-settings:easeofaccess"),           QStringLiteral("打开：设置 - 辅助功能") },
        { QStringLiteral("隐私和安全性"),   QStringLiteral("🔐"), QStringLiteral("start ms-settings:privacy"),                 QStringLiteral("打开：设置 - 隐私和安全性") },
        { QStringLiteral("查找我的设备"),   QStringLiteral("🔍"), QStringLiteral("start ms-settings:findmydevice"),           QStringLiteral("打开：设置 - 查找我的设备") },
        { QStringLiteral("Windows 更新"),   QStringLiteral("🔄"), QStringLiteral("start ms-settings:windowsupdate"),         QStringLiteral("打开：设置 - Windows 更新") },
        { QStringLiteral("关于电脑"),       QStringLiteral("ℹ️"), QStringLiteral("start ms-settings:about"),                   QStringLiteral("打开：设置 - 关于") },
    };
    list.append(settings);

    // ============ 2. 管理工具（MMC 管理单元等系统管理窗口） ============
    MenuEntry admin;
    admin.title = QStringLiteral("管理工具");
    admin.emoji = QStringLiteral("🧰");
    admin.items = {
        { QStringLiteral("服务"),           QStringLiteral("⚙️"), QStringLiteral("start services.msc"),  QStringLiteral("打开：服务管理器") },
        { QStringLiteral("任务管理器"),     QStringLiteral("📊"), QStringLiteral("start taskmgr"),       QStringLiteral("打开：任务管理器") },
        { QStringLiteral("设备管理器"),     QStringLiteral("🔌"), QStringLiteral("start devmgmt.msc"),   QStringLiteral("打开：设备管理器") },
        { QStringLiteral("磁盘管理"),       QStringLiteral("💿"), QStringLiteral("start diskmgmt.msc"),   QStringLiteral("打开：磁盘管理") },
        { QStringLiteral("事件查看器"),     QStringLiteral("📜"), QStringLiteral("start eventvwr.msc"),   QStringLiteral("打开：事件查看器") },
        { QStringLiteral("注册表编辑器"),   QStringLiteral("🗂️"), QStringLiteral("start regedit"),        QStringLiteral("打开：注册表编辑器（会弹出 UAC 确认）") },
        { QStringLiteral("计算机管理"),     QStringLiteral("🖥️"), QStringLiteral("start compmgmt.msc"),   QStringLiteral("打开：计算机管理") },
        { QStringLiteral("本地用户和组"),   QStringLiteral("👥"), QStringLiteral("start lusrmgr.msc"),    QStringLiteral("打开：本地用户和组") },
        { QStringLiteral("组策略编辑器"),   QStringLiteral("📋"), QStringLiteral("start gpedit.msc"),     QStringLiteral("打开：本地组策略编辑器（专业版及以上）") },
        { QStringLiteral("系统配置"),       QStringLiteral("🧮"), QStringLiteral("start msconfig"),       QStringLiteral("打开：系统配置（msconfig）") },
        { QStringLiteral("系统信息"),       QStringLiteral("📄"), QStringLiteral("start msinfo32"),       QStringLiteral("打开：系统信息") },
        { QStringLiteral("性能监视器"),     QStringLiteral("📈"), QStringLiteral("start perfmon"),        QStringLiteral("打开：性能监视器") },
        { QStringLiteral("资源监视器"),     QStringLiteral("📉"), QStringLiteral("start resmon"),         QStringLiteral("打开：资源监视器") },
        { QStringLiteral("组件服务"),       QStringLiteral("🧩"), QStringLiteral("start comexp.msc"),      QStringLiteral("打开：组件服务") },
        { QStringLiteral("本地安全策略"),   QStringLiteral("🛡️"), QStringLiteral("start secpol.msc"),     QStringLiteral("打开：本地安全策略") },
        { QStringLiteral("防火墙（高级）"), QStringLiteral("🔥"), QStringLiteral("start wf.msc"),          QStringLiteral("打开：Windows 防火墙高级安全") },
        { QStringLiteral("证书管理器"),     QStringLiteral("📜"), QStringLiteral("start certmgr.msc"),   QStringLiteral("打开：当前用户证书管理器") },
        { QStringLiteral("ODBC 数据源"),    QStringLiteral("🗄️"), QStringLiteral("start odbcad32"),       QStringLiteral("打开：ODBC 数据源管理器") },
        { QStringLiteral("环境变量"),       QStringLiteral("🌱"), QStringLiteral("start rundll32 sysdm.cpl,EditEnvironmentVariables"), QStringLiteral("打开：环境变量编辑窗口") },
        { QStringLiteral("系统属性"),       QStringLiteral("💡"), QStringLiteral("start control sysdm.cpl"), QStringLiteral("打开：系统属性") },
        { QStringLiteral("Windows 功能"),   QStringLiteral("🧱"), QStringLiteral("start optionalfeatures"), QStringLiteral("打开：启用或关闭 Windows 功能") },
        { QStringLiteral("任务计划程序"),   QStringLiteral("⏰"), QStringLiteral("start taskschd.msc"),    QStringLiteral("打开：任务计划程序") },
    };
    list.append(admin);

    // ============ 3. 快捷操作（控制面板、小工具、电源动作等） ============
    MenuEntry quick;
    quick.title = QStringLiteral("快捷操作");
    quick.emoji = QStringLiteral("⚡");
    quick.items = {
        { QStringLiteral("控制面板"),       QStringLiteral("🎛️"), QStringLiteral("start control"),          QStringLiteral("打开：控制面板") },
        { QStringLiteral("命令提示符"),     QStringLiteral("⌨️"), QStringLiteral("start cmd"),              QStringLiteral("打开：命令提示符") },
        { QStringLiteral("PowerShell"),     QStringLiteral("🔷"), QStringLiteral("start powershell"),       QStringLiteral("打开：Windows PowerShell") },
        { QStringLiteral("文件资源管理器"), QStringLiteral("📁"), QStringLiteral("start explorer"),         QStringLiteral("打开：文件资源管理器") },
        { QStringLiteral("此电脑"),         QStringLiteral("💻"), QStringLiteral("start explorer shell:::{20D04FE0-3AEA-1069-A2D8-08002B30309D}"), QStringLiteral("打开：此电脑") },
        { QStringLiteral("回收站"),         QStringLiteral("🗑️"), QStringLiteral("start explorer shell:::{645FF040-5081-101B-9F08-00AA002F954E}"), QStringLiteral("打开：回收站") },
        { QStringLiteral("运行对话框"),     QStringLiteral("▶️"), QStringLiteral("start explorer shell:::{2559a1f3-21d7-11d4-bdaf-00c04f60b9f0}"), QStringLiteral("打开：运行（Win+R）对话框") },
        { QStringLiteral("计算器"),         QStringLiteral("🧮"), QStringLiteral("start calc"),              QStringLiteral("打开：计算器") },
        { QStringLiteral("记事本"),         QStringLiteral("📝"), QStringLiteral("start notepad"),           QStringLiteral("打开：记事本") },
        { QStringLiteral("截图工具"),       QStringLiteral("📷"), QStringLiteral("start snippingtool"),     QStringLiteral("打开：截图工具") },
        { QStringLiteral("字符映射表"),     QStringLiteral("🔤"), QStringLiteral("start charmap"),           QStringLiteral("打开：字符映射表") },
        { QStringLiteral("远程桌面"),       QStringLiteral("📡"), QStringLiteral("start mstsc"),             QStringLiteral("打开：远程桌面连接") },
        { QStringLiteral("磁盘清理"),       QStringLiteral("🧹"), QStringLiteral("start cleanmgr"),          QStringLiteral("打开：磁盘清理") },
        { QStringLiteral("程序和功能"),     QStringLiteral("📦"), QStringLiteral("start appwiz.cpl"),        QStringLiteral("打开：程序和功能（卸载程序）") },
        { QStringLiteral("声音属性"),       QStringLiteral("🎚️"), QStringLiteral("start control mmsys.cpl"), QStringLiteral("打开：声音属性") },
        { QStringLiteral("鼠标属性"),       QStringLiteral("🖱️"), QStringLiteral("start control main.cpl"),  QStringLiteral("打开：鼠标属性") },
        { QStringLiteral("日期和时间"),     QStringLiteral("🕰️"), QStringLiteral("start control timedate.cpl"), QStringLiteral("打开：日期和时间") },
        { QStringLiteral("电源选项"),       QStringLiteral("🔋"), QStringLiteral("start control powercfg.cpl"), QStringLiteral("打开：电源选项") },
        { QStringLiteral("锁定屏幕"),       QStringLiteral("🔒"), QStringLiteral("rundll32.exe user32.dll,LockWorkStation"), QStringLiteral("立即锁定当前会话") },
        { QStringLiteral("注销"),           QStringLiteral("🚪"), QStringLiteral("shutdown /l"),            QStringLiteral("注销当前用户"), true },
        { QStringLiteral("重启电脑"),       QStringLiteral("🔄"), QStringLiteral("shutdown /r /t 0"),        QStringLiteral("立即重启电脑"), true },
        { QStringLiteral("关闭电脑"),       QStringLiteral("⏹️"), QStringLiteral("shutdown /s /t 0"),        QStringLiteral("立即关闭电脑"), true },
        { QStringLiteral("取消关机"),       QStringLiteral("🚫"), QStringLiteral("shutdown /a"),             QStringLiteral("取消已计划的关机/重启") },
    };
    list.append(quick);

    return list;
}

} // namespace AppData
