#include "u_login.h"
#include "ui_u_login.h"
#include "menu_control.h"



U_login::U_login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::U_login)
{
    ui->setupUi(this);
    ui->verifyCode->hide();
    ui->verifyImg->hide();
//    this->setWindowFlags(windowFlag); //正常有边框设置有最小化，最大化和，关闭按钮
//    this->setWindowFlags(Qt::FramelessWindowHint);  //设置无边框
    UI_helper::setFramelessForm(this);  //自定义设置无边框
    ui->login_title_widget->installEventFilter(this);  //捕捉鼠标双击动作

     //用户名输入栏QComboBox提示
    ui->u_username->lineEdit()->setPlaceholderText("username");
    //设置字体大小
    ui->u_username->lineEdit()->setFont(QFont("normal" ,14 , QFont::Light));


        // 密码框中的小键盘按钮;
    QPushButton *m_keyboardButton = new QPushButton();
    m_keyboardButton->setObjectName("pButtonKeyboard");
    m_keyboardButton->setFixedSize(QSize(32, 32));
    m_keyboardButton->setCursor(QCursor(Qt::PointingHandCursor));


//    Icon_helper::setIcon(ui->btn_login_min, 0xf068);   //最小化图标
//    Icon_helper::setIcon(ui->btn_login_max, 0xf067);   //最大化图标
//    Icon_helper::setIcon(ui->btn_login_close, 0xf00d);  //关闭按钮图标
    //最小化最大化关闭按钮图标
    ui->btn_login_min->setIcon(QIcon(":/images/app_minimize.png"));
    ui->btn_login_max->setIcon(QIcon(":/images/app_maximize.png"));
    ui->btn_login_close->setIcon(QIcon(":/images/app_exit.png"));


    //用户输入栏和密码输入栏放置图标
    ui->u_password->addAction(QIcon(":/images/diagram.png"),QLineEdit::TrailingPosition);
    //用户名和密码左边加入图标
    ui->u_password->addAction(QIcon(":/images/authorisation_password.png"),QLineEdit::LeadingPosition);
    ui->u_username->lineEdit()->addAction(QIcon(":/images/users.ico"),QLineEdit::LeadingPosition);

    //显示标题动画
    QMovie *movie = new QMovie(this);  //指针动画
    movie->setFileName(":/images/mobile_legend.gif");
    movie->start(); //播放
    ui->title_gif_label->setMovie(movie);    


    //设置用户名和密码正则规则只能字母和数字或密码的特殊字符
    const QRegExpValidator *p_revalidator_user = new QRegExpValidator(QRegExp("[a-zA-Z0-9]+$"),this);
    ui->u_username->setValidator(p_revalidator_user);
    const QRegExpValidator *p_revalidator_passwd = new QRegExpValidator(QRegExp("[a-zA-Z0-9.!@#$%^&*+_-]+$"),this);
    ui->u_password->setValidator(p_revalidator_passwd);


    //输入完用户名后触发信号槽
    connect(ui->u_username->lineEdit(),&QLineEdit::editingFinished,this,&U_login::check_verify_user);
//    connect(ui->u_username->lineEdit(),&QLineEdit::editingFinished,this,&U_login::check_verify_user);
    connect(Menu_control_show::u_login_net,&U_login_net::get_verify_code_finished,this,&U_login::get_verify_code_finished);
    connect(Menu_control_show::u_login_net,&U_login_net::get_verify_code_img_finished,this,&U_login::get_verify_code_img_Finished);
    connect(Menu_control_show::u_login_net,&U_login_net::login_finished,this,&U_login::login_finished);


//    DB初始化测试连通性和DB配置/直连信号登录槽槽函数
    db_net_config->config_file_loading();
    connect(db_net_config,&DB_Net_Setting::directconn_login_finished,this,&U_login::login_finished);


    //初始化用户数据
    U_login::user_data_init();

    //打印是否支持ssl
//    qDebug()<<"QSslSocket="<<QSslSocket::sslLibraryBuildVersionString();
//    qDebug() << "OpenSSL支持情况:" << QSslSocket::supportsSsl();

}


U_login::~U_login()
{
    db_net_config->deleteLater();
    u_accountlist->deleteLater();
    delete ui;
}


