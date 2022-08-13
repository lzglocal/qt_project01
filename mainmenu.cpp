#include "mainmenu.h"
#include "ui_mainmenu.h"
#include <QDesktopServices>


MainMenu::MainMenu(QString user_name,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainMenu)
    , username(user_name)   //初始化用户名
{
    ui->setupUi(this); 
    //设置无边框
    ui_helper->setFramelessForm(this);

    //设置无边框可鼠标拖动放大
    setMouseTracking(true);

    //初始化ico图标和托盘
    init_ico_tray();

    //初始化顶部按钮/用户名/用户头像/点击显示对应页面
    init_title_tbtn();
    ui->title_home_tbtn->click();  //初始化页面为home主页

    //初始化功能项按钮
    add_func_tbtn();

    //托盘信号槽
    connect(tray,&QSystemTrayIcon::activated,this,&MainMenu::TrayIconAction);
    connect(recov_vision,&QAction::triggered,this,&MainMenu::recover_vision);
    connect(exit_system,&QAction::triggered,qApp,&QApplication::quit);

}


//析构函数
MainMenu::~MainMenu()
{
    common_core->deleteLater();
    tray->deleteLater();
    recov_vision->deleteLater();
    exit_system->deleteLater();
    menu->deleteLater();
    delete ui_helper;
    delete ui;
}

//设置语言
void MainMenu::set_user_language(quint32 language_index){
    common_core->get_user_language(language_index);
    switch(language_index){
    case 0:
        user_language = "zh_CN";
        break;
    case 1:
        user_language = "en";
        break;
    case 2:
        user_language = "fil";
    default :
        user_language = "en";
    }
}



//----------------------初始化ico图标和实例化托盘---------------------------------------
void MainMenu::init_ico_tray(){
    //标头菜单栏ico
    ui->title_home_tbtn->setIcon(QIcon(":/images/home_page.png"));
    ui->title_office_tbtn->setIcon(QIcon(":/images/office.png"));
//    ui->automatic_tButton->setIcon(QIcon(":/images/automation.png"));
    ui->title_ops_tbtn->setIcon(QIcon(":/images/ops.png"));
    ui->title_vpn_tbtn->setIcon(QIcon(":/images/shadowsocks.png"));
    ui->title_instantly_tbtn->setIcon(QIcon(":/images/communication.png"));
    ui->title_settting_tbtn->setIcon(QIcon(":/images/setting.png"));
    //用户ico
    ui->title_users_tbtn->setIcon(QIcon(":/images/08.png"));


    //最小化最大化退出ico
    ui->ptn_main_min->setIcon(QIcon(":/images/app_minimize.png"));
    ui->ptn_main_max->setIcon(QIcon(":/images/app_maximize.png"));
    ui->ptn_main_exit->setIcon(QIcon(":/images/app_exit.png"));

    //实例化托盘
    tray = new QSystemTrayIcon(this);
    //设置图标
    tray->setIcon(QIcon(":/images/logo.ico"));
    tray->show();

    //右键菜单托盘菜单栏-还原显示
    menu = new QMenu(this);
    recov_vision = new QAction(this);
    recov_vision->setText("show");
    //设置图标
    recov_vision->setIcon(QIcon(":/images/restore.png"));

    //托盘菜单栏-退出显示
    exit_system = new QAction(this);
    //设置图标
    exit_system->setIcon(QIcon(":/images/exit.png"));
    exit_system->setText("exit");

    //VPN托盘设置


    //设置托盘菜单
    tray->setContextMenu(menu);
    menu->addAction(recov_vision);
    menu->addAction(exit_system);
}



