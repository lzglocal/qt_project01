#ifndef MENU_CONTROL_H
#define MENU_CONTROL_H

#include "core_common.h"
#include "mainmenu.h"
#include "u_login.h"
#include "u_login_net.h"
#include "db_network_core.h"
//#include "u_chat_main.h"
//#include "u_friend.h"
//#include "u_group.h"
//#include "u_msg.h"

class U_login;
class MainMenu;
//class U_login_net;
namespace Menu_control_show {
class Menu_control : public QObject
{
    Q_OBJECT
public:
    explicit Menu_control(QObject *parent = nullptr);
    ~Menu_control();

signals:
};

extern U_status u_status;
extern U_login_net* u_login_net;
extern U_login* u_login;
extern MainMenu* mainmenu;
extern void login();
extern void login_finished(bool ok,quint32 language_index);
extern QString u_username;
//extern QHash<QString,U_group*> u_groups;
//extern QHash<QString,U_friend*> u_friends;
//extern QList<U_msg*> u_msgs;
//extern QStandardItemModel *u_group_list_model;
//extern QStandardItemModel *u_friend_list_model;
extern bool is_close;
extern QMutex mutex;
}
#endif // MENU_CONTROL_H
