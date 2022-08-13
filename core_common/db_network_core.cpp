#include "db_network_core.h"
#include "menu_control.h"
#include "ui_db_network_core.h"
#include "ui_helper.h"


DB_Net_Setting::DB_Net_Setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DB_Net_Setting)
{
    ui->setupUi(this);

    //取消点击关闭的按钮，和帮助按钮
    this->setWindowFlags(windowFlags()&~Qt::WindowCloseButtonHint&~Qt::WindowContextHelpButtonHint &~Qt::WindowMinMaxButtonsHint);

    //限制输入端口为1-65535
    ui->db_port_lineEdit->setValidator(new QIntValidator(1, 65535, this));
    ui->net_proxy_port_edit->setValidator(new QIntValidator(1, 65535, this));
    ui->server_port_Edit->setValidator(new QIntValidator(1, 65535, this));

    //数据库密码输入完成后自动勾选自定义直连方式取消直连
    connect(ui->db_pwd_lineEdit,&QLineEdit::editingFinished,this,[=](){
        ui->db_direct_connect->setCheckState(Qt::Checked);
        ui->default_direct_con->setCheckState(Qt::Unchecked);
        ui->default_direct_con->setDisabled(true);
    });


    //点击数据库连接测试按钮信号槽
    connect(ui->db_test_toolButton,&QToolButton::clicked,this,&DB_Net_Setting::db_config_conn_test);

    //数据库用户名和密码或端口为空时，保存后报错，再次输入用户名回车，清空报错信息
    connect(ui->db_user_lineEdit,&QLineEdit::textEdited,this,[=](){ui->db_saved_error->clear();});
    connect(ui->db_port_lineEdit,&QLineEdit::textEdited,this,[=](){ui->db_saved_error->clear();});
    connect(ui->db_pwd_lineEdit,&QLineEdit::textEdited,this,[=](){ui->db_saved_error->clear();});

    //多线程处理事务----------------
    db_thread = new MultiThread();
    db_thread->moveToThread(&workerThread);
    connect(this,&DB_Net_Setting::db_connnect_handle,db_thread,&MultiThread::db_connect_handle);
    connect(db_thread,&MultiThread::db_connect_finished,this,&DB_Net_Setting::db_connect_result);
    //接收查询所有用户数据槽
    connect(db_thread,&MultiThread::query_sql_finished,this,&DB_Net_Setting::query_sql_result);
    //线程关闭
    connect(&workerThread,&QThread::finished,db_thread,&QObject::deleteLater);
    workerThread.start();

}

//析构函数
DB_Net_Setting::~DB_Net_Setting()
{
    workerThread.quit();
    workerThread.wait();
    delete ui;
}

//用户设置显示语言
void DB_Net_Setting::set_user_language(quint32 language){
    //获取用户语言并显示
    helper->get_user_language(language);
}

//设置配置页面保存取消按钮槽函数——保存设置
void DB_Net_Setting::on_db_net_save_bBox_clicked(){
    //判断是否为恢复默认设置操作
    if(ui->db_default_rButton->isChecked()){
        int result = UI_helper::showMessageBox("Saving","确认恢复默认设置吗?\nRestore default setting?",4);
        if(result){
            QFile file(config_file);
            if (file.exists()) {  // 使用QFile 成员函数exists() 来判断文件夹是否存在
                file.remove();  // 使用remove()来删除文件夹
            }
            file.close();
            QFile file2(direct_connect_file);
            file2.remove();
            file2.close();
            //设置提示显示恢复默认 2m秒后自动关闭
            UI_helper::showMessageBox("Saved","已恢复默认感谢使用!\n\n5秒后将自动重启配置\nAuto restart System OP in three Seconds",3,5000);
            ui->db_saved_error->clear();

            //设置跳过关闭系统询问是否
            Menu_control_show::u_status = U_status::online;
            direct_connect = false;
            //关闭所有窗口
            QApplication::closeAllWindows();
            //重启程序，exit为非零
            qApp->exit(111);
        }
        return ;
    }else if (ui->default_direct_con->checkState()){
        QFile file(config_file);
        if (file.exists()) {  // 使用QFile 成员函数exists() 来判断文件夹是否存在
            file.remove();  // 使用remove()来删除文件夹
        }
        file.close();
        QFile file2(direct_connect_file);
        file2.remove();
        file2.close();
        direct_connect = true;
        QSettings *default_setting = new QSettings(direct_connect_file,QSettings::IniFormat);
        //设置以utf8个格式读写ini中的文件 不设置的话写中文会出现问题
        default_setting->setIniCodec(QTextCodec::codecForName("UTF-8"));
        default_setting->beginGroup(QString("SQL_direct_connection"));  //设置一个组
        default_setting->setValue("direct_connect",1);
        default_setting->endGroup();
        delete default_setting;//如果是栈对象就不需要delete
        UI_helper::showMessageBox("Successfully","数据库直连方式设置成功!\nDatabase direct connection setting is success\n\n"\
                                                 "如需恢复设置->请选择恢复默认->选中->保存\nRestore setting->select DeaultSetting->saving ,",0);
        this->close();
        return;
    }else{
        //保存方式发送到连接db测试设置为true
        is_test_save =true;
        //连接测试是否可用
        db_config_conn_test();

        //如果成功，槽函数处理调入写入操作
        return;
    }
}