//---------------------------初始化顶部标题栏按钮点击和点击槽显示对应页槽以及用户名和头像处理-------------------------
void MainMenu::init_title_tbtn(){

    //设置顶部导航按钮
    //QList<QToolButton *> title_tbtn = ui->title_widget_all->findChildren<QToolButton *>();
    QList<QToolButton *> title_tbtn = ui->title_widget_all->findChildren<QToolButton *>();
    foreach (QToolButton *btn, title_tbtn) {
//        btn->setMinimumWidth(icoWidth);
        btn->setCheckable(true);
//        qDebug()<<btn->objectName();
        connect(btn, &QToolButton::clicked, this, &MainMenu::title_tbtn_cliecked);
    }

    //设置用户设置 用户名和头像设置
    ui->title_users_tbtn->setText(username);
    QString user_pic_path = usersData_path+"/"+username+"/"+username+"%1";
    QFile png_path = QFile(user_pic_path.arg(".png"));
    QFile jpg_path = QFile(user_pic_path.arg(".jpg"));
    QFile ico_path = QFile(user_pic_path.arg(".ico"));
    if(png_path.exists()){
        ui->title_users_tbtn->setIcon(QIcon(user_pic_path.arg(".png")));
    }else if(jpg_path.exists()){
        ui->title_users_tbtn->setIcon(QIcon(user_pic_path.arg(".jpg")));
    }else if(ico_path.exists()){
        ui->title_users_tbtn->setIcon(QIcon(user_pic_path.arg(".ico")));
    }

    //main页面浏览器地图
    ui->main_map_axWidget->setFocusPolicy(Qt::StrongFocus);//设置控件接收键盘焦点的方式：鼠标单击、Tab键;
    ui->main_map_axWidget->setProperty("DisplayAlerts",false); //不显示任何警告信息。
    ui->main_map_axWidget->setProperty("DisplayScrollBars",true); // 显示滚动条
    ui->main_map_axWidget->show();
    QString url = QString("https://www.google.com/maps/@14.4288604,121.0220416,15z?hl=%1");
    //跟随用户语言设置地图的语言
    ui->main_map_axWidget->dynamicCall("Navigate(const QString&)",url.arg(user_language));

    //main页面代办事务
    ui->main_event_axWidget->setFocusPolicy(Qt::StrongFocus);//设置控件接收键盘焦点的方式：鼠标单击、Tab键;
    ui->main_event_axWidget->setProperty("DisplayAlerts",false); //不显示任何警告信息。
    ui->main_event_axWidget->setProperty("DisplayScrollBars",true); // 显示滚动条
    ui->main_event_axWidget->show();
    QString url2 = QString("https://www.google.com");
    ui->main_event_axWidget->dynamicCall("Navigate(const QString&)",url2);

}


//标题栏按钮点击触发槽
void MainMenu::title_tbtn_cliecked(){
    //获取发送者
    QToolButton *tbtn_name = (QToolButton *)sender();
    QString name = tbtn_name->objectName();

    //寻找对应的QToolbutton按钮
    QList<QToolButton *> title_tbtn = ui->title_widget_all->findChildren<QToolButton *>();
    foreach (QToolButton *btn, title_tbtn) {
        btn->setChecked(btn == tbtn_name);
    }

    if (name == "title_home_tbtn") {
        ui->stackedWidget->setCurrentIndex(0);
    } else if (name == "title_office_tbtn") {
        //json格式化窗口隐藏
        ui->office_textEdit2->hide();
        ui->json_format_widget->hide();
        ui->stackedWidget->setCurrentIndex(1);
    } else if (name == "title_ops_tbtn") {
        ui->stackedWidget->setCurrentIndex(2);
    } else if (name == "title_vpn_tbtn") {
        ui->stackedWidget->setCurrentIndex(3);
    } else if (name == "title_instantly_tbtn"){
        ui->stackedWidget->setCurrentIndex(4);
    } else if(name == "title_settting_tbtn"){
        ui->stackedWidget->setCurrentIndex(5);
    }else if(name == "title_users_tbtn"){
        ui->stackedWidget->setCurrentIndex(6);
    }else if(name == "bbb"){
        QMessageBox msgBox(QMessageBox::Question,"登录","确定要退出登录吗？",QMessageBox::Yes|QMessageBox::No,this);
        if(msgBox.exec() == QMessageBox::Yes){
            close();
        }
    }
}



//----------------------------------窗口最小化最大化和退出到托盘功能-------------------------------
//最小化窗口槽函数
void MainMenu::on_ptn_main_min_clicked(){
    this->showMinimized();
}

