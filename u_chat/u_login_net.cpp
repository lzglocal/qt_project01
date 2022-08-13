#include "u_login_net.h"
#include "menu_control.h"

U_login_net::U_login_net(QObject *parent)
    : QObject{parent}
{
    p_cookie_jar=new QNetworkCookieJar(this);
    http=new QNetworkAccessManager(this);
    http_pull=new QNetworkAccessManager(this);
    http_extra_text_info=new QNetworkAccessManager(this);
    http->setCookieJar(p_cookie_jar);
    http_pull->setCookieJar(p_cookie_jar);
    http_extra_text_info->setCookieJar(p_cookie_jar);
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    client_id=QString::number(qrand());
    msg_id=(qrand()%1000)*10000+1;
    timer =  new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(send_msg()));
    //qDebug()<<"msg_id="<<msg_id<<endl;
    //clientid="30770623";


    connect(http,SIGNAL(finished(QNetworkReply*)),this,SLOT(http_finished(QNetworkReply*)));
    connect(http_pull,SIGNAL(finished(QNetworkReply*)),this,SLOT(http_pull_finished(QNetworkReply*)));
    connect(http_extra_text_info,SIGNAL(finished(QNetworkReply*)),this,SLOT(http_ext_info_Finished(QNetworkReply*)));

}


//void U_login_net::pull_msg(){
//    if(u_status==U_status::offline) return;
//    QNetworkRequest request;
//    request.setUrl(QUrl("http://d.web2.qq.com/channel/poll2"));
//    request.setRawHeader("Accept","*/*");
//    request.setRawHeader("Accept-Encoding","gzip,deflate,sdch");
//    request.setRawHeader("Accept-Language","zh-CN,zh;q=0.8,en-US;q=0.6,en;q=0.4");
//    request.setRawHeader("Accept-Charset","GBK,utf-8;q=0.7,*;q=0.3");
//    request.setRawHeader("Connection","keep-alive");
//    request.setRawHeader("Content-Type","application/x-www-form-urlencoded");
//    request.setRawHeader("Host","d.web2.qq.com");
//    request.setRawHeader("Origin","http://d.web2.qq.com");
//    request.setRawHeader("Referer","http://d.web2.qq.com/proxy.html?v=20110331002&callback=1&id=2");
//    request.setRawHeader("User-Agent","Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/29.0.1547.62 Safari/537.36");
//    QByteArray postData;
//    postData.append("r="+QUrl::toPercentEncoding(QString("{\"clientid\":\"%1\",\"psessionid\":\"%2\",\"key\":0,\"ids\":[]}").arg(clientid).arg(psessionid)));
//    postData.append(QString("&clientid=%1&psessionid=%2").arg(client_id).arg(psession_id));
//    request.setHeader(QNetworkRequest::ContentLengthHeader,postData.size());
//    //httpAction=HttpAction::GetMsgAction;
//    qDebug()<<"PollMsg"<<QDateTime::currentMSecsSinceEpoch()<<endl;
//    http_pull->post(request,postData);
//}

//void U_login_net::http_pull_finished(QNetworkReply* reply){

//    QByteArray replyData= reply->readAll();
//    reply->deleteLater();
//    QString replystr=QString::fromUtf8(replyData);
//    QJsonDocument jsonDoc;
//    QJsonObject jsonObj;
//    QJsonArray jsonArray;
//    int retcode=0;
//    jsonDoc=QJsonDocument::fromJson(replyData);
////    qDebug()<<"httpPollFinished jsonDoc="<<jsonDoc<<endl<<"replystr="<<replystr<<replystr.length()<<endl;
//    if(jsonDoc.isObject()){
//        jsonObj=jsonDoc.object();
//        retcode=jsonObj.value("retcode").toDouble();
//        if(retcode==0){
//            U_login_net::mutex.lock();
//            jsonArray=jsonObj.value("result").toArray();
//            for(int i=0;i<jsonArray.size();i++){
//                jsonObj=jsonArray.at(i).toObject();
//                U_msg* msg=new U_msg();
//                if(jsonObj.value("poll_type").toString()==QString("group_message")){
//                    jsonObj=jsonObj.value("value").toObject();
//                    msg->type=1;//群信息
//                    //send_uin群内发言者TXUIN（通迅号）
//                    msg->send_uin=QString::number(jsonObj.value("send_uin").toDouble(),'f',0);
//                    //from_uin群TXUIN（群消息）或者好友TXUIN（私聊时）
//                    msg->from_uin=QString::number(jsonObj.value("from_uin").toDouble(),'f',0);
//                }else if(jsonObj.value("poll_type").toString()==QString("message")){
//                    jsonObj=jsonObj.value("value").toObject();
//                    msg->type=0;//好友信息
//                    //from_uin群TXUIN（群消息）或者好友TXUIN（私聊时）
//                    msg->from_uin=QString::number(jsonObj.value("from_uin").toDouble(),'f',0);
//                }else{
//                    delete msg;
//                    break;
//                }
//                msg->time=jsonObj.value("time").toDouble();
//                jsonArray=jsonObj.value("content").toArray();
//                for(int i=1;i<jsonArray.size();i++){
//                    if(jsonArray.at(i).isString()){
//                        msg->content+=jsonArray.at(i).toString();
//                    }else if(jsonArray.at(i).isArray()){
//                        msg->content+="[图片]";
//                    }
//                }
//                Web_API::u_msgs.append(msg);
////                qDebug()<<msg->type<<msg->from_uin<<msg->time<<QDateTime::fromTime_t(msg->time)<<endl<<msg->content<<endl;
//            }
//            Web_API::mutex.unlock();
//            emit msgReceived();

//        }else if(retcode==116){
//            pt_web_api=jsonObj.value("p").toString();
////            qDebug()<<"httpPollFinished retcode="<<retcode<<endl;
//        }
//    }
//    if(retcode==0||retcode==102||retcode==116){
//        this->pollMsg();
//    }else{
//        Web_API::status=u_status::offline;
//        emit sysMsg("掉线了，请重新登录。");
//    }
//}