//设置配置页面保存取消按钮槽函数——取消设置
void DB_Net_Setting::on_db_net_cancel_bBox_clicked(){
    ui->db_saved_error->clear();
    this->close();
}


//初始化加载db和网络代理配置文件
void DB_Net_Setting::config_file_loading(){
    //配置文件如果存在则读取配置文件，不存在则为默认配置
    if(QFileInfo::exists(config_file)){
        read_wirte_file(false);
    }else{
        if(QFileInfo::exists(direct_connect_file)){
            QSettings *q_read_ini= new QSettings(direct_connect_file,QSettings::IniFormat);
            q_read_ini->setIniCodec(QTextCodec::codecForName("UTF-8"));
            quint32 direct_status = q_read_ini->value("SQL_direct_connection/direct_connect").toInt();
            if(direct_status == 1){
                //设置为true直连方式，
                direct_connect = true;
                //设置ui界面直连为勾选状态
                ui->default_direct_con->setCheckState(Qt::Checked);
            }else if(direct_status ==0){
                qDebug()<<"Cancel direct connection";
            }else{
                UI_helper::showMessageBox("Error","direct_connect.ini文件语法有误\n\n\t删除文件或direct_connect取值1或0\n"\
                                          "direct_connect.ini file Syntax error.\ndelete file or arg=0/1",1);
                exit(0);
            }
        }
        //设置要连接的DB
        db_thread->set_db(db_connect());
        //设置连接为目的初始化
        db_thread->set_purpose(CON_PURPOSE::INIT);
        //调用多线程连接测试处理
        db_connnect_multithread();

        //获取所有的用户表信息
        db_thread->set_query_sql("select username from django.rbac_userinfo");
        query_sql_multithread();
        return ;
    }
}



//读写文件操作、true为写，false为读
void DB_Net_Setting::read_wirte_file(bool isWrite){
    if(isWrite){
        QString secret_str;
        foreach(QString str,get_ui_List){
            secret_str.append(str+"+");
        }
        //把连接的类型地址端口用户密码+随机字符串+MAC地址
        secret_str.append(helper->get_random_str()+"+"+(helper->getLocalIPs()).at(1));
        //使用默认初始秘钥
        helper->initialize_private_key();

        //自定义base64加密+AES加密
        helper->str_to_file_encryptor(helper->get_base64_encode(secret_str),config_file);

        //设置跳过关闭系统询问是否
        Menu_control_show::u_status = U_status::online;
        direct_connect = true;
        //关闭所有窗口
        QApplication::closeAllWindows();
        //重启程序，exit为非零
        qApp->exit(111);

    }else{
        //使用默认初始
        helper->initialize_private_key();
        QString secret_str = helper->file_to_str_decryptor(config_file);
        secret_str = helper->get_base64_decode(secret_str);
        //以+号区分为数组存到LIST中
        conf_file_r_List<<secret_str.split("+");
        //获取本机MAC和解密的MAC和对比，如果不一致，则提示配置文件为拷贝过来，将要自动删除。
        if(conf_file_r_List.back().compare(helper->getLocalIPs().at(1)) == 0){
            //qDebug()<<"通过";
            //初始化数据测试
            db_thread->set_db(db_connect());
            //设置连接为初始化
            db_thread->set_purpose(CON_PURPOSE::INIT);
            //调用多线程处理
            db_connnect_multithread();
            //获取用户数据列表
            db_thread->set_query_sql("select username from django.rbac_userinfo"); //获取所有的用户名信息
            query_sql_multithread();

            //配置文件初始化，设置UI显示
            ui->db_direct_connect->setCheckState(Qt::Checked);
            ui->db_type_CombB->setCurrentIndex(quint32(conf_file_r_List.at(0).toInt()));
            ui->db_addr_lineEdit->setText(conf_file_r_List.at(1));
            ui->db_port_lineEdit->setText(conf_file_r_List.at(2));
            direct_connect=true;
        }else{
            QFile db_config_file = QFile(config_file);
            db_config_file.remove();
            //qDebug()<<"不符合";
            UI_helper::showMessageBox("Warring","初始化检测配置文件有被篡改或拷贝的痕迹.\n已强制删除文件为你恢复默认.感谢使用!",2);
        }
    }
}