//最大化窗口槽函数
void MainMenu::on_ptn_main_max_clicked(){
    static bool max = false;
    static QRect location = this->geometry();

    if (max) {
        this->setGeometry(location);
        ui->ptn_main_max->setIcon(QIcon(":/images/app_maximize.png"));
    } else {
        location = this->geometry();
        this->setGeometry(UI_helper::getScreenRect());
        //最大化时设置恢复图标
        ui->ptn_main_max->setIcon(QIcon(":/images/app_maxrestore.png"));
    }
    this->setProperty("canMove", max);
    max = !max;
}

//点击关闭窗口槽函数
void MainMenu::on_ptn_main_exit_clicked(){
     this->close();
}

//托盘槽函数
void MainMenu::TrayIconAction(QSystemTrayIcon::ActivationReason reason){
    //按显示按钮或双击图标  单击QSystemTrayIcon::Trigger
    if(reason == QSystemTrayIcon::DoubleClick){
        this->showNormal();
    }
}


//重写关闭事件
void MainMenu::closeEvent(QCloseEvent *event){

    ui_helper->showMessageBox("后台提示","-----------系统将隐藏到托盘-----------\n\n-----------Will hide in tray-----------",0,2000);
    this->hide();
    //显示托盘信息
    tray->showMessage("SexySport Platform","已经最小化到托盘,双击打开");
    event->ignore();
}

void MainMenu::recover_vision(){
    this->showNormal();
}



//-----------------------------------//各功能项添加按钮和处理信号槽------------------------------
//各功能项添加按钮和初始化信号槽
void MainMenu::add_func_tbtn(){


    //获取办公按钮下面所有的toolbutton
    QList<QLayout *>office_tools_ly = ui->office_toolBox->findChildren<QLayout *>();


    //所有toolbutton的容器
    QVector<QToolButton*> vector_all_tbtn;

    //循环办按钮下面的所有toolbutton名称
    foreach(QLayout *layout,office_tools_ly){
//        qDebug()<<layout->objectName();
        if(layout->objectName()=="office_tool_layout"){
            int i=0;
            foreach(QString name,tools_func_name){
                //设置头像
                QToolButton * btn = new QToolButton;
                //设置toolbutton大小
                btn->setMaximumSize(100,32);
                btn->setMinimumSize(100,32);
                //设置文字
                btn->setText(name);
                //设置头像
                btn->setIcon(QPixmap(":/images/"+tools_func_ico.at(i)));
                //设置头像大小
                btn->setIconSize(QSize(24,24));

                //设置按钮风格 透明
                btn->setAutoRaise(true);
                //设置文字和图片一起显示
                btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
                //加到 垂直布局中
                ui->office_tool_layout->addWidget(btn);
                //容器保存住9个按钮，方便以后再次操作
                vector_all_tbtn.push_back(btn);
                ++i;
            }
            //添加弹簧填充空的地方
            ui->office_tool_layout->addStretch();
        }else if(layout->objectName()=="office_video_layout"){

        }else if(layout->objectName()=="office_monitor_layout"){

        }else if(layout->objectName()=="office_playchess_layout"){

        }else{
            continue;
        }
    }


    //连接信号槽
    for(int i=0;i<vector_all_tbtn.size();i++){
        connect(vector_all_tbtn[i],&QToolButton::clicked,this,& MainMenu::vector_all_tbtn_handle);
    }


}

