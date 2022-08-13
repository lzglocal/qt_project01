#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>
#include "core_common.h"
#include "ui_helper.h"
#include <QSystemTrayIcon>
#include <QMediaPlayer>
#include "gifwidget.h"
#include "cJSON.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainMenu; }
QT_END_NAMESPACE

class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    //构造函数初始化用户名
    MainMenu(QString user_name,QWidget *parent = nullptr);
    ~MainMenu();

private:
    //初始化设置ico图标和托盘
    void init_ico_tray();
    //初始化顶部按钮点击显示对应页面
    void init_title_tbtn();
    //各功能项添加按钮和初始化信号槽
    void add_func_tbtn();



private:
    Ui::MainMenu *ui;

    //用户名
    const QString username;
    //用户使用的语言
    QString user_language = "zh_CN";
    //用户登录数据的路径,保存到当前用户的家目录下，设置为隐藏文件
    //QString usersData_path = QDir().homePath()+"/.UsersData";
    QString usersData_path ="./UsersData";

    Core_common *common_core = new Core_common();
    UI_helper *ui_helper = new UI_helper();
    //托盘图标托盘对象
    QSystemTrayIcon *tray;
    //托盘菜单
    QMenu *menu;
    //菜单实现功能：还原窗口
    QAction *recov_vision;
    //退出系统按钮
    QAction *exit_system;
    //office工具栏的功能项名称和ico
    QList<QString>tools_func_name = {"截图","录制GIF","获取IP地址","AES文件加密","AES文件解密","Base加解密","格式系列化"};
    QStringList tools_func_ico = {"screenshot.png","newgif.png","ipaddress.png","security_aes.png","security_aes_1.png","lock.png","json_filetype.png"};
    //



public:
    //设置当前用户选择的语言
    void set_user_language(quint32);
    //设置登录用户名
//    static void set_user_name(QString);

private slots:
    //最小化
    void on_ptn_main_min_clicked();
    //最大化
    void on_ptn_main_max_clicked();
    //退出
    void on_ptn_main_exit_clicked();
    //托盘图标恢复窗口槽函数
    void TrayIconAction(QSystemTrayIcon::ActivationReason reason);
    //还原系统到窗口
    void recover_vision();

    //标题栏按钮点击触发槽
    void title_tbtn_cliecked();

    //功能项下所有的toolbutton处理
    void vector_all_tbtn_handle();

    //上传头像
    void on_profile_photo_pbtn_clicked();

    //离开锁屏
    void on_lock_sys_pbtn_clicked();

    //用户退出系统
    void on_exit_sys_pbtn_clicked();

    //主页tab1-tab3切换
    void on_main_tabWidget_currentChanged(int index);

    //json格式化按钮
    void on_json_format_pbtn_clicked();

    //转为html
    void on_office_to_html_pbtn_clicked();

    //转为markdown格式
    void on_office_to_mkd_pbtn_clicked();

    //清空内容
    void on_office_clear_pbtn_clicked();

    //转为字节流
    void on_office_to_bytes_pbtn_clicked();

protected:
    //重写关闭事件
    void closeEvent(QCloseEvent *event);



};
#endif // MAINMENU_H
