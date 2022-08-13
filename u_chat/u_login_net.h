#ifndef U_LOGIN_NET_H
#define U_LOGIN_NET_H

#include <QObject>
#include "core_common.h"



class U_login_net : public QObject
{
    Q_OBJECT
public:
    explicit U_login_net(QObject *parent = nullptr);

signals:
    void get_verify_code_finished(bool get_code,QString code);
    void get_verify_code_img_finished(const QByteArray imgdata);
    void login_finished(bool login_ok,QString message);
    void msg_received();
    void send_msg_finished(QString txuin,QString msg,bool is_ok);
    void sys_msg(QString msg);   //系统信息


public slots:
    void http_finished(QNetworkReply*);
    void http_pull_finished(QNetworkReply*);
    void http_ext_info_Finished(QNetworkReply*);
    void send_msg();      //发送信息槽函数


public:
    //获取验证码字符
    void get_verify_code(QString u_usernam);
    //获取验证码图片
    void get_verify_code_imags(QString u_usernam);
    //登录
    void login(QString u_num,QString u_pass,QString verify_code);
    //退出
    void logout();
    //
//    void get_friend_info(QString u_username);
//    void get_user_friends_list();
//    void get_group_name_list();
//    void send_msg(QString txuin,QString msg);
//    void get_group_member_info(QString groupTXUIN);
//    qint32 uin;

public:
    void get_friend_info(QString u_username);
    void get_user_friends_list();
    void get_groupname_list();
    void send_msg(QString txuin,QString msg);
    void get_group_member_info(QString groupTXUIN);
    qint32 uin;

private:
    Http_action http_action;   //http要执行的动作
    U_status u_status;       // 用户登录状态
    QTime time;
    QTimer *timer;
    QNetworkAccessManager * http;
    QString uinhexstr;
    QString get_password_mix(QString u_password,QString verify_code,QString uinhexstr);

    void pull_msg();
    void send_buddy_msg(QString uin,QString msg);
    void send_group_msg(QString group_uin,QString msg);
    void login_2();
    void check_siguare(QString urlstr);

    QString curr_group_tx_uin;
    QList<QPair<QString,QString>> send_msg_list;
    QPair<QString,QString> curr_send_msg;
    QNetworkAccessManager * http_pull;
    QNetworkAccessManager * http_extra_text_info;      //获得群内成员信息等
    QNetworkCookieJar * p_cookie_jar;
    QString get_cookie(const QString &name);
    QString get_hash(qint32 uin, QString u_username);
    qint32 msg_id;

private:
    //cookie 客户端信息保存
    QString web_inet_info;
    QString verify_web_api;
    QString psession_id;
    QString client_id;

    //处理json格式
    QJsonDocument json_doc;
    QJsonObject json_obj;
    QJsonArray json_array;
};

#endif // U_LOGIN_NET_H