//功能项下所有的toolbutton处理
void MainMenu::vector_all_tbtn_handle(){
    //获取发送者
    QToolButton *tbtn_name = (QToolButton *)sender();
//    tbtn_name->setShortcut(QKeySequence(tr("Ctrl+Shift+f")));
    QString name = tbtn_name->text();
//    qDebug()<<name;
    //json格式化窗口隐藏
    ui->office_textEdit2->hide();
    ui->json_format_widget->hide();

    //对按钮进行判断信号操作
    if(name=="截图"){
        ui->office_textEdit->setDisabled(true);
        ui->office_textEdit->setText("截图快捷键:Ctrl+Shift+s 键");
        QProcess *pPrecess = new QProcess(this);
        pPrecess->start("./screenshot/screencapture.exe");
//         QDesktopServices::openUrl(QUrl("file:///"+QCoreApplication::applicationDirPath()+"/screenshot/screencapture.exe"));

    }else if(name=="录制GIF"){
        ui->office_textEdit->setText("录制GIF快捷键:Ctrl+Shift+f 键");
        ui->office_textEdit->setDisabled(true);
        QProcess *pPrecess = new QProcess(new GifWidget());
        pPrecess->start();
        delete pPrecess;

    }else if(name=="获取IP地址"){
        ui->office_textEdit->setDisabled(false);
        //获取局域网ip和MAC地址
        QStringList ip_mac = common_core->getLocalIPs();
        ui->office_textEdit->setText("局域网IP(Local IP address)\n"+ip_mac.at(0)+"\n\nMAC地址(Physical Address)\n"+ip_mac.at(1));
        //获取公网ip
        QString public_ip = common_core->getPublicIP();
        ui->office_textEdit->append("\n公网IP(Public IP address)\n"+public_ip);
        tbtn_name->setDisabled(true);
        //防止再次点击
        ui_helper->sleep(200);
        tbtn_name->setDisabled(false);

    }else if(name=="AES文件加密"){
        ui->office_textEdit->setText("只支持txt文件内容加密");
        ui->office_textEdit->setDisabled(true);
       QString file_path  = ui_helper->getOpenFileName(tr("txt(*.txt)"));
       bool secret_input = false;
       if(!file_path.isEmpty()){
           QString secret_str = QInputDialog::getText(this,"AES对称加密","请输入加密秘钥:",QLineEdit::Normal,"",&secret_input,Qt::FramelessWindowHint);
           if(secret_input && !secret_str.isEmpty()){
               //不够16加到16位数
               while(secret_str.size()<16){
                   secret_str=secret_str+"0";
               }

               QFile file_ = QFile(file_path);
               if(file_.open(( QIODevice::ReadWrite))){
                   QByteArray data_byte = file_.readAll();
                   QString data = data_byte.data();
                   //转为uchar类型
                   QByteArray bytes = secret_str.toLatin1();
                   //初始化秘钥
                   common_core->initialize_private_key(16,(uchar *)bytes.data());
                   //选择保存的路径
                   QString save_file_path = file_path+"_encry.crt";
                   if(!save_file_path.isEmpty()){
                       common_core->str_to_file_encryptor(data,save_file_path);
                       ui_helper->showMessageBox("success","保存成功",3,500);
                       ui->office_textEdit->setText(QString("只支持txt文件内容AES加密\n文件路径: %1").arg(save_file_path));
                   }
               }
               file_.close();
           }
       }

    }else if(name=="AES文件解密"){
       QString file_path  = ui_helper->getOpenFileName(tr("crt(*.crt)"));
       ui->office_textEdit->setText("加密文件格式为xxx.crt");
       ui->office_textEdit->setDisabled(true);
       bool secret_out = false;
       if(!file_path.isEmpty()){
           QString secret_str = QInputDialog::getText(this,"AES解密","请输入秘钥:",QLineEdit::Normal,"",&secret_out,Qt::FramelessWindowHint);
           if(secret_out && !secret_str.isEmpty()){
               //不够16加到16位数
               while(secret_str.size()<16){
                   secret_str=secret_str+"0";
               }
               //转为uchar类型
               QByteArray bytes = secret_str.toLatin1();
               //初始化秘钥
               common_core->initialize_private_key(16,(uchar *)bytes.data());
               //把密文加载到显示栏
               ui->office_textEdit->setText((common_core->file_to_str_decryptor(file_path)).toLocal8Bit());
               ui->office_textEdit->setDisabled(false);
           }
       }

    }else if(name=="Base加解密"){
          ui->office_textEdit->setText("Base加解密");

    }else{
        //开启编辑模式
         ui->office_textEdit->setDisabled(false);
         //显示json格式化窗口
         ui->office_textEdit2->show();
         ui->json_format_widget->show();
         ui->office_textEdit->setText("格式系列化");
    }
    //                   qDebug()<<vTool_tbtn[i]->text()
}


