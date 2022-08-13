#include "mainmenu.h"
#include "menu_control.h"
#include "ui_helper.h"
#include "app_init.h"
#include <QApplication>
#include <QSharedMemory>

int main(int argc, char *argv[])
{
    UI_helper::initMain();
    QApplication a(argc, argv);
    //创建运行时只允许存在一个程序
    static QSharedMemory *shareMem = new QSharedMemory("SingleAPP");
    //判断如果存在存直接退出
    if(!shareMem->create(1)){
        qApp->quit();
        return -1;
    }else{
        //安装事件过滤
        App_init::Instance()->start();
        //初始值-1进入程序循环
        int result = -1;
        //进入循环
        while (result !=0) {
            Menu_control_show::u_status = U_status::offline;  //设置用户离线状态，当重启时激活退出时有显示提问窗口

            Menu_control_show::login();  //启动登录界面
            UI_helper::setFont();  //设置应用字体为12
            UI_helper::setCode();  //设置字体为utf8
//            MainMenu m("jiji");
//            UI_helper::setFormInCenter(&m);
//            m.show();
            result= a.exec();  //!退出时如果为零，正常退出，否则重启程序
            continue;
        }
        return result;
    }
}
