#include "multithread.h"

//MultiThread::MultiThread(quint32 action,QSqlDatabase db,QObject *parent)
MultiThread::MultiThread(QObject *parent)
    :QObject{parent}

{
//    get_action = action;
//    get_db = db;


}

MultiThread::~MultiThread(){

}

//设置db连接
void MultiThread::set_db(QSqlDatabase db){
        get_db = db;
}
//获取一个操作
void MultiThread::set_action(quint32 action){
     get_action = action;
}

//获取请求连接的id
void MultiThread::set_purpose(quint32 purpose_num){
    get_purpose = purpose_num;
}

//获取执行的sql语句
void MultiThread::set_query_sql(QString sql){
    get_query_sql = sql;
}


//处理数据库连接
void MultiThread::db_connect_handle(){    
//    QMutexLocker locker(&mutex);
    switch(get_action){
    //测试连接连通性
    case quint32(ACTION::db_connect):{
        if(get_db.open()){
            //成功返回true
             emit db_connect_finished(true,"Connection successful",get_purpose);
        }else{
            emit db_connect_finished(false,get_db.lastError().text(),get_purpose);
        }
        get_db.close();
        break;
    }
        //执行sql
    case quint32(ACTION::query_sql):
    {
        if(get_db.open()){
            //成功返回true
            //查询操作
             QSqlQuery  query(get_db.exec(get_query_sql));
             QStringList query_strList;
             while (query.next()) {
                 query_strList<<query.value(0).toStringList();
             }
             emit query_sql_finished(query_strList);
             get_db.close();
        }else{
            emit db_connect_finished(false,get_db.lastError().text(),get_purpose);
        }
    }
    default:
        break;
    }
    return;
}