//上传头像
void MainMenu::on_profile_photo_pbtn_clicked()
{
     QString path = QFileDialog::getOpenFileName(this,tr("Open File"), ".", tr("JPG|PNG(*.jpg;*.png)"));
     if(!path.isEmpty()){
         ui->profile_label_pic->setPixmap(path);
     }
}

//离开锁屏
void MainMenu::on_lock_sys_pbtn_clicked()
{
    bool is_lock = false;
    QString secret_lock= QInputDialog::getText(this,"lockScreen sys","请设置解锁密码:\nPlease set the unlock PW:",QLineEdit::Normal,"",&is_lock,Qt::FramelessWindowHint);
    if(is_lock && !secret_lock.isEmpty()){
        ui->all_stackedWidget->setCurrentIndex(1);
        QString auth_str;
        while(is_lock){
            is_lock = false;
            auth_str = QInputDialog::getText(this,"","请输入解锁密码:\nPlease enter the unlock PW:",QLineEdit::Normal,"",&is_lock,Qt::FramelessWindowHint);
            if(is_lock&&!auth_str.isEmpty()){
                if(auth_str.compare(secret_lock) == 0){
                    ui->all_stackedWidget->setCurrentIndex(0);
                    return;
                }
                ui_helper->showMessageBox("password error","解锁密码错误!\nunlock password is wrong!",1);
                is_lock = true;
            }
            is_lock = true;
        }
    }
}


//用户退出系统
void MainMenu::on_exit_sys_pbtn_clicked()
{
    ui_helper->showMessageBox("Bye-bye","感谢使用!bye-bye",0,1500);
    exit(0);
}

////主页tab1-tab3切换
void MainMenu::on_main_tabWidget_currentChanged(int index)
{
    switch(index){
    case 0:{
        ui->main_tab1->show();
        QMediaPlayer *player = new QMediaPlayer();
        player->setMedia(QMediaContent(QUrl("http://vfx.mtime.cn/Video/2019/03/19/mp4/190319212559089721.mp4")));
//        ui->main_news_video->addAction();
        ui->video_axWidget->show();
        QString url2 = QString("http://vfx.mtime.cn/Video/2019/03/19/mp4/190319212559089721.mp4");
        ui->video_axWidget->dynamicCall("Navigate(const QString&)",url2);
//        break;
    }
    case 1:{
        ui->main_tab2->show();

        break;
    }
    case 2:{
        ui->main_tab3;
    }
    }
}

//json格式化按钮
void MainMenu::on_json_format_pbtn_clicked()
{
    if(ui->office_textEdit->toPlainText().isEmpty()){
        ui->office_textEdit2->setText("NULL");
    }else{
        char *out;cJSON *json;

        json=cJSON_Parse(ui->office_textEdit->toPlainText().toLatin1());
        if (!json) {printf("Error before: [%s]\n",cJSON_GetErrorPtr());}
        else
        {
            out=cJSON_Print(json);
            cJSON_Delete(json);
            printf("%s\n",out);
            ui->office_textEdit2->setText(out);
            free(out);
        }
    }
}

//工具栏转为html
void MainMenu::on_office_to_html_pbtn_clicked()
{
    if(ui->office_textEdit->toPlainText().isEmpty()){
        ui->office_textEdit2->setText("NULL");
    }else{
        ui->office_textEdit2->setHtml(ui->office_textEdit->toHtml());
    }
}

//转为Markdown文本
void MainMenu::on_office_to_mkd_pbtn_clicked()
{
    if(ui->office_textEdit->toPlainText().isEmpty()){
        ui->office_textEdit2->setText("NULL");
    }else{
        ui->office_textEdit2->setMarkdown(ui->office_textEdit->toMarkdown());
    }
}

//清空内容
void MainMenu::on_office_clear_pbtn_clicked()
{
    ui->office_textEdit2->setText("");
}

//转为字节流
void MainMenu::on_office_to_bytes_pbtn_clicked()
{
    if(ui->office_textEdit->toPlainText().isEmpty()){
        ui->office_textEdit2->setText("NULL");
    }else{
        ui->office_textEdit2->setText(QByteArray::fromHex(ui->office_textEdit->toPlainText().toLocal8Bit()));
        ui->office_textEdit2;
    }
}