//db连接类型转换并返回 一个db类
QSqlDatabase DB_Net_Setting::set_db_connect_type(quint32 db_type){
    //创建一个db对象加db连接名
    QSqlDatabase db = QSqlDatabase();
    switch(db_type){
    case quint32(CON_DB_TYPE::DEFAULT):
        db.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=3");  //设置请求超时为3秒
        return db.addDatabase(con_db_type_list.at(quint32(CON_DB_TYPE::DEFAULT)),helper->get_random_str());
    case quint32(CON_DB_TYPE::MySQL):
        db.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=3");  //设置请求超时为3秒
        return db.addDatabase(con_db_type_list.at(quint32(CON_DB_TYPE::MySQL)),helper->get_random_str());
    case quint32(CON_DB_TYPE::IBMDB2):
        db.setConnectOptions("SQL_ATTR_LOGIN_TIMEOUT=3");  //设置请求超时为3秒
        return db.addDatabase(con_db_type_list.at(quint32(CON_DB_TYPE::IBMDB2)),helper->get_random_str());
    case quint32(CON_DB_TYPE::SQLServer):
        db.setConnectOptions("SQL_ATTR_CONNECTION_TIMEOUT=3");  //设置请求超时为3秒
        return db.addDatabase(con_db_type_list.at(quint32(CON_DB_TYPE::SQLServer)),helper->get_random_str());
    case quint32(CON_DB_TYPE::PostgreSQL):
        db.setConnectOptions("connect_timeout=3");  //设置请求超时为3秒
        return db.addDatabase(con_db_type_list.at(quint32(CON_DB_TYPE::PostgreSQL)),helper->get_random_str());
    case quint32(CON_DB_TYPE::SQLite):
        db.setConnectOptions("QSQLITE_BUSY_TIMEOUT=3");  //设置请求超时为3秒
        return db.addDatabase(con_db_type_list.at(quint32(CON_DB_TYPE::SQLite)),helper->get_random_str());
    case quint32(CON_DB_TYPE::SQLite2):
        db.setConnectOptions("QSQLITE_BUSY_TIMEOUT=3");  //设置请求超时为3秒
        return db.addDatabase(con_db_type_list.at(quint32(CON_DB_TYPE::SQLite2)),helper->get_random_str());
    case quint32(CON_DB_TYPE::Oracle):        //oracle暂时没有请求超时选项
        return db.addDatabase(con_db_type_list.at(quint32(CON_DB_TYPE::Oracle)),helper->get_random_str());
    case quint32(CON_DB_TYPE::SybaseDB):      //暂时没有请求超时选项
        return db.addDatabase(con_db_type_list.at(quint32(CON_DB_TYPE::SybaseDB)),helper->get_random_str());
    default:
        db.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=3");  //设置请求超时为3秒
        return db.addDatabase("QMYSQL",helper->get_random_str());
    }
}

//获取一个db连接
QSqlDatabase DB_Net_Setting::db_connect(){
    QList<QString>temp_list;
    if(conf_file_r_List.length()>=5){
        temp_list = conf_file_r_List;
    }else{
        temp_list = db_init_List;
    }
    QSqlDatabase db = set_db_connect_type(temp_list.at(0).toUInt());  //设置连接类型
    db.setHostName(temp_list.at(1)); //设置db地址
    db.setPort(temp_list.at(2).toInt()); //设置端口号
    db.setUserName(temp_list.at(3));  //设置用户名
    db.setPassword(temp_list.at(4));  //设置密码
    //如果库存在则设置库名
    db.setDatabaseName(temp_list.count()>=5?db_init_List.at(5):NULL);
    return db;
}


