#ifndef U_LOGIN_H
#define U_LOGIN_H

#include "db_network_core.h"
#include "u_login_net.h"
#include "icon_helper.h"
#include "ui_helper.h"
#include "accountitem.h"

QT_BEGIN_NAMESPACE
namespace Ui { class U_login; }
QT_END_NAMESPACE

//extern QString u_username;
class U_login : public QDialog
{
    Q_OBJECT

public:
    U_login(QWidget *parent = nullptr);
    ~U_login();
    //语言切换选择
    Core_common common_core;
//    //设置窗口最大化最小化
//    Qt::WindowFlags windowFlag;


signals:
    //获取用户文件数据信号
    void read_filedata();


private slots:
    //注册功能槽函数
    void on_u_enroll_clicked();
    //忘记密码功能槽函数
    void on_u_forgot_passwd_clicked();
     //登录按钮槽函数
    void on_u_login_clicked();
    //语言切换槽函数
    void on_language_translation_currentIndexChanged(int index);
    //点击设置按钮
    void on_setting_db_clicked();
    //最小化
    void on_btn_login_min_clicked();
    //最大化
    void on_btn_login_max_clicked();
    //退出程序按钮
    void on_btn_login_close_clicked();


    //用户名输入完成验证接收信号处理
    void check_verify_user();
    //获取验证码字符串结束
    void get_verify_code_finished(bool get_code,QString code);
    //获取验证码图片后槽函数
    void get_verify_code_img_Finished(const QByteArray imgdata);
     //登录完成后槽函数
    void login_finished(bool login_ok,QString message);
    //关闭事件槽函数
    void closeEvent(QCloseEvent* event);

    //二维码
    void on_btn_qrcode_clicked();

    //显示下拉框用户槽
    void onShowAccountInfo(int,QString);
    //删除用户登录记录
    void onRemoveAccount(int,QString);

private:
    Ui::U_login *ui;
    //设置按钮DB和网络设置
    DB_Net_Setting *db_net_config = new DB_Net_Setting();

    //是否需要登录验证
    bool is_login_error = false;

    //用户登录数据的路径,保存到当前用户的家目录下，设置为隐藏文件
//    QString usersData_path = QDir().homePath()+"/.UsersData";
    QString usersData_path ="./UsersData";

    //用户默认头像路径
    QString default_users_ico = ":/images/login.ico";
    //初始化用户数据--读取用户名以及密码
    void user_data_init();

    // 下拉列表;
    QListWidget* u_accountlist;

    //记住密码保存到文件
    void saving_login_record(QString,QString password=NULL);

    //用户语言index
    quint32 language_index = 0;

    //处理account列表
    AccountItem *accounts;






protected:
    //事件过滤，捕捉鼠标双击事件
    bool eventFilter(QObject *watched, QEvent *event);
    //用户名和密码MAP容器，可以继承
    QMap<QString,QString>userdata_map;


};
#endif // U_LOGIN_H
