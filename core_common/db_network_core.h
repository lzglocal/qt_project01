#ifndef DB_NETWORK_CORE_H
#define DB_NETWORK_CORE_H
/* 自定义连接保存文件路径
 * 默认直连启动连接配置
 * 默认直连配置文件
 * DB连接方式
 * DB多线程
*/
#include <QDialog>
#include "multithread.h"
#include <QDir>

//数据库类型
enum class CON_DB_TYPE{DEFAULT,MySQL,IBMDB2,SQLServer,PostgreSQL,SQLite,SQLite2,Oracle,SybaseDB};
//根据连接db来区分打印信息
enum CON_PURPOSE{INIT=1,TEST,SAVE};

namespace Ui {
class DB_Net_Setting;
}

class DB_Net_Setting : public QDialog
{
    Q_OBJECT
    QThread workerThread;
public:
    explicit DB_Net_Setting(QWidget *parent = nullptr);
    ~DB_Net_Setting();
    //配置加载
    void config_file_loading();

    //获取登录直连方式:返回True为直连数据库(自定义或默认)，false为HttpAPI接口连接
    bool get_login_connect_type();

    //传入用户名匹配是否存在用户
    bool get_verify_user(QString);

    //创建对象指针获取MAC地址和加密解密
    Core_common  *helper = new Core_common();

    //获取一个db连接
    QSqlDatabase db_connect();

    //创建多线程对象指针
    MultiThread *db_thread ;

    //用户语言
    void set_user_language(quint32);


private:
    //登录时请求连接的方式，true为直连，false为HttpAPI连接
    bool direct_connect = false;


signals:
    //发起多线程连接db槽信号
    void db_connnect_handle();
    //登录结束
    void directconn_login_finished(bool ,QString);


private slots:
     //测试按钮槽
    void db_config_conn_test();
     //保存数据按钮槽
    void on_db_net_save_bBox_clicked();
    //取消按钮槽
    void on_db_net_cancel_bBox_clicked();
    //接收db处理状态槽函数
    void db_connect_result(bool,QString,quint32);
    //处理sql执行的结果
    void query_sql_result(QStringList);

private:
    Ui::DB_Net_Setting *ui;

//    QList<QWidget *> setting_widget;
    //db_net自定义配置直连文件路径
//    const QString config_file = QDir().homePath()+"/.dbconfig.dat";
    const QString config_file = "dbconfig.dat";
     //默认的直连配置文件路径
    const QString direct_connect_file = "direct_connect.ini";

    //LIST配置默认参数：连接类型 主机  端口  用户名 密码 库名
    QList <QString> db_init_List ={QString::number(quint32(CON_DB_TYPE::MySQL)),"192.168.130.128","3306","root","baidu.com123","",""};

    //从文件读取到的DB配置LIST
    QList <QString>conf_file_r_List;

    //从UI界面获取数据配置存储到LIST容器
    QList <QString> get_ui_List ;

    //处理UI界面数据配置，测试可用返回true，不可用返回FALSE
    bool get_ui_config();

     //db连接类型转换
    QSqlDatabase set_db_connect_type(quint32);

    //支持db连接类型
    QList<QString>con_db_type_list = {"SexySport Exclusive DB","QMYSQL","QDB2","QODBC","QPSQL","QSQLITE","QSQLITE2","QOCI","QTDS"};

    //读写文件函数/真为写，假为读
    void read_wirte_file(bool);

     //发送多线程db连接
    void db_connnect_multithread();

    //发起多线程执行sql语句
    void query_sql_multithread() ;

    //true 是点击保存发起db连接，false为test
    bool is_test_save = false;

    //DB所有用户
    QMap<QString,QString>allusers_map;

public:
    //直连登录参入参数用户名，密码，验证码
    void direct_conn_u_login(QString,QString,QString);

};

#endif // DB_NETWORK_CORE_H
