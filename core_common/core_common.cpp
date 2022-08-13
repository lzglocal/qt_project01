#include "core_common.h"



Core_common::Core_common(QWidget *parent)
    : QObject(parent)
{
        language_translator_en = new QTranslator(this);
        language_translator_ph = new QTranslator(this);
        language_translator_ms = new QTranslator(this);
        language_translator_en->load(":/translations_en.qm");
        language_translator_ph->load(":/translations_ph.qm");
        language_translator_ms->load(":/translations_ms.qm");
}

Core_common::~Core_common()
{   //删除AES加密算偏移
    if (m_lp_aes!=NULL){
        delete m_lp_aes;
    }
    delete ui;
}

void Core_common::get_user_language(int index){
       //    ui->language_translation->currentText();
//        qDebug()<<int(index);
        switch (index) {
            case int(U_language::chinese):
//                u_language =U_language::chinese;
                //卸载英文翻译器（恢复默认中文）
                qApp->removeTranslator(language_translator_en);
                qApp->removeTranslator(language_translator_ph);
                qApp->removeTranslator(language_translator_ms);
            break;

            case int(U_language::english):
    //            u_language = U_language::english;
                //为应用安装英文翻译器
                qApp->removeTranslator(language_translator_ph);
                qApp->removeTranslator(language_translator_ms);
                qApp->installTranslator(language_translator_en);
            break;

            case int(U_language::Filipino):
                //为应用安装菲律宾语翻译器
    //            u_language = U_language::Filipino;
                qApp->removeTranslator(language_translator_en);
                qApp->removeTranslator(language_translator_ms);
                qApp->installTranslator(language_translator_ph);
                break;

            case int(U_language::Malay):
    //            u_language = U_language::Malay;
                qApp->removeTranslator(language_translator_en);
                qApp->removeTranslator(language_translator_ph);
                qApp->installTranslator(language_translator_ms);
                //重新翻译界面英文本字符串
                break;

            default:
                qDebug("Not found This Language");
                break;
        }
}



//获取本地ip地址和Mac地址，返回QstringLst 第一个为ip 第二个为Mac地址
QStringList Core_common::getLocalIPs()
{
    static QStringList ips;
    if (ips.count() == 0) {
#ifdef Q_OS_WASM
        ips << "127.0.0.1";
#else
        QList<QNetworkInterface> netInterfaces = QNetworkInterface::allInterfaces();
        foreach (const QNetworkInterface  &netInterface, netInterfaces) {
            //移除虚拟机和抓包工具的虚拟网卡
            QString humanReadableName = netInterface.humanReadableName().toLower();
            if (humanReadableName.startsWith("vmware network adapter") || humanReadableName.startsWith("npcap loopback adapter")) {
                continue;
            }

            //过滤当前网络接口
            bool flag = (netInterface.flags() == (QNetworkInterface::IsUp | QNetworkInterface::IsRunning | QNetworkInterface::CanBroadcast | QNetworkInterface::CanMulticast));
            if (!flag) {
                continue;
            }

            QList<QNetworkAddressEntry> addrs = netInterface.addressEntries();
            foreach (QNetworkAddressEntry addr, addrs) {
                //只取出IPV4的地址
                if (addr.ip().protocol() != QAbstractSocket::IPv4Protocol) {
                    continue;
                }
                QString ip4 = addr.ip().toString().remove("("&&")");
                if (ip4 != "127.0.0.1") {
                    ips << ip4;
                    ips<<netInterface.hardwareAddress();
                }
            }
        }
#endif
    }
    return ips;
}