//注册功能槽函数
void U_login::on_u_enroll_clicked()
{
    QString enroll_notice = "很抱歉暂时没有开放注册功能！请联系管理员\nSorry, this function is not open for the time being.\n\nPlease contact your administrator for assistance";
    int res = UI_helper::showMessageBox("注册Sign up",enroll_notice,4);
    if(res){
        return ;
    }else{
        QDesktopServices::openUrl(QUrl(QString("https://t.me/SGI_Liam")));
    }
}


//忘记密码功能槽函数
void U_login::on_u_forgot_passwd_clicked()
{
    QDialog forgot(this);
    forgot.setWindowTitle("忘记密码ForgotPW");
    forgot.resize(400,500);
    Qt::WindowFlags windowFlag  = Qt::Dialog;
    windowFlag  |= Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint|Qt::WindowCloseButtonHint;
    forgot.setWindowFlags(windowFlag);
    QLabel label("我是标签",&forgot);
    QPushButton button("确认修改",&forgot);
    label.move(30,50);
    button.move(220,280);
    forgot.exec();
}

//登录按钮槽函数
void U_login::on_u_login_clicked()
{
    //QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    if(ui->u_username->currentText().trimmed().isEmpty()){
        UI_helper::showMessageBox("warning","用户名不能为空!\nIncorrect username or Password",2);
        return;
    }else if(ui->u_password->text().trimmed().isEmpty()){
        UI_helper::showMessageBox("warning","Incorrect username or Password",2);
        return;
    }

    if(is_login_error){
        if(ui->verifyCode->text().trimmed().isEmpty()){
            UI_helper::showMessageBox("warning","Note: that both fields may be case-sensitive",2);
            return;
        }
    }

    //判断连接的方式:true为数据库直连，false为 API接口连接
    if( db_net_config->get_login_connect_type()){
        db_net_config->direct_conn_u_login(ui->u_username->currentText().trimmed(),ui->u_password->text().trimmed(),ui->verifyCode->text().trimmed());
    }else{
        Menu_control_show::u_login_net->login(ui->u_username->currentText().trimmed(),ui->u_password->text().trimmed(),ui->verifyCode->text().trimmed());
    }

}

//语言切换换槽函数
void U_login::on_language_translation_currentIndexChanged(int index)
{
    common_core.get_user_language(index);
    ui->language_translation->blockSignals(true);
    ui->retranslateUi(this);
    ui->language_translation->setCurrentIndex(index);
    ui->language_translation->blockSignals(false);
    //设置主页的语言相同
    language_index=index;
}


//获取用户名验证码字符
void U_login::check_verify_user(){
    //    qDebug()<<"u_login-checkVerifycode"<<endl;
    if(ui->u_username->currentText().trimmed().isEmpty()){
        UI_helper::showMessageBox("Error","用户名不能为空\nIncorrect username or Empty",1);
        return;
    }else{
        //用户MAP容器验证用户是否存在
        if(db_net_config->get_verify_user(ui->u_username->currentText().trimmed())){
            //发起get_verify_code请求验证用户和获取用户是否存在
            Menu_control_show::u_login_net->get_verify_code(ui->u_username->currentText().trimmed());
        }else{
            ui->u_username->setCurrentText(""); //用户不存在清空输入栏
            ui->u_username->setFocus();  //把焦点重新放回输入栏
        }
    }
}


void U_login::get_verify_code_finished(bool get_code,QString code){
    if(get_code){
        //第一次在获取用户不需要验证码
        ui->verifyCode->setText(code);
        ui->verifyImg->setText("不需要验证码");
        ui->verifyCode->hide();
        ui->verifyImg->hide();
    }else{
        //清理验证码输入框，获取验证码
        ui->verifyCode->clear();
        Menu_control_show::u_login_net->get_verify_code_imags(ui->u_username->currentText().trimmed());
    }
}

void U_login::get_verify_code_img_Finished(const QByteArray imgdata){
//    qDebug()<<imgdata.length()<<endl;
    QPixmap p;
    p.loadFromData(imgdata);
    ui->verifyImg->setPixmap(p);
    ui->verifyCode->show();
    ui->verifyImg->show();
    is_login_error = true;
}

void U_login::login_finished(bool login_ok,QString message){
    if(login_ok){
        //如果勾选了记住密码则记住账户和密码
        saving_login_record(ui->u_username->currentText().trimmed(),ui->u_password->text().trimmed());
        Menu_control_show::u_username=ui->u_username->currentText().trimmed();
        Menu_control_show::u_status = U_status::online;
        Menu_control_show::login_finished(login_ok,quint32(language_index));
    }else{
        UI_helper::showMessageBox("Error","Logon failure:\nunknown user name or bad password",1,3000);
        is_login_error = true;
        check_verify_user();
    }
}