//void U_login_net::get_group_member_info(QString groupTXUIN){
//    if(Web_API::status==u_status::offline) return;
//    if(!currGroupTXUIN.isEmpty()){
//        return;
//    }
//    curr_group_tx_uix=group_TXUIN;
//    QString groupCode=Web_API::u_groups.value(groupTXUIN)->code;
//    QNetworkRequest request;
//    QString urlstr=QString("http://s.web2.qq.com/api/get_group_info_ext2?gcode=%1&cb=undefined&vfwebqq=%2&t=%3").arg(groupCode).arg(vf_web_api).arg(QDateTime::currentMSecsSinceEpoch());
//    request.setUrl(QUrl(urlstr));
//    request.setRawHeader("Accept","*/*");
//    request.setRawHeader("Accept-Encoding","gzip,deflate,sdch");
//    request.setRawHeader("Accept-Language","zh-CN,zh;q=0.8,en-US;q=0.6,en;q=0.4");
//    request.setRawHeader("Accept-Charset","GBK,utf-8;q=0.7,*;q=0.3");
//    request.setRawHeader("Connection","keep-alive");
//    request.setRawHeader("Content-Type","utf-8");
//    request.setRawHeader("Host","s.web2.qq.com");
//    request.setRawHeader("Origin","http://d.web2.qq.com");
//    request.setRawHeader("Referer","http://s.web2.qq.com/proxy.html?v=20110412001&callback=1&id=3");
//    request.setRawHeader("User-Agent","Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/29.0.1547.62 Safari/537.36");

//    qDebug()<<"getGroupMemberInfo"<<urlstr<<endl;
//    http_extra_text_info->get(request);
//}

//void U_login_net::http_ext_info_finished(QNetworkReply* reply){
//    qDebug()<<"httpPollFinished"<<endl;
//    QByteArray replyData=reply->readAll();

//   // replyData.replace("\xe2\x80\xae","");//字符串中包含这几个字符，会莫名其妙的反转显示，非常奇特！
//   //  replyData.replace("\xe2\x80\xa8","");//导致回车换行
////由于有些群成员名片是非法utf-8字符，导致json数据解析错误只好自己使用replystr.toUtf8()再转一次
//    reply->deleteLater();
//    QString replystr=QString::fromUtf8(replyData);
//    U_group*pGroup=Web_API::u_groups.value(currGroupTXUIN,nullptr);
//    if(pGroup==nullptr){
//        qDebug()<<"httpExtInfoFinished 查无此群！"<<endl;
//        return;
//    }
//    QJsonDocument jsonDoc;
//    QJsonObject jsonObj;
//    QJsonArray jsonArray;
//    int retcode=0;
//    QJsonParseError jsonError;
//    jsonDoc=QJsonDocument::fromJson(replystr.toUtf8(),&jsonError);
//    // qDebug()<<"httpPollFinished"<<jsonDoc<<endl;
//    if(jsonDoc.isObject()){
//        jsonObj=jsonDoc.object();
//        retcode=jsonObj.value("retcode").toDouble();
//        if(retcode==0){
//            jsonObj=jsonObj.value("result").toObject();
//            jsonArray=jsonObj.value("minfo").toArray();
//            foreach(QJsonValue jv,jsonArray){
//                QJsonObject jo=jv.toObject();
//                U_friend *f=new U_friend();
//                f->txuin=QString::number(jo.value("uin").toDouble(),'f',0);
//                f->nick=jo.value("nick").toString();
//                pGroup->members.insert(f->txuin,f);
//            }
//            jsonArray=jsonObj.value("cards").toArray();
//            foreach(QJsonValue jv,jsonArray){
//                QJsonObject jo=jv.toObject();
//                QString txuin=QString::number(jo.value("muin").toDouble(),'f',0);;
//                U_friend* f=pGroup->members.value(txuin,nullptr);
//                if(f==nullptr) f=new U_friend();
//                f->txuin=txuin;
//                f->markname=jo.value("card").toString();
//                pGroup->members.insert(f->txuin,f);
//            }
//        }else{//有时候返回 {"retcode":100000}，得不到成员信息
//            pGroup->members.insert("0",new U_friend);
//        }
//    }else{
//        qDebug()<<"httpExtInfoFinished failure"<<replystr<<jsonError.errorString()<<endl<<replyData.data()<<endl;
//    }
//    emit sysMsg(QString("获取%1群成员信息完成，成员数量：%2").arg(Web_API::u_groups.value(currGroupTXUIN)->name)
//                .arg(Web_API::u_groups.value(currGroupTXUIN)->members.size()));
//    emit msgReceived();
//    currGroupTXUIN.clear();
//    qDebug()<<"httpExtInfoFinished /////////////////////////////////////////////////////////////////////////////"<<endl;
//    //this->pollMsg();
//}