//获取公网地址Public ip address
QString Core_common:: getPublicIP(){
    QNetworkAccessManager http;
    QByteArray response_data;
    QEventLoop event_loop;
    QNetworkReply *reply_data;

    QNetworkRequest request;
    //QUrl url = QUrl("https://httpbin.org/get");
     QUrl url = QUrl("https://ifconfig.me/all.json");

    request.setUrl(url);
//    request.setRawHeader("Content-Type","application/json");//服务器要求的数据头部
//    request.setRawHeader("Accept","text/json,*/*;q=0.5");//服务器要求的数据头部
    request.setRawHeader("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9");
    request.setRawHeader("Accept-Charset","GBK,utf-8;q=0.7,*;q=0.3");
    request.setRawHeader("Accept-Encoding","gzip, deflate, br");
    request.setRawHeader("Accept-Language","zh-CN,zh;q=0.9,en-US;q=0.8,en;q=0.7");
//    request.setRawHeader("Cache-Control","max-age=0");
//    request.setRawHeader("Connection","keep-alive");
//    request.setRawHeader("Host", "httpbin.org");
    request.setRawHeader("User-Agent","Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/29.0.1547.62 Safari/537.36");

    connect(&http , &QNetworkAccessManager::finished, &event_loop, &QEventLoop::quit);
    reply_data = http.get(request);
    event_loop.exec();
    response_data = reply_data->readAll();
//    qDebug()<<response_data;


    //QJsonParseError 类用于在 JSON 解析中报告错误
    QJsonParseError json_error;

    /*QJsonDocument 类用于读和写 JSON 文档从基于文本的表示转化为 QJsonDocument， toJson() 则可以反向转化为文本。
     * 解析器非常快且高效，并将 JSON 转换为 Qt 使用的二进制表示。
    已解析文档的有效性，可以使用 !isNull() 进行查询*/
    QJsonDocument doucment = QJsonDocument::fromJson(response_data, &json_error);
//    qDebug()<<doucment;
    //判断如果未发生错误
    if (!doucment.isNull() && (json_error.error == QJsonParseError::NoError)) {
        //是否包含一个数组或一个对象，使用 isArray() 和 isObject()
        if (doucment.isObject()) {  // JSON 文档为对象
            const QJsonObject object = doucment.object(); // 转化为对象
//             qDebug()<<object;

//            if (object.contains("origin")) { // 包含指定的 key吗
//                QJsonValue value = object.value("origin");  // 获取指定 key 对应的 value
//                if (value.isString()) {  // 判断 value 是否为字符串
//                    QString str_data = value.toString();  // 将 value 转化为字符串
////                    qDebug() << "Public ip address: " << str_data;
//                    reply_data->deleteLater();
//                    return str_data;

//                }else if(value.isArray()){
//                    QJsonArray array = value.toArray();
//                    int nSize = array.size();
//                    for (int i = 0; i < nSize; ++i) {
//                        QJsonValue value = array.at(i);
//                        if (value.isDouble()) {
//                            double dVersion = value.toDouble();
//                            qDebug() << "Version : " << dVersion;
//                        }
//                    }
//                }else if(value.isDouble()){
//                    double dVersion = value.toDouble();
//                    qDebug() << dVersion;

//                }else if(value.type() == QJsonValue::Bool){
//                    bool bCrossPlatform  = value.toBool();
//                    qDebug() << bCrossPlatform;
//                }
//            }

            if (object.contains("ip_addr")) { // 包含指定的 key吗
                QJsonValue value = object.value("ip_addr");  // 获取指定 key 对应的 value
//                qDebug()<<value;
                if (value.isString()) {  // 判断 value 是否为字符串
                    QString str_data = value.toString();  // 将 value 转化为字符串
//                    qDebug() << "Public ip address: " << str_data;
                    reply_data->deleteLater();
                    return str_data;
                }
            }


            //获取另外一个key
//            else if (object.contains("Cross Platform")) {
//                QJsonValue value = object.value("Cross Platform");
//                if (value.isBool()) {
//                    bool bCrossPlatform = value.toBool();
//                    qDebug() << "CrossPlatform : " << bCrossPlatform;
//                }
//            }




        }
    }
//    qDebug()<<"\nerror";
    return "";
}


QString Core_common::get_current_time(){
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy-MM-dd_hh:mm:ss");
    return current_date;
}


//使用sha256对字符串进行加密
QString Core_common::get_cryptographic_hash(const QString cryptoString){
    static QString crypto_to_string;
    //[static] QByteArray QCryptographicHash::hash(const QByteArray &data, QCryptographicHash::Algorithm method)
    crypto_to_string =QCryptographicHash::hash(cryptoString.toLocal8Bit(),QCryptographicHash::Sha256);
    return crypto_to_string;
}


//生成随机字符串4位
QString Core_common::get_random_str(){
    char str[] = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QString r_str;
    int i=0;
    while(i<4){
        r_str+=str[QRandomGenerator::global()->bounded(1, int(strlen(str)))];
        i++;
    }
    return r_str;
}


//--------------------------------自定义base64为加密--------------------------------------------
QString Core_common::get_base64_encode(QString input){
    QString output = "";
       quint32 i= 0;
       while(i <quint32(input.length())){
           quint32 chr1, chr2, chr3 ;
           quint32 enc1, enc2, enc3, enc4 ;
           chr1 = input[i++].unicode();
           chr2 = input[i++].unicode();
           chr3 = input[i<quint32(input.length())?i++:quint32(input.length())].unicode();


           enc1 = chr1 >> 2;   //右移二位
           enc2 = ((chr1 & 3) << 4) | (chr2 >> 4);
           enc3 = ((chr2 & 15) << 2) | (chr3 >> 6);
           enc4 = chr3 & 63;

           if (std::isnan(chr2)) {
               enc3 = enc4 = 64;
           } else if (std::isnan(chr3)) {
               enc4 = 64;
           }
           output = output + keyStr[enc1] +keyStr[enc2] +keyStr[enc3] +keyStr[enc4];
       }
       return output;
}

