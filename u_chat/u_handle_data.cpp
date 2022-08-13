#include "u_handle_data.h"
#include "qdebug.h"

U_handle_data::U_handle_data(QObject *parent)
    : QObject{parent}
{

}

U_handle_data::~U_handle_data(){

}

//写入登录信息数据：user+密码+头像路径
void U_handle_data::write_filedata_handle(){

}

//读取本地用户数据
void U_handle_data::get_filedata_handle(){
    emit read_logindata_finished("ok","ok","ok");
}