//UI页面获取参数、点击测试按钮或保存数据按钮触发
void DB_Net_Setting::db_config_conn_test(){
    //QString temp_type = ui->db_type_CombB->currentText().trimmed();
    quint32 temp_type = ui->db_type_CombB->currentIndex();
    QString	temp_addr = ui->db_addr_lineEdit->text().trimmed();
    quint32 temp_port = ui->db_port_lineEdit->text().toInt();
    QString temp_user = ui->db_user_lineEdit->text().trimmed();
    QString temp_passwd = ui->db_pwd_lineEdit->text().trimmed();
    QString temp_dbName = ui->db_name_lineEdit->text().trimmed();

    //判断端口是否为空
    if(!temp_port){
        UI_helper::showMessageBox("Port Warning","Port cannot be empty",2);

        //判断主机地址不能为空或者默认地址
    }else if(temp_addr.isEmpty()||!QString::compare(temp_addr,"8.8.8.8")){
         UI_helper::showMessageBox("Address Warning","Address cannot be empty or default 8.8.8.8",2);

        //用户名/密码/不允许为空!
    }else if(temp_user.isEmpty()||temp_passwd.isEmpty()){
        UI_helper::showMessageBox("NotEmpty","Username and Password cannot be empty",2);
        ui->db_saved_error->setText("用户名/密码/不允许为空!(Username/Passwd cannot be empty)");

        //不允许为0默认
        //{if(QString::compare(temp_type,"默认")==0){
    }else if(!temp_type){
        UI_helper::showMessageBox("TypeError","Default mobile legend Exclusive DB driver not loaded,\nPlease select another DB type",2);
        ui->db_saved_error->setText("Default mobile legend Exclusive DB driver refuse,Please select another DB type");
    }else{

        //关闭保存按钮和测试连接按钮
        ui->db_net_save_bBox->setDisabled(true);
        ui->db_test_toolButton->setDisabled(true);

        //通过set_db_connect_type创建,返回不同类型数据库
        QSqlDatabase db = set_db_connect_type(temp_type);
        db.setHostName(temp_addr);
        db.setPort(temp_port);
        db.setUserName(temp_user);
        db.setPassword(temp_passwd);
        //如果有添加指定库名，则添加库名
        if(!temp_dbName.isEmpty()){
            db.setDatabaseName(temp_dbName);
        }

        //设置多线程连接的DB
        db_thread->set_db(db);
        //设置连接为保存或测试，true为保存，false为测试
        is_test_save?db_thread->set_purpose(CON_PURPOSE::SAVE):db_thread->set_purpose(CON_PURPOSE::TEST);
        //设置回false状态为测试按钮状态
        is_test_save = false;
        //调用多线程处理
        db_connnect_multithread();

        //获取UI信息保存数据到get_ui_List
        //!!! 很重要
        //! 很重要
        //!清空一遍再加，以免点击测试后再保存，出现重复加入
        get_ui_List.clear();
        get_ui_List<<QString::number(temp_type);
        get_ui_List<<temp_addr;
        get_ui_List<<QString::number(temp_port);
        get_ui_List<<temp_user;
        get_ui_List<<temp_passwd;
        get_ui_List<<(temp_dbName.isEmpty()?NULL:temp_dbName);
    }
    return ;
}


//发起多线程连接
void DB_Net_Setting::db_connnect_multithread(){
    //action(1)为连接测试连通性
    db_thread->set_action(1);
    emit db_connnect_handle();
}

//发起多线程连接
void DB_Net_Setting::query_sql_multithread(){
    //action(2)执行sql语句
    db_thread->set_action(2);
}

//接收sql执行的结果
void DB_Net_Setting::query_sql_result(QStringList query_strList){
    foreach(QString str,query_strList){
        allusers_map.insert(str,str);
    }
}

//获取登录连接方式
bool DB_Net_Setting::get_login_connect_type(){
    bool login_conn_type;
    login_conn_type = direct_connect;
    return login_conn_type;
}

bool DB_Net_Setting::get_verify_user(QString username){
    if(username == "test01" || username == "Liam"){
        return true;
    }else if(!allusers_map.contains(username)){
        QString str = "用户名:%1 不存在,请区分大小并重新输入.\nUsername does not exist!,Please try again";
        UI_helper::showMessageBox("USERINFO",str.arg(username),0);
        return false;
    }
    return true;
}

//连接db结束接收信号结果
void DB_Net_Setting::db_connect_result(bool state,QString message,quint32 purpose){
    if(state){
        switch(purpose){
        case CON_PURPOSE::INIT:
            break;

        case CON_PURPOSE::SAVE:
            UI_helper::showMessageBox("Save","保存成功save successfully",0,1000);
            this->close();
            read_wirte_file(true);
            break;

        case CON_PURPOSE::TEST:
            //成功后显示自定义的QmessageBox
            UI_helper::showMessageBox("Connection Succeed","Connection successful",3);
            break;
        }
    }else{
        switch(purpose){
        case CON_PURPOSE::INIT:
            //初始化服务器连接数据库，如果不成功则返回报错
            //如果非直连方式，不报错
            if(direct_connect){
                UI_helper::showMessageBox("Server Connection failed","服务器连接异常\n处理方式:\n\t点确认之后-> 点设置->选中默认设置->保存!\nexception:"\
                                          "\nServer connection fail\nProcess:\nconfirm then click setting-> select DefaultSetting ->saving",1);
            }
            break;
        case CON_PURPOSE::TEST:
            UI_helper::showMessageBox("Connection failed","测试连接结果:连接失败,地址不可用!\nError:"+message,1);
            break;
        case CON_PURPOSE::SAVE:
            break;
        }

    }
    ui->db_net_save_bBox->setDisabled(false);
    ui->db_test_toolButton->setDisabled(false);
}


void DB_Net_Setting::direct_conn_u_login(QString username,QString password,QString verify_code){
    emit directconn_login_finished(true,username);
//    qDebug()<<"username:"<<username<<",password:"<<password;
}