QString	Core_common::get_base64_decode(QString input){
     QString output = "";
     quint32 i = 0;

    QRegExp rx("[^A-Za-z0-9+/=]");
    // remove all characters that are not A-Z, a-z, 0-9, +, /, or =
    if (rx.exactMatch(input)) {
        return "There were invalid base64 characters in the input text.\nValid base64 characters are A-Z, a-z, 0-9, '+', '/', and '='\nExpect errors in decoding.";
    }

    input = input.replace(rx, "");
    while (i < quint32(input.length())) {
        quint32 chr1, chr2, chr3;
        quint32 enc1, enc2, enc3, enc4;
        enc1 = keyStr.indexOf(input[i++]);
        enc2 = keyStr.indexOf(input[i++]);
        enc3 = keyStr.indexOf(input[i++]);
        enc4 = keyStr.indexOf(input[i++]);

        chr1 = (enc1 << 2) | (enc2 >> 4);
        chr2 = ((enc2 & 15) << 4) | (enc3 >> 2);
        chr3 = ((enc3 & 3) << 6) | enc4;

        output = output + QString(chr1);
        if (enc3 != 64) {
            output = output +QString(chr2);
        }
        if (enc4 != 64) {
            output = output + QString(chr3);
        }
    }

    //QString中包含“\u0000“的处理方式,去掉空格
    QChar rep_str = 0x00;
    if (output.contains(rep_str))
    {
        output.replace(rep_str, "");
    }
    return output;
}



//-----------------------------AES加解密函数---------------------------------------------------
// 函数名称：initialize_private_key
// 函数描述：初始化AES 密钥，密钥用于加密解密
void Core_common::initialize_private_key(ulong key_size,uchar *key_bytes)
{
    if (m_lp_aes){
        delete m_lp_aes;
        m_lp_aes=NULL;
    }
    m_lp_aes=new Aes(key_size,key_bytes);
}

//------------------------------------------------------------------------------------------------------------
// 函数名称：on_aes_encrypt
// 函数描述：用AES加密算法加密数据
ulong Core_common::on_aes_encrypt(void* in_Buffer,ulong in_length,void* out_Buffer){
    //ulong 占4个字节
    ulong out_length=0,extra_bytes = 0;
    if (m_lp_aes==NULL||out_Buffer==NULL){
        return 0;
    }
    //InBuffer是一个char类型的数组，lpCurInBuff指针指向的是InBuffer char数组的第一个元素的地址
    uchar *lpCurInBuff=(uchar *)in_Buffer;
    //加密后的数据往OutBuffer中写入时，从第16个字节开始写。前面空出16个字节，用于存放密文大小和额外加密的字节个数，只用到8个字节，但为了保证字节总数是16的倍数，故预留了16个字节
    uchar *lpCurOutBuff=(uchar *)out_Buffer+16;
    long blocknum=in_length/16;
    long leftnum=in_length%16;

    for(long i=0;i<blocknum;i++){
        //加密时，传入的数组必须时16个字节的
        m_lp_aes->Cipher(lpCurInBuff,lpCurOutBuff);
        //每次加密16个字节，循环直至所有字节均被加密
        lpCurInBuff+=16;
        lpCurOutBuff+=16;
        out_length+=16;
    }

    //当传入的密文的字节总数不是16的倍数时，会多余出leftnum 个字节。
    //此时，需要添加16-leftnum 个字节到mingwen中。则加密得到的密文，会多出16-leftnum 个字节。
    //这16-leftnum 个字节并不是mingwen里存在的

    if(leftnum){
        uchar inbuff[16];
        memset(inbuff,'X',16);
        //经过上面的for循环，此时lpCurInBuff指针指向InBuffer数组的sizeof(InBuffer)-leftnum 位置处
        memcpy(inbuff,lpCurInBuff,leftnum);
        //此次解密，实际上是多加密了16-leftnum 个字节
        m_lp_aes->Cipher(inbuff,lpCurOutBuff);
        lpCurOutBuff+=16;
        out_length+=16;
    }

    uchar *bytesSize=(uchar *)out_Buffer;
    uchar *extraBytesSize = (uchar *)(quint64(out_Buffer)+4);

    //多加密的字节个数
    extra_bytes = (16-leftnum)%16;
    //将OutLength的地址复制给bytesSize的前4个字节。即将加密后的密文大小存放在miwen的前四个字节中
    memcpy(bytesSize,&out_length,4);
    //将多加密的字节个数存放在第4-8个字节中
    memcpy(extraBytesSize,&extra_bytes,4);

    //返回的OutLength只包括密文长度，不包括outBuffer中预留用来存放outBuffer字节个数和额外多加密的字节个数的16字节。
    //即OutLength = sizeof(OutBuffer)-16
    return out_length;
}