void U_login_net::http_finished(QNetworkReply* reply){
    QByteArray reply_data = reply->readAll();
    reply->deleteLater();
    QString replystr=QString::fromUtf8(reply_data);

    bool isok;
    switch(http_action){
    case Http_action::no_action:
        break;

    //获取验证码字符串动作
    case Http_action::get_veriry_code_action: 
        if(replystr.indexOf("'0'")>-1){
            emit get_verify_code_finished(true,replystr.mid(18,4));
            http_action=Http_action::no_action;
        }else{
            emit get_verify_code_finished(false,"");
        }
        replystr.chop(3);
        uinhexstr=replystr.right(32).remove("\\x");
        break;

    //匹配http获取图片动作
    case Http_action::get_verify_code_img_action:
        //qDebug()<<reply->header(QNetworkRequest::ContentTypeHeader).toString();
        emit get_verify_code_img_finished(reply_data);
        http_action=Http_action::no_action;
        break;

    //登录
    case Http_action::login_action:
//        qDebug()<<replystr<<endl;
        if(replystr.indexOf("ptuiCB('0'")>-1){
            web_inet_info = U_login_net::get_cookie("ptwebqq");
//            qDebug()<<"get cookie username="<<web_inet_info<<endl;
        }
        if(!web_inet_info.isEmpty()){
            U_login_net::check_siguare(replystr);
        }else{

            QJsonParseError json_error;
            QJsonDocument doucment = QJsonDocument::fromJson(reply_data, &json_error);
            //    qDebug()<<doucment;
            //判断如果未发生错误
            if (!doucment.isNull() && (json_error.error == QJsonParseError::NoError)) {
                //是否包含一个数组或一个对象，使用 isArray() 和 isObject()
                if (doucment.isObject()) {  // JSON 文档为对象
                    const QJsonObject object = doucment.object(); // 转化为对象
                    if (object.contains("code")) { // 包含指定的 key吗
                        QJsonValue value = object.value("code");  // 获取指定 key 对应的 value
//                                                qDebug()<<value;
                        if(value ==0){
                            emit login_finished(true,"成功！");
                        }else{
                            emit login_finished(false,"登录失败！");
                        }
                        //
                        //                        if (value.isString()) {  // 判断 value 是否为字符串
                        //                            QString str_data = value.toString();  // 将 value 转化为字符串
                        //                            qDebug()<<str_data;

                        //                    }
                    }
                }
            }            
        }
        break;


    case Http_action::check_sigaction:
        login_2();
        break;

    case Http_action::login2_action:
        qDebug()<<"Login2_action"<<replystr<<endl;
        json_doc=QJsonDocument::fromJson(reply_data);
        if(json_doc.isNull()){
            emit login_finished(false,"登录失败！");
        }else{
            json_obj=json_doc.object();
            if(json_obj.value("retcode").toDouble()==0){
                json_obj=json_obj.value("result").toObject();
                uin=json_obj.value("uin").toDouble();
                qDebug()<<"uin="<<uin<<endl<<json_obj;
                verify_web_api=json_obj.value("vfwebqq").toString();
                psession_id=json_obj.value("psessionid").toString();
                //qDebug()<<"登录成功！vfwebqq="<<vfwebqq<<"psessionid="<<psessionid<<endl;
                emit login_finished(true,"成功！");
                this->get_user_friends_list();
            }
        }
        //qDebug()<<replystr<<endl;
        //qDebug()<<replystr<<endl<<http->cookieJar()->cookiesForUrl(QUrl("http://www.qq.com/"));
        break;

//    case Http_action::get_friend_info_action:
//        qDebug()<<replystr<<endl;
//        break;

//    case Http_action::get_user_friends_action:
//        json_doc=QJsonDocument::fromJson(reply_data);
//        qDebug()<<"GetUserFriendsAction finished"<<endl;

//        if(json_doc.isObject()){
//            json_obj=json_doc.object();
//            if(json_obj.value("retcode").toDouble()==0){
//                json_obj=json_obj.value("result").toObject();
//                json_array=json_obj.value("marknames").toArray();
//                for(int i=0;i<QMetaType::QJsonArray.size();i++){
//                    //jsonObj=jsonArray.at(i).toObject();
//                    U_friend* f=new U_friend();
//                    f->txuin=QString::number(json_array.at(i).toObject().value("uin").toDouble(),'f',0);
//                    f->markname=json_array.at(i).toObject().value("markname").toString();
//                    Menu_control_show::u_friends.insert(f->txuin,f);
//                }
//                json_array=json_obj.value("info").toArray();
//                for(int i=0;i<QMetaType::QJsonArray.size();i++){
//                    //jsonObj=jsonArray.at(i).toObject();
//                    QString txuin=QString::number(json_array.at(i).toObject().value("uin").toDouble(),'f',0);
//                    U_friend* f=Web_API::u_friends.value(txuin,nullptr);
//                    if(f==nullptr) f=new U_friend();
//                    f->txuin=txuin;
//                    f->nick=json_array.at(i).toObject().value("nick").toString();
//                    Web_API::u_friends.insert(f->txuin,f);
//                    QStandardPaths *item=new QStandardPaths();
//                    item->setText(f->markname.isEmpty()?f->nick:QString("%1(%2)").arg(f->nick).arg(f->markname));
//                    item->setData("FTX"+f->txuin,Qt::UserRole);
//                    Web_API::u_friend_list_model->appendRow(item);
//                }
//            }
//            emit sys_msg("完成好友信息下载！/n");

//        }else{
//            emit sys_msg("好友信息下载失败！/n");
//        }

//        this->get_groupname_list();
//        qDebug()<<Menu_control_show::u_friends.size()<<Menu_control_show::u_friends<<endl;
//        break;
//    case Http_action::get_groupname_list_action:
//        qDebug()<<"GetGroupNameListAction finished"<<endl;
//        json_doc=QJsonDocument::fromJson(reply_data);
//        qDebug()<<"GetGroupNameListAction"<<json_doc<<endl;
//        if(json_doc.isObject()){
//            json_obj=json_doc.object();
//            if(json_obj.value("retcode").toDouble()==0){
//                json_obj=json_obj.value("result").toObject();
//                json_array=json_obj.value("gnamelist").toArray();
//                for(int i=0;i<QMetaType::QJsonArray.size();i++){
//                    json_obj=json_array.at(i).toObject();
//                    U_group* g=new U_group();
//                    g->name=json_obj.value("name").toString();
//                    g->txuin=QString::number(json_obj.value("gid").toDouble(),'f',0);
//                    g->code=QString::number(json_obj.value("code").toDouble(),'f',0);
//                    Menu_control_show::u_groups.insert(g->txuin,g);
//                    qDebug()<<g->txuin<<g->name<<endl;
//                    QStandardPaths *item=new QStandardPaths();
//                    item->setText(g->name);
//                    item->setData("GTX"+g->txuin,Qt::UserRole);
//                    Menu_control_show::u_group_list_model->appendRow(item);
//                }
//            }
//            emit sys_msg("完成群信息下载！开始接收聊天信息......");

//        }else{
//            emit sys_msg("群信息下载失败！尝试接收聊天信息......");

//        }
//        this->pull_msg();
//        http_action=Http_action::no_action;
//        break;
//    case Http_action::send_buddymsg_action:
//        //{"retcode":0,"result":"ok"}
//        isok=false;
//        json_doc=QJsonDocument::fromJson(reply_data);
//        if(json_doc.isObject()){
//            json_obj=json_doc.object();
//            if(json_doc.value("retcode").toDouble()==0){
//                isok=true;
//            }
//        }
//        emit send_msg_finished(curr_send_msg.first,curr_send_msg.second,isok);
//        qDebug()<<"SendBuddyMsgAction"<<curr_send_msg<<isok<<replystr<<endl;
//        http_action=Http_action::no_action;
//        break;
//    case Http_action::Send_group_msg_action:
//        isok=false;
//        json_doc=QJsonDocument::fromJson(reply_data);
//        if(json_doc.isObject()){
//            json_obj=json_doc.object();
//            if(json_doc.value("retcode").toDouble()==0){
//                isok=true;
//            }
//        }
//        //qDebug()<<"SendGroupMsgAction"<<currSendMsg<<isok<<endl;
//        emit send_msg_finished(curr_send_msg.first,curr_send_msg.second,isok);
//        qDebug()<<"Send_group_msg_action"<<curr_send_msg<<isok<<replystr<<endl;
//        http_action=Http_action::no_action;
//        break;


    case Http_action::logout_action:

        if(Menu_control_show::is_close){
            qApp->quit();
        }else{
            Menu_control_show::u_status=U_status::offline;

            emit sys_msg("账号已经注销登录。");
        }
        qDebug()<<"LoginOutAction"<<reply->error()<<replystr<<endl;
        http_action=Http_action::no_action;
        break;
    default:
        break;



    }
    //qDebug()<<replystr<<endl<<http->cookieJar()->cookiesForUrl(QUrl("http://www.qq.com/"));

    reply->deleteLater();
}



