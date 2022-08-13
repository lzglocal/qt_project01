#ifndef U_HANDLE_DATA_H
#define U_HANDLE_DATA_H

#include <QObject>

class U_handle_data : public QObject
{
    Q_OBJECT
public:
    explicit U_handle_data(QObject *parent = nullptr);
    ~U_handle_data();



signals:
    //写入登录信息数据信号
//    void write_logindata();
    //读取登录信息数据信号
    void read_logindata_finished(QString,QString,QString);

public slots:
    //读取本地用户数据
    void get_filedata_handle();



private:
     //用户账号+ 用户密码 +头像保存地址
    QStringList users_info_list;
    //用户目录列表
    QStringList usersdir_list(const QString &strDirpath);


//    void write_users_logindata();
//    //读取本地用户数据
//    void read_users_logindata();

private slots:
    //写入登录信息数据：user+密码+头像路径
    void write_filedata_handle();



};

#endif // U_HANDLE_DATA_H
