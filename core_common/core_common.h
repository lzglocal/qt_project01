#ifndef CORE_COMMON_H
#define CORE_COMMON_H

#include "aes.h"
#include "QtWidgets/qdialog.h"
#include "QtWidgets/qwidget.h"
#include "qobjectdefs.h"
#include <QPainter>
#include <QStyle>
#include <QDateTime>
#include <QScreen>
#include "qcoreapplication.h"
#include <QTranslator>
#include <QDebug>
#include <QMessageBox>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QNetworkReply>
#include <QImage>
#include <QFileInfo>
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QCryptographicHash>
#include <QStandardPaths>
#include <QDesktopServices>   //直接点链接跳转到浏览器
#include <QUrl>
#include <QTimer>
#include <QMutex>
#include <QMutexLocker>
#include <QList>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QCloseEvent>
#include <QNetworkInterface>
#include <QCryptographicHash>
#include <QSplashScreen>
#include <QThread>
#include <QEventLoop>
#include <QRandomGenerator>  //生成随机数
#include <QMovie>   //动画


//枚举请求网络一些动作
enum class Http_action{no_action,get_veriry_code_action,get_verify_code_img_action,login_action,logout_action,check_sigaction,login2_action,
                      get_friend_info_action,get_user_friends_action,get_groupname_list_action,get_msg_action,
                     send_buddymsg_action,Send_group_msg_action};

//语言选择
enum  class U_language{chinese,english,Filipino,Malay};

//登录状态
enum class U_status{offline,online};

QT_BEGIN_NAMESPACE
namespace Ui { class Core_common; }
QT_END_NAMESPACE

class Core_common : public QObject
{
    Q_OBJECT

public:
    Core_common(QWidget *parent = nullptr);
    ~Core_common();
    //获取语言切换
    void get_user_language(int language);
    //登录状态
    U_status u_status;

     //获取本地网卡IP集合(MAC地址)
    static QStringList getLocalIPs();

    //获取公网地址Public ip address
    static QString getPublicIP();

    //使用SHA256进行加密 参数: 加密字字符串
    static QString get_cryptographic_hash(const QString);

     //随机组合字符串4位
    QString get_random_str();

    //获取当前时间 返回格式yyyy-MM-dd_hh:mm:ss
    QString get_current_time();


    //自定义加密
    QString get_base64_encode(QString);
    //自定义解密
    QString get_base64_decode(QString);


private:
    Ui::Core_common *ui;
    QTranslator  *language_translator_en;
    QTranslator  *language_translator_ph;
    QTranslator  *language_translator_ms;

    //使用自定义base64加密字符串
    QString keyStr = "Sjbsnta7BFdW4u2hZRmpIExVPeNDfo9yiJH8Gq3v=gQkTz/MYXLlc1OCKw+6Ar50U";



//---------------AES加密算法-------------------------
public:
    //AES密钥初始化
    void initialize_private_key(ulong key_size=16,uchar *key_bytes=(unsigned char*)"\x6B\x77\x66\x6E\x68\x72\x65\x79\x76\x68\x73\x6B\x70\x6E\x63\x6C");

    //AES加密数据
    ulong on_aes_encrypt(void* in_Buffer,ulong in_length,void* out_Buffer);

    //AES解密数据
    ulong on_aes_decrypt(void* in_Buffer,ulong in_length,void* out_Buffer);
    uint extra_bytes;
    uint cipher_text_length;


    //AES加密字符串到写到文件
    void str_to_file_encryptor(QString str_data,QString out_file_path);
    //AES从加密文件解密返回字符串
    QString file_to_str_decryptor(QString in_file_path);

private:
    Aes * m_lp_aes=NULL;

    //打开指定文件
    QByteArray open_File(QString fileName);
    //将内容data写入到指定的文件内
    void write_File(QString fileName,QByteArray data);

};
#endif // CORE_COMMON_H