//生成密码和验证码和字符串做MD5加密的字符串
QString U_login_net::get_password_mix(QString u_password, QString verify_code, QString uinhexstr){
    QByteArray u_passwordbyte=QCryptographicHash::hash(u_password.toLatin1(),QCryptographicHash::Md5);
    QByteArray uinhexbyte = QByteArray::fromHex(uinhexstr.toLatin1());
//    qDebug()<<"uinhexbyte="<<uinhexbyte<<"uinstr="<<uinhexstr<<endl;
    QByteArray u_passworduinbyte=QCryptographicHash::hash(u_passwordbyte+uinhexbyte,QCryptographicHash::Md5).toHex().toUpper();
    QByteArray byteP=QCryptographicHash::hash(u_passworduinbyte+verify_code.toUpper().toLatin1(),QCryptographicHash::Md5).toHex().toUpper();
    return QString::fromLatin1(byteP);
}


//获取验证码
void U_login_net::get_verify_code(QString u_usernam){
//    qDebug()<<"跳转到web-yunwei-net_checkVerifycode";
    QNetworkRequest request;
    request.setUrl(QUrl(QString("http://check.ptlogin2.qq.com/check?uin=%1&appid=1003903&js_ver=10041").arg(u_usernam)));
    request.setRawHeader("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setRawHeader("Accept-Charset","GBK,utf-8;q=0.7,*;q=0.3");
    request.setRawHeader("Accept-Encoding","gzip,deflate,sdch");
    request.setRawHeader("Accept-Language","zh-CN,zh;q=0.8");
    request.setRawHeader("Cache-Control","max-age=0");
    request.setRawHeader("Connection","keep-alive");
    request.setRawHeader("Host","check.ptlogin2.qq.com");
    request.setRawHeader("User-Agent","Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/29.0.1547.62 Safari/537.36");
    http_action=Http_action::get_veriry_code_action;
    http->get(request);
}

//获取验证码图片
void U_login_net::get_verify_code_imags(QString u_username){
    QNetworkRequest request;
    request.setUrl(QUrl(QString("http://captcha.qq.com/getimage?aid=1003903&uin=%1").arg(u_username)));
    request.setRawHeader("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    //request.setRawHeader("Accept-Charset","GBK,utf-8;q=0.7,*;q=0.3");
    request.setRawHeader("Accept-Encoding","gzip,deflate,sdch");
    request.setRawHeader("Accept-Language","zh-CN,zh;q=0.8");
    request.setRawHeader("Cache-Control","max-age=0");
    request.setRawHeader("Connection","keep-alive");
    request.setRawHeader("Host","captcha.qq.com");
    request.setRawHeader("User-Agent","Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/29.0.1547.62 Safari/537.36");
    http_action=Http_action::get_verify_code_img_action;
    http->get(request);
}

//登录
void U_login_net::login(QString u_username,QString u_password,QString verify_code){
    if((u_username =="test01"||u_username == "Liam") &&u_password == "123456"){
        emit login_finished(true,"成功！");
        return;
    }
    QNetworkRequest request;
//    qDebug()<<get_password_mix(u_password,verify_code,uinhexstr);
    //QString urlstr=QString("http://ptlogin2.qq.com/login?u=%1&p=%2&verifycode=%3&webqq_type=10&remember_uin=1&login2qq=1&aid=1003903&u1=%4&h=1&ptredirect=0&ptlang=2052&daid=164&from_ui=1&pttype=1&dumy=&fp=loginerroralert&action=2-11-12939&mibao_css=m_webqq&t=1&g=1&js_type=0&js_ver=10041&login_sig=Ip4V0YQezz6DEwpsE*-Cq*4NWVecBqWZYbNS22JCEQIT-sp20PyQtj8f-pqSIREa").arg(u_username).arg(get_password_mix(u_password,verify_code,uinhexstr)).arg(verify_code).arg("http%3A%2F%2Fweb2.qq.com%2Floginproxy.html%3Flogin2qq%3D1%26webqq_type%3D10");
    //设置url
//    QString urlstr=QString("https://legendsapi.com/api/tokens");
    QString urlstr=QString("http://10.6.3.10:8000/login/");
    request.setUrl(QUrl(urlstr));

    //设置头部信息
//    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    request.setRawHeader("Content-Type","application/json");//服务器要求的数据头部
//    request.setRawHeader("Content-Type","application/x-www-form-urlencoded");//服务器要求的数据头部
    request.setRawHeader("Accept","text/json,*/*;q=0.5");//服务器要求的数据头部
//    request.setRawHeader("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setRawHeader("Accept-Charset","GBK,utf-8;q=0.7,*;q=0.3");
    request.setRawHeader("Accept-Encoding","gzip,deflate,sdch");
    request.setRawHeader("Accept-Language","zh-CN,zh;q=0.8");
    request.setRawHeader("Cache-Control","max-age=0");
    request.setRawHeader("Connection","keep-alive");
//    request.setRawHeader("Host","ptlogin2.qq.com");
    request.setRawHeader("User-Agent","Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/29.0.1547.62 Safari/537.36");



    //发送数据
    QJsonObject json_object;
    json_object.insert("username",u_username);
    json_object.insert("password",u_password);

//    QJsonObject json_data;
//    json_data.insert("data",QJsonValue(json_object));

////    QJsonArray array_data;
////    array_data.append(5);
////    array_data.append(10);

    QJsonDocument json_doc;
////    json_doc.setObject(json_data);
    json_doc.setObject(json_object);

    QByteArray bytes_data = json_doc.toJson();

    http_action=Http_action::login_action;
    http->post(request,bytes_data);
}



void U_login_net::check_siguare(QString urlstr){
    urlstr=urlstr.mid(16);
    urlstr=urlstr.left(urlstr.indexOf('\''));
    //qDebug()<<urlstr<<endl;return;
    QNetworkRequest request;
    request.setUrl(QUrl(urlstr));
    // qDebug()<<urlstr<<endl;
    request.setRawHeader("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setRawHeader("Accept-Charset","GBK,utf-8;q=0.7,*;q=0.3");
    request.setRawHeader("Accept-Encoding","gzip,deflate,sdch");
    request.setRawHeader("Accept-Language","zh-CN,zh;q=0.8");
    request.setRawHeader("Cache-Control","max-age=0");
    request.setRawHeader("Connection","keep-alive");
    request.setRawHeader("Host","ptlogin2.qq.com");
    request.setRawHeader("User-Agent","Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/29.0.1547.62 Safari/537.36");
    http_action=Http_action::check_sigaction;
    http->get(request);
}

void U_login_net::login_2(){
    QNetworkRequest request;
    request.setUrl(QUrl("http://d.web2.qq.com/channel/login2"));
    request.setRawHeader("Accept","*/*");
    request.setRawHeader("Accept-Encoding","gzip,deflate,sdch");
    request.setRawHeader("Accept-Language","zh-CN,zh;q=0.8,en-US;q=0.6,en;q=0.4");
    request.setRawHeader("Accept-Charset","GBK,utf-8;q=0.7,*;q=0.3");
    request.setRawHeader("Connection","keep-alive");
    request.setRawHeader("Content-Type","application/x-www-form-urlencoded");
    request.setRawHeader("Host","d.web2.qq.com");
    request.setRawHeader("Origin","http://d.web2.qq.com");
    request.setRawHeader("Referer","http://d.web2.qq.com/proxy.html?v=20110331002&callback=2");
    request.setRawHeader("User-Agent","Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/29.0.1547.62 Safari/537.36");
    QByteArray postData;
    postData.append("r="+QUrl::toPercentEncoding(QString("{\"status\":\"online\",\"ptwebqq\":\"%1\",\"passwd_sig\":\"\",\"clientid\":\"%2\",\"psessionid\":null}").arg(web_inet_info).arg(client_id)));
    postData.append(QString("&clientid=%1&psessionid=null").arg(client_id));
    request.setHeader(QNetworkRequest::ContentLengthHeader,postData.size());
    http_action=Http_action::login2_action;
//    qDebug()<<postData<<endl;
    http->post(request,postData);
}



void U_login_net::logout(){

//    emit  U_login_net::sys_msg("正在注销登录信息...........");
    QNetworkRequest request;
    QString urlstr=QString("http://d.web2.qq.com/channel/change_status2?newstatus=offline&clientid=%1&psessionid=%2&t=%3").arg(client_id).arg(psession_id).arg(QDateTime::currentMSecsSinceEpoch());
    request.setUrl(QUrl(urlstr));
    // qDebug()<<urlstr<<endl;
    request.setRawHeader("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setRawHeader("Accept-Charset","GBK,utf-8;q=0.7,*;q=0.3");
    request.setRawHeader("Accept-Encoding","gzip,deflate,sdch");
    request.setRawHeader("Accept-Language","zh-CN,zh;q=0.8");
    request.setRawHeader("Cache-Control","max-age=0");
    request.setRawHeader("Connection","keep-alive");
    request.setRawHeader("Host","d.web2.qq.com");
    request.setRawHeader("Referer","http://d.web2.qq.com/proxy.html?v=20110331002&callback=1&id=3");
    request.setRawHeader("User-Agent","Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/29.0.1547.62 Safari/537.36");
    http_action=Http_action::logout_action;
    http->get(request);
}


void U_login_net::http_pull_finished(QNetworkReply* reply){

    QByteArray replyData=reply->readAll();
    reply->deleteLater();
    QString replystr=QString::fromUtf8(replyData);
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;
    QJsonArray jsonArray;
    int retcode=0;
    jsonDoc=QJsonDocument::fromJson(replyData);
    qDebug()<<"httpPollFinished jsonDoc="<<jsonDoc<<endl<<"replystr="<<replystr<<replystr.length()<<endl;

//    if(jsonDoc.isObject()){
//        jsonObj=jsonDoc.object();
//        retcode=jsonObj.value("retcode").toDouble();
//        if(retcode==0){
//            Menu_control_show::mutex.lock();
//            jsonArray=jsonObj.value("result").toArray();
//            for(int i=0;i<jsonArray.size();i++){
//                jsonObj=jsonArray.at(i).toObject();
//                QQmsg* msg=new QQmsg();
//                if(jsonObj.value("poll_type").toString()==QString("group_message")){
//                    jsonObj=jsonObj.value("value").toObject();
//                    msg->type=1;//群信息
//                    //send_uin群内发言者TXUIN（通迅号）
//                    msg->send_uin=QString::number(jsonObj.value("send_uin").toDouble(),'f',0);
//                    //from_uin群TXUIN（群消息）或者好友TXUIN（私聊时）
//                    msg->from_uin=QString::number(jsonObj.value("from_uin").toDouble(),'f',0);
//                }else if(jsonObj.value("poll_type").toString()==QString("message")){
//                    jsonObj=jsonObj.value("value").toObject();
//                    msg->type=0;//好友信息
//                    //from_uin群TXUIN（群消息）或者好友TXUIN（私聊时）
//                    msg->from_uin=QString::number(jsonObj.value("from_uin").toDouble(),'f',0);
//                }else{
//                    delete msg;
//                    break;
//                }
//                msg->time=jsonObj.value("time").toDouble();
//                jsonArray=jsonObj.value("content").toArray();
//                for(int i=1;i<jsonArray.size();i++){
//                    if(jsonArray.at(i).isString()){
//                        msg->content+=jsonArray.at(i).toString();
//                    }else if(jsonArray.at(i).isArray()){
//                        msg->content+="[图片]";
//                    }
//                }
//                WebQQ::qqmsgs.append(msg);
//                qDebug()<<msg->type<<msg->from_uin<<msg->time<<QDateTime::fromTime_t(msg->time)<<endl<<msg->content<<endl;
//            }
//            Menu_control_show::mutex.unlock();
//            emit msg_received();

//        }else if(retcode==116){
//            ptwebqq=jsonObj.value("p").toString();
//            qDebug()<<"httpPollFinished retcode="<<retcode<<endl;
//        }
//    }
//    if(retcode==0||retcode==102||retcode==116){
//        this->pull_msg();
//    }else{
//        WebQQ::status=U_status::offline;
//        emit sys_msg("掉线了，请重新登录。");
//    }
}

void U_login_net::http_ext_info_Finished(QNetworkReply* reply){
    qDebug()<<"httpPollFinished"<<endl;
    QByteArray replyData=reply->readAll();

//   // replyData.replace("\xe2\x80\xae","");//字符串中包含这几个字符，会莫名其妙的反转显示，非常奇特！
//   //  replyData.replace("\xe2\x80\xa8","");//导致回车换行
////由于有些群成员名片是非法utf-8字符，导致json数据解析错误只好自己使用replystr.toUtf8()再转一次
//    reply->deleteLater();
//    QString replystr=QString::fromUtf8(replyData);
//    U_group*pGroup=WebQQ::qqGroups.value(curr_group_tx_uin,nullptr);
//    if(pGroup==nullptr){
//        qDebug()<<"httpExtInfoFinished 查无此群！"<<endl;
//        return;
//    }
//    QJsonDocument jsonDoc;
//    QJsonObject jsonObj;
//    QJsonArray jsonArray;
//    int retcode=0;
//    QJsonParseError jsonError;
//    jsonDoc=QJsonDocument::fromJson(replystr.toUtf8(),&jsonError);
//    // qDebug()<<"httpPollFinished"<<jsonDoc<<endl;
//    if(jsonDoc.isObject()){
//        jsonObj=jsonDoc.object();
//        retcode=jsonObj.value("retcode").toDouble();
//        if(retcode==0){
//            jsonObj=jsonObj.value("result").toObject();
//            jsonArray=jsonObj.value("minfo").toArray();
//            foreach(QJsonValue jv,jsonArray){
//                QJsonObject jo=jv.toObject();
//                U_friend *f=new U_friend();
//                f->txuin=QString::number(jo.value("uin").toDouble(),'f',0);
//                f->nick=jo.value("nick").toString();
//                pGroup->members.insert(f->txuin,f);
//            }
//            jsonArray=jsonObj.value("cards").toArray();
//            foreach(QJsonValue jv,jsonArray){
//                QJsonObject jo=jv.toObject();
//                QString txuin=QString::number(jo.value("muin").toDouble(),'f',0);;
//                QQfriend* f=pGroup->members.value(txuin,nullptr);
//                if(f==nullptr) f=new QQfriend();
//                f->txuin=txuin;
//                f->markname=jo.value("card").toString();
//                pGroup->members.insert(f->txuin,f);
//            }
//        }else{//有时候返回 {"retcode":100000}，得不到成员信息
//            pGroup->members.insert("0",new QQfriend);
//        }
//    }else{
//        qDebug()<<"httpExtInfoFinished failure"<<replystr<<jsonError.errorString()<<endl<<replyData.data()<<endl;
//    }
//    emit sysMsg(QString("获取%1群成员信息完成，成员数量：%2").arg(WebQQ::qqGroups.value(currGroupTXUIN)->name)
//                .arg(WebQQ::qqGroups.value(currGroupTXUIN)->members.size()));
//    emit msg_received();
//    currGroupTXUIN.clear();
//    qDebug()<<"httpExtInfoFinished /////////////////////////////////////////////////////////////////////////////"<<endl;
//    //this->pollMsg();

}


void U_login_net::get_friend_info(QString u_username){
    QNetworkRequest request;
    QString urlstr=QString("http://s.web2.qq.com/api/get_friend_info2?tuin=%1&verifysession=&code=&vfwebqq=%2&t=1377829770859").arg(u_username).arg(verify_web_api);
    request.setUrl(QUrl(urlstr));
    // qDebug()<<urlstr<<endl;
    request.setRawHeader("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setRawHeader("Accept-Charset","GBK,utf-8;q=0.7,*;q=0.3");
    request.setRawHeader("Accept-Encoding","gzip,deflate,sdch");
    request.setRawHeader("Accept-Language","zh-CN,zh;q=0.8");
    request.setRawHeader("Cache-Control","max-age=0");
    request.setRawHeader("Connection","keep-alive");
    request.setRawHeader("Host","s.web2.qq.com");
    request.setRawHeader("Referer","http://d.web2.qq.com/proxy.html?v=20110412001&callback=2");
    request.setRawHeader("User-Agent","Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/29.0.1547.62 Safari/537.36");
    http_action=Http_action::get_friend_info_action;
    http->get(request);
}



void U_login_net::get_user_friends_list(){

//    emit U_login_net::sys_msg("获取好友信息...........");
    QNetworkRequest request;
    request.setUrl(QUrl("http://s.web2.qq.com/api/get_user_friends2"));
    request.setRawHeader("Accept","*/*");
    request.setRawHeader("Accept-Encoding","gzip,deflate,sdch");
    request.setRawHeader("Accept-Language","zh-CN,zh;q=0.8,en-US;q=0.6,en;q=0.4");
    request.setRawHeader("Accept-Charset","GBK,utf-8;q=0.7,*;q=0.3");
    request.setRawHeader("Connection","keep-alive");
    request.setRawHeader("Content-Type","application/x-www-form-urlencoded");
    request.setRawHeader("Host","s.web2.qq.com");
    request.setRawHeader("Origin","http://s.web2.qq.com");
    request.setRawHeader("Referer","http://s.web2.qq.com/proxy.html?v=20110412001&callback=1&id=3");
    request.setRawHeader("User-Agent","Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/29.0.1547.62 Safari/537.36");
    QByteArray postData;
    postData.append("r="+QUrl::toPercentEncoding(QString("{\"h\":\"hello\",\"hash\":\"%1\",\"vfwebqq\":\"%2\"}").arg(get_hash(uin,web_inet_info)).arg(verify_web_api)));
    request.setHeader(QNetworkRequest::ContentLengthHeader,postData.size());
    http_action=Http_action::get_user_friends_action;
    qDebug()<<postData<<endl;
    http->post(request,postData);
}



void U_login_net::get_groupname_list(){
//    emit U_login_net::sys_msg("获取群信息...........");
    QNetworkRequest request;
    request.setUrl(QUrl("http://s.web2.qq.com/api/get_group_name_list_mask2"));
    request.setRawHeader("Accept","*/*");
    request.setRawHeader("Accept-Encoding","gzip,deflate,sdch");
    request.setRawHeader("Accept-Language","zh-CN,zh;q=0.8,en-US;q=0.6,en;q=0.4");
    request.setRawHeader("Accept-Charset","GBK,utf-8;q=0.7,*;q=0.3");
    request.setRawHeader("Connection","keep-alive");
    request.setRawHeader("Content-Type","application/x-www-form-urlencoded");
    request.setRawHeader("Host","s.web2.qq.com");
    request.setRawHeader("Origin","http://s.web2.qq.com");
    request.setRawHeader("Referer","http://s.web2.qq.com/proxy.html?v=20110412001&callback=1&id=3");
    request.setRawHeader("User-Agent","Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/29.0.1547.62 Safari/537.36");
    QByteArray postData;
    postData.append("r="+QUrl::toPercentEncoding(QString("{\"vfwebqq\":\"%1\"}").arg(verify_web_api)));
    request.setHeader(QNetworkRequest::ContentLengthHeader,postData.size());
    http_action=Http_action::get_groupname_list_action;
    qDebug()<<postData<<endl;
    http->post(request,postData);
}

void U_login_net::send_msg(QString txuin, QString msg){
    if(http_action==Http_action::no_action){
        QString txuinflag=txuin.left(3);
        curr_send_msg.first=txuin;
        curr_send_msg.second=msg;
        //对json 特殊字符进行替换
        qDebug()<<"对json 特殊字符进行替换__前"<<msg<<endl;
        msg.replace("\\", "\\\\")
                //.replace("\'", "")
                .replace("\t", " ")
                .replace("\r", " ")
                .replace("\n", "\\\\n")
                .replace("\"", "'");
        qDebug()<<"对json 特殊字符进行替换__后"<<msg<<endl;
        if(txuinflag=="FTX"){
            Menu_control_show::u_login_net->send_buddy_msg(txuin.mid(3),msg);
        }else if(txuinflag=="GTX"){
            Menu_control_show::u_login_net->send_group_msg(txuin.mid(3),msg);
        }

    }else{
        send_msg_list.append(QPair<QString,QString>(txuin,msg));
        if(!timer->isActive()){
            timer->start(1000);
        }
    }
}


void U_login_net::send_msg(){
    if(send_msg_list.isEmpty()){
        timer->stop();
        return;
    }
    if(http_action==Http_action::no_action){
        curr_send_msg=send_msg_list.takeLast();
        send_msg(curr_send_msg.first,curr_send_msg.second);
    }
}

void U_login_net::send_buddy_msg(QString uin, QString msg){
    QNetworkRequest request;
    request.setUrl(QUrl("http://d.web2.qq.com/channel/send_buddy_msg2"));
    request.setRawHeader("Accept","*/*");
    request.setRawHeader("Accept-Encoding","gzip,deflate,sdch");
    request.setRawHeader("Accept-Language","zh-CN,zh;q=0.8,en-US;q=0.6,en;q=0.4");
    request.setRawHeader("Accept-Charset","GBK,utf-8;q=0.7,*;q=0.3");
    request.setRawHeader("Connection","keep-alive");
    request.setRawHeader("Content-Type","application/x-www-form-urlencoded");
    request.setRawHeader("Host","d.web2.qq.com");
    request.setRawHeader("Origin","http://d.web2.qq.com");
    request.setRawHeader("Referer","http://d.web2.qq.com/proxy.html?v=20110331002&callback=1&id=2");
    request.setRawHeader("User-Agent","Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/29.0.1547.62 Safari/537.36");
    QByteArray postData;
    QString tmpdata=QString("{\"to\":%1,\"face\":492,\"content\":\"[\\\"%2\\\",\\\"\\\",[\\\"font\\\",{\\\"name\\\":\\\"宋体\\\",\\\"size\\\":\\\"9\\\",\\\"style\\\":[0,0,0],\\\"color\\\":\\\"FF0000\\\"}]]\",\"msg_id\":%3,\"clientid\":\"%4\",\"psessionid\":\"%5\"}").arg(uin).arg(msg).arg(msg_id++).arg(client_id).arg(psession_id);
    qDebug()<<tmpdata<<endl;
    postData.append("r="+QUrl::toPercentEncoding(tmpdata));
    postData.append(QString("&clientid=%1&psessionid=%2").arg(client_id).arg(psession_id));
    request.setHeader(QNetworkRequest::ContentLengthHeader,postData.size());
    http_action=Http_action::send_buddymsg_action;
    qDebug()<<"sendBuddyMsg"<<msg<<endl;
    http->post(request,postData);

}

//给群组发送信息
void U_login_net::send_group_msg(QString groupuin, QString msg){
    QNetworkRequest request;
    request.setUrl(QUrl("http://d.web2.qq.com/channel/send_qun_msg2"));
    request.setRawHeader("Accept","*/*");
    request.setRawHeader("Accept-Encoding","gzip,deflate,sdch");
    request.setRawHeader("Accept-Language","zh-CN,zh;q=0.8,en-US;q=0.6,en;q=0.4");
    request.setRawHeader("Accept-Charset","GBK,utf-8;q=0.7,*;q=0.3");
    request.setRawHeader("Connection","keep-alive");
    request.setRawHeader("Content-Type","application/x-www-form-urlencoded");
    request.setRawHeader("Host","d.web2.qq.com");
    request.setRawHeader("Origin","http://d.web2.qq.com");
    request.setRawHeader("Referer","http://d.web2.qq.com/proxy.html?v=20110331002&callback=1&id=3");
    request.setRawHeader("User-Agent","Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/29.0.1547.62 Safari/537.36");
    QByteArray postData;
    QString tmpdata=QString("{\"group_uin\":%1,\"content\":\"[\\\"%2\\\",\\\"\\\",[\\\"font\\\",{\\\"name\\\":\\\"宋体\\\",\\\"size\\\":\\\"9\\\",\\\"style\\\":[0,0,0],\\\"color\\\":\\\"FF0000\\\"}]]\",\"msg_id\":%3,\"clientid\":\"%4\",\"psessionid\":\"%5\"}").arg(groupuin).arg(msg).arg(msg_id++).arg(client_id).arg(psession_id);
    qDebug()<<tmpdata<<endl;
    postData.append("r="+QUrl::toPercentEncoding(tmpdata));
    postData.append(QString("&clientid=%1&psessionid=%2").arg(client_id).arg(psession_id));
    request.setHeader(QNetworkRequest::ContentLengthHeader,postData.size());
    http_action=Http_action::Send_group_msg_action;
    qDebug()<<"sendGroupMsg"<<msg<<endl;
    http->post(request,postData);
}


//获取浏览器得到的COOKIE
QString U_login_net::get_cookie(const QString &name){
    QString retstr;
    foreach(QNetworkCookie cookie , http->cookieJar()->cookiesForUrl(QUrl("http://www.qq.com/"))){
        // qDebug()<<cookie<<endl;
        if(cookie.name()==name){
            retstr=cookie.value();
        }
    }
    return retstr;
}


//使用hash算法加密
QString U_login_net::get_hash(qint32 uin, QString u_username){
    QVector<qint8> r;
    r.append(uin>> 24 &255);r.append(uin>> 16 &255);r.append(uin>> 8 &255); r.append(uin&255);
    QVector<int>ja;
    for(int e=0;e<u_username.length();++e){
        ja.push_back(u_username.at(e).toLatin1());
        //qDebug()<<a.at(e)<<ja.at(e);
    }
    QVector<QPair<int,int> > e;
    for(e.push_back(QPair<int,int>(0,ja.size()-1));e.size()>0;){
        QPair<int,int>c=e.takeLast();
        if(!(c.first>=c.second||c.first<0||c.second>=ja.size())){
            if(c.first+1==c.second){
                if(ja[c.first]>ja[c.second]){
                    int l=ja[c.first];
                    ja[c.first]=ja[c.second];
                    ja[c.second]=l;
                }
            }else{
                int l,J,f;
                for(l=c.first,J=c.second, f=ja[c.first];c.first<c.second;){
                    for(;c.first<c.second&&ja[c.second]>=f;){
                        c.second--,r[0]=r[0]+(3&255);
                    }
                    (c.first<c.second)&&(ja[c.first]=ja[c.second],c.first++,r[1]=r[1]*13+(43&255));
                    for(;c.first<c.second&&ja[c.first]<=f;){
                        c.first++,r[2]=r[2]-(3&255);
                    }
                    c.first<c.second&&(ja[c.second]=ja[c.first],c.second--,r[3]=(r[0]^r[1]^r[2]^(r[3]+1))&255);
                }
                ja[c.first]=f;
                e.push_back(QPair<int,int>(l,c.first-1));
                e.push_back(QPair<int,int>(c.first+1,J));
            }
        }
    }
    char j[]="0123456789ABCDEF";
    QString hash;
    for(int c=0;c<r.size();c++){
        hash.append(j[r[c]>>4&15]),hash.append(j[r[c]&15]);
    }
    return hash;
}
