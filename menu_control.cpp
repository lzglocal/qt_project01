#include "menu_control.h"


namespace Menu_control_show{
Menu_control::Menu_control(QObject *parent)
    : QObject{parent}{
}
Menu_control::~Menu_control(){
    if(u_login_net !=NULL){
        delete u_login_net;
    }

    if(mainmenu !=NULL){
        delete mainmenu;
    }

    if(u_login !=NULL){
        delete u_login;
    }
}
U_status u_status;
U_login_net* u_login_net=0;
U_login* u_login=0;
MainMenu* mainmenu=0;     //聊天窗口      //登录成功进入主页面
QString u_username;
//QHash<QString,U_group*> u_groups;
//QHash<QString,U_friend*> u_friends;
//QList<U_msg*> u_msgs;
//QStandardItemModel *u_group_list_model=new QStandardItemModel();
//QStandardItemModel *u_friend_list_model=new QStandardItemModel();
bool is_close=false;
QMutex mutex;

void login(){
    u_login_net = new U_login_net();
    u_login = new U_login();
    u_login->show();
}


void login_finished(bool ok,quint32 language_index){
    if(ok){
        //创建对象初始化用户名
        mainmenu = new MainMenu(u_username);
        //设置用户语言
        mainmenu->set_user_language(language_index);
        mainmenu->show();
        u_login->hide();
        u_login->close();
        u_login->deleteLater();
    }
}
}