//发送获取初始化用户数据
void U_login::user_data_init(){
    QDir user_data_path = QDir(usersData_path);
    if(user_data_path.exists()){
        //Do not list the special entries "." and ".."
        QStringList alluser_path_List = user_data_path.entryList(QDir::Dirs|QDir::NoDotAndDotDot,QDir::Time);
        //如果列表容器不为空
        if(!alluser_path_List.isEmpty()){
            //循环加载每一个用户
            foreach(QString username,alluser_path_List){
                QDir user_dir = usersData_path+"/"+username;

                QStringList user_files_List = user_dir.entryList(QDir::Files|QDir::NoDotAndDotDot,QDir::Time);
                if(!user_files_List.isEmpty()){
                    if(user_files_List.contains(username+".crt",Qt::CaseInsensitive)){
                        //AES对称加密初始化秘钥,使用默认秘钥
                        common_core.initialize_private_key();

                        //AES读取加密文件返回字符串
                        QString str_user_pd_base64 = common_core.file_to_str_decryptor(usersData_path+"/"+username+"/"+username+".crt");
                        //base64解密为源加密串
                        QString source_str = common_core.get_base64_decode(str_user_pd_base64);

                        //以>>号区分为数组存到LIST中
                        QStringList userLogin_record_List = source_str.split(">>");

//                        foreach(QString str,userLogin_record_List){
//                            qDebug()<<str;
//                        }
                        //获取本机MAC和解密的MAC和对比，如果不一致，则提示配置文件为拷贝过来，将要自动删除。
                        if(userLogin_record_List.back().compare(common_core.getLocalIPs().at(1)) == 0){
                            //用户名+密码保存到map容器中
                            userdata_map.insert(userLogin_record_List.at(0),userLogin_record_List.at(1));
                        }else{
                            QDir user_dir_delete = QDir(usersData_path+"/"+username);
                            user_dir_delete.removeRecursively();
                            QString user_message = "%1文件有被篡改或拷贝的痕迹,已强制删除\n%2 was tampered with and deleted";
                            UI_helper::showMessageBox("warnning",user_message.arg(username+".crt",username+".crt"),2,2000);
                        }
                    }
                }else{
                    //只保存用户名
                    userdata_map.insert(username,"");

                }
            }


            //初始化UI界面  --遍历MAP容器keys
            u_accountlist = new  QListWidget(this);
            //设置下拉框的宽度和高度
            u_accountlist->setStyleSheet("QListWidget::item{height:35px;}");
            ui->u_username->setModel(u_accountlist->model());
            ui->u_username->setView(u_accountlist);
            //循环把用户名显示出来
            int i =0;
            foreach(QString username,userdata_map.keys()){
                AccountItem *account_item = new AccountItem();
                //用户头像路径
                QString user_ico_path = QString(usersData_path+"/"+username+"/"+username+".png");
                QFile user_ico = QFile(user_ico_path);

                //判断用户头像是否存在
                if(user_ico.exists()){
                   account_item->setAccountInfo(i, username, QString(user_ico_path));
                }else{
                   //用户头像不存在则使用默认头像
                   account_item->setAccountInfo(i, username, QString(default_users_ico));
                }

                //显示和删除用户登录记录槽函数
                connect(account_item, &AccountItem::signalShowAccountInfo, this, &U_login::onShowAccountInfo);
                connect(account_item, &AccountItem::signalRemoveAccount, this, &U_login::onRemoveAccount);

                QListWidgetItem *list_item = new QListWidgetItem(u_accountlist);
                u_accountlist->setItemWidget(list_item, account_item);
                ++i;
            }
        }
    }
}


