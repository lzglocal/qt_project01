#ifndef MULTITHREAD_H
#define MULTITHREAD_H

#include <QObject>
#include "core_common.h"

//多线程操作的动作
enum class ACTION{db_connect=1,query_sql};

class MultiThread : public QObject
{
    Q_OBJECT
public:
    //explicit MultiThread(quint32,QSqlDatabase,QObject *parent = nullptr);
    explicit MultiThread(QObject *parent = nullptr);
    ~MultiThread();

private:
    QMutex mutex;
    QSqlDatabase get_db; //设置一个db连接
    quint32 get_action;  //操作的动作
    quint32 get_purpose;  //设置什么请求发起的db连接
    //获取查询语句
    QString get_query_sql;


public:
    void set_db(QSqlDatabase);  //设置一个db连接接口
    void set_action(quint32);   //操作的动作
    void set_purpose(quint32);  //获取请求的数字，然后返回
    //需要执行的sql语句
    void set_query_sql(QString);


signals:
    //db连接结束信号
    void db_connect_finished(bool,QString,quint32);
    void query_sql_finished(QStringList);

public slots:
    //处理db连接槽
    void db_connect_handle();
};




#endif // MULTITHREAD_H