//--------------
// 函数名称：on_aes_decrypt
// 函数描述：用AES加密算法解密数据
// 返回数值：解密后的数据大小 ，错误返回值 0
ulong Core_common::on_aes_decrypt(void* in_Buffer,ulong in_length,void* out_Buffer)
{
    //传入的InLength大小是加密时返回的OutLength+16，即outBuffer的大小
    ulong out_Length=0,extra_bytes=0;
    if (m_lp_aes==NULL||out_Buffer==NULL){
        return 0;
    }
    //密文是从第16个字节开始的，故解密时，前16个字节忽略，直接从第16个字节开始解密  
    uchar *lpCurInBuff=(uchar *)in_Buffer+16;
    uchar *lpCurOutBuff=(uchar *)out_Buffer;
    long blocknum=in_length/16;
    long leftnum=in_length%16;

    //传入的密文大小必须是16的整数倍个字节
    if(leftnum){
        return 0;
    }
    //每次解密16个字节，循环全部解出
    for(long i=0;i<blocknum;i++)
    {
        m_lp_aes->InvCipher(lpCurInBuff,lpCurOutBuff);
        lpCurInBuff+=16;
        lpCurOutBuff+=16;
        out_Length+=16;
    }
    //ExtraBytesSize指针指向的是InBuffer的第四个字节处
    uchar* ExtraBytesSize =(uchar *) in_Buffer+4;
    //将InBuffer的第4-8个字节中的内容复制给ExtraBytes。此时ExtraBytes代表的是加密是额外加密的字节数
    memcpy(&extra_bytes,ExtraBytesSize,4);
    //将额外加密的那部分内容，即ExtraBytes个字节的内容置为0
    memset(lpCurOutBuff-extra_bytes,0,extra_bytes);
    return out_Length;
}


//打开文件，将文件中的内容返回一个QbyteArray的数组
QByteArray Core_common::open_File(QString fileName){
    QFile file(fileName);
    if(file.open(( QIODevice::ReadWrite))){
        QByteArray temp = file.read(file.bytesAvailable());
        file.close();
        return temp;
    }else{
        QMessageBox box(QMessageBox::Warning, "warnning", "文件有被篡改或拷贝的痕迹,已强制删除\nThe file was tampered with and deleted");
        box.setStandardButtons(QMessageBox::Yes);
     box.setWindowIcon(QPixmap(":/images/logo.ico"));
//       box.setButtonText(QMessageBox::Yes, QString("确 定"));
    box.exec();
    }
}

//将一个QbyteArray数组写入到指定文件中去。
//使用二进制数组进行文件的读写能够有效避免各种由于编码格式和类型转换造成的问题
void Core_common::write_File(QString fileName, QByteArray data){
    QFile file(fileName);
    file.open(( QIODevice::ReadWrite|QIODevice::Truncate));
    file.write(data);
    file.close();
    return ;
}



    //AES加密字符串到写到文件-->调用前必须初始化加密 initialize_private_key
void Core_common::str_to_file_encryptor(QString str_data,QString out_file_path){
    //设置保存对称加密的密文
    uchar user_secret_bytes[4096] = {0};

    //做对称加密
    on_aes_encrypt((void*)str_data.toLatin1().data(),str_data.length(),user_secret_bytes);

    //处理加密密文
    QByteArray miwen_data;
    ulong byte_size = 0;
    //将密文的前四个字符复制给bytesize的地址
    memcpy(&byte_size,user_secret_bytes,4);
    //qDebug()<<"bytesize:"<<byteSize;
    miwen_data.resize(byte_size+16);
    //将密文的前byteSize+16个字符复制给miwen_date
    memcpy(miwen_data.data(),user_secret_bytes,byte_size+16);

    //以数据流写入到文件中
    write_File(out_file_path,miwen_data);
}
    //AES从加密文件解密返回字符串 调用前必须初始化加密 initialize_private_key
QString Core_common::file_to_str_decryptor(QString in_file_path){
    QByteArray temp = open_File(in_file_path);


    char miwen[4096] = {0};
    char jiemi_str[4096]={0};
    //将temp字节数组中的所有数据复制给miwen char类型数组
    memcpy(miwen,temp.data(),temp.size());
    ulong byteSize = 0;
    //miwen的大小存放在miwen的前四个字节中，将miwen大小赋值给byteSize
    memcpy(&byteSize,miwen,4);
    on_aes_decrypt((char *)miwen, (ulong)byteSize,(char *)jiemi_str); //进行解密
    //解密结果Qstring字符串返回
    return QString(jiemi_str);
}