//保存登录记录和保存密码
void U_login::saving_login_record(QString username,QString password){
    QDir users_dir = QDir();
    QString user_path = usersData_path+"/%1";
    //创建用户数据目录+用户一个家目录+一个数据文件
    if(users_dir.mkpath(user_path.arg(username))){
        if(ui->checkBox_pw->isChecked()){
            //自定义base64加密 ,加密方式：---用户名 +密码 +当前时间+ 随机字符串+ MAC地址--后期需要加时间戳
            QString str = username+">>"+password+">>"+common_core.get_current_time()+">>"+common_core.get_random_str()+">>"+(common_core.getLocalIPs()).at(1);
            QString base64_en = common_core.get_base64_encode(str);
            //qDebug()<<base64_en;

            //AES对称加密初始化秘钥,使用默认秘钥
            common_core.initialize_private_key();
            //加密写入文件中
            common_core.str_to_file_encryptor(base64_en,QString(user_path+"/%1.crt").arg(username));
        }
    }else{
        qDebug()<<"create userpath failed";
    }
}

//删除用户登录记录槽函数
void U_login::onRemoveAccount(int index,QString username){
//    QString username = ui->u_username->itemText(index);
    ui->u_username->removeItem(index);
    QDir user_recode_delete = QDir(usersData_path+"/"+username);
    user_recode_delete.removeRecursively();
}



//关闭窗口事件: 1.初始化页面状态 user_status是offline= 0有询问。2.登录后页面跳转关闭窗口时登录状态为offline=1默默关闭。
void U_login::closeEvent(QCloseEvent* event){
    if(!(int(Menu_control_show::u_status))){
        int result = UI_helper::showMessageBox("Sign Out","Logging Out of Sexy OP?",4);
        //如果true，则选择了yes正常关闭
        if(result){
            event->accept();
        }else{    //否则忽略
            event->ignore();
        }
    }
}

//点击最小化隐藏
void U_login::on_btn_login_min_clicked()
{
   this->showMinimized();
}

//点击最大化
void U_login::on_btn_login_max_clicked()
{
    static bool max = false;
    static QRect location = this->geometry();

    if (max) {
        this->setGeometry(location);
        ui->btn_login_max->setIcon(QIcon(":/images/app_maximize.png"));
    } else {
        location = this->geometry();
        this->setGeometry(UI_helper::getScreenRect());
        ui->btn_login_max->setIcon(QIcon(":/images/app_maxrestore.png")); //最大化时设置恢复图标
    }
    this->setProperty("canMove", max);
    max = !max;
}

//点击关闭按钮事件
void U_login::on_btn_login_close_clicked()
{
    this->close();
}


//数据库与网络设置页面显示
void U_login::on_setting_db_clicked()
{
    db_net_config->set_user_language(language_index);
    db_net_config->exec();
}


//双击最大屏幕事件过滤
bool U_login::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->login_title_widget) {
        if (event->type() == QEvent::MouseButtonDblClick) {
            on_btn_login_max_clicked();
        }
    }
    return QWidget::eventFilter(watched, event);
}



//点击显示二维码
void U_login::on_btn_qrcode_clicked()
{
    //隐藏登录页面
    this->hide();
    //创建新的二维码窗口
    QDialog qr_code = QDialog();
//    qr_code.setWindowIcon(QIcon(":/images/logo.ico"));
//    qr_code.setWindowIconText("QRcode");
//    qr_code.setWindowFlags(Qt::FramelessWindowHint);  //设置无边框
    UI_helper::setFramelessForm(&qr_code);  //自定义设置无边框

    //设置窗口大小
    qr_code.setFixedSize(300,300);
    //label 图片
    QLabel qrcode_label = QLabel(&qr_code);
    qrcode_label.setPixmap(QPixmap(":/images/ui_qrcode.jpg"));
    qrcode_label.move(9,9);

    //pushbutton按钮
    QPushButton btn_qrcode = QPushButton("返 回",&qr_code);
    btn_qrcode.move(9,270);
    btn_qrcode.setMinimumSize(55,30);
    btn_qrcode.setMaximumSize(55,30);

    //点击返回按钮信号槽
    connect(&btn_qrcode,&QPushButton::clicked,&qr_code,[&, this](){
        qr_code.close();
        this->show();
        qr_code.deleteLater();
    });

    //窗口循环
    qr_code.exec();
}


//将选项文本显示在QComboBox当中
void U_login::onShowAccountInfo(int index, QString account_name){
    ui->u_username->setEditText(account_name);
    ui->u_username->hidePopup();

    //自动填充密码
    ui->u_password->setText(userdata_map.find(account_name).value());

    // 更换用户头像;
//    QString fileName = QString(":/Resources/LoginWindow/headImage/head_%1.png").arg(index);
//    ui->userHead->setPixmap(QPixmap(fileName).scaled(ui->userHead->width(), ui->userHead->height()));
}
