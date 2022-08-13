#ifndef CREATE_QRCODE_H
#define CREATE_QRCODE_H

#include <QMainWindow>
//#include <climits>
//#include <cstdint>
//#include <cstdlib>
#include <cstring>
//#include <iostream>
//#include <sstream>
//#include <string>
//#include <vector>
#include "qrcodegen.h"
#include <QImage>
#include <QClipboard>
#include <QFileDialog>
//#include <QDebug>

using std::uint8_t;
using qrcodegen::QrCode;
using qrcodegen::QrSegment;


QT_BEGIN_NAMESPACE
namespace Ui { class Create_qrcode; }
QT_END_NAMESPACE

class Create_qrcode : public QMainWindow
{
    Q_OBJECT

public:
    Create_qrcode(QWidget *parent = nullptr);
    ~Create_qrcode();

private slots:
    //生成二维码
    void on_generator_qrcode_pbtn_clicked();

    //复制二维码
    void on_copy_pic_pbtn_clicked();

    void on_qrcode_scan_pbtn_clicked();

    void on_actionluzhi_triggered();

    void on_actionjei_triggered();

    void on_actionshengc_triggered();

private:
    Ui::Create_qrcode *ui;
//    QString toSvgString(const QrCode &qr, int border);
//    void printQr(const QrCode &qr);

    void doBasicDemo(bool is_copy=false);
//    void doVarietyDemo();
//    void doSegmentDemo();
//    void doMaskDemo();
};
#endif // CREATE_QRCODE_H
