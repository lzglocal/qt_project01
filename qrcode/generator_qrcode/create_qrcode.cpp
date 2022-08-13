#include "create_qrcode.h"
#include "ui_create_qrcode.h"
#include "u_gif/gifwidget.h"
// Function prototypes
//static void doBasicDemo();
#include <QProcess>
#include <QDesktopServices>
#include "QZXing.h"
#include <QMessageBox>


Create_qrcode::Create_qrcode(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Create_qrcode)
{
    ui->setupUi(this);
}

Create_qrcode::~Create_qrcode()
{
    delete ui;
}


/*---- Demo suite ----*/

// Creates a single QR Code, then prints it to the console.
void Create_qrcode::doBasicDemo(bool is_copy) {

    const char *text = (ui->qrcode_texEdit->toPlainText()).toLocal8Bit().data();              // User-supplied text
    const QrCode::Ecc errCorLvl = QrCode::Ecc::LOW;  // Error correction level

    // Make and print the QR Code symbol
    const QrCode qr = QrCode::encodeText(text, errCorLvl);
//    printQr(qr);

    QImage QrCode_Image=QImage(qr.getSize(),qr.getSize(),QImage::Format_RGB888);


    for (int y = 0; y < qr.getSize(); y++) {
        for (int x =0; x < qr.getSize(); x++) {
            if(qr.getModule(x, y)==0){
                QrCode_Image.setPixel(x,y,qRgb(255,255,255));
            }else{
                QrCode_Image.setPixel(x,y,qRgb(0,0,0));
            }
        }
    }

    //图像大小转换为适当的大小
        QrCode_Image=QrCode_Image.scaled(ui->label->width(),ui->label->height(),
                        Qt::KeepAspectRatio);
        //转换为QPixmap在Label中显示
        ui->label->setPixmap(QPixmap::fromImage(QrCode_Image));

        if(is_copy){
            QClipboard *clip=QApplication::clipboard();
            clip->setPixmap(QPixmap::fromImage(QrCode_Image));
        }
}


//// Creates a variety of QR Codes that exercise different features of the library, and prints each one to the console.
//void Create_qrcode::doVarietyDemo() {
//    // Numeric mode encoding (3.33 bits per digit)
//    const QrCode qr0 = QrCode::encodeText("314159265358979323846264338327950288419716939937510", QrCode::Ecc::MEDIUM);
//    printQr(qr0);

//    // Alphanumeric mode encoding (5.5 bits per character)
//    const QrCode qr1 = QrCode::encodeText("DOLLAR-AMOUNT:$39.87 PERCENTAGE:100.00% OPERATIONS:+-*/", QrCode::Ecc::HIGH);
//    printQr(qr1);

//    // Unicode text as UTF-8
//    const QrCode qr2 = QrCode::encodeText("\xE3\x81\x93\xE3\x82\x93\xE3\x81\xAB\xE3\x81\xA1wa\xE3\x80\x81"
//        "\xE4\xB8\x96\xE7\x95\x8C\xEF\xBC\x81\x20\xCE\xB1\xCE\xB2\xCE\xB3\xCE\xB4", QrCode::Ecc::QUARTILE);
//    printQr(qr2);

//    // Moderately large QR Code using longer text (from Lewis Carroll's Alice in Wonderland)
//    const QrCode qr3 = QrCode::encodeText(
//        "Alice was beginning to get very tired of sitting by her sister on the bank, "
//        "and of having nothing to do: once or twice she had peeped into the book her sister was reading, "
//        "but it had no pictures or conversations in it, 'and what is the use of a book,' thought Alice "
//        "'without pictures or conversations?' So she was considering in her own mind (as well as she could, "
//        "for the hot day made her feel very sleepy and stupid), whether the pleasure of making a "
//        "daisy-chain would be worth the trouble of getting up and picking the daisies, when suddenly "
//        "a White Rabbit with pink eyes ran close by her.", QrCode::Ecc::HIGH);
//    printQr(qr3);
//}


//// Creates QR Codes with manually specified segments for better compactness.
//void Create_qrcode::doSegmentDemo() {
//    // Illustration "silver"
//    const char *silver0 = "THE SQUARE ROOT OF 2 IS 1.";
//    const char *silver1 = "41421356237309504880168872420969807856967187537694807317667973799";
//    const QrCode qr0 = QrCode::encodeText(
//        (std::string(silver0) + silver1).c_str(),
//        QrCode::Ecc::LOW);
//    printQr(qr0);

//    const QrCode qr1 = QrCode::encodeSegments(
//        {QrSegment::makeAlphanumeric(silver0), QrSegment::makeNumeric(silver1)},
//        QrCode::Ecc::LOW);
//    printQr(qr1);

//    // Illustration "golden"
//    const char *golden0 = "Golden ratio \xCF\x86 = 1.";
//    const char *golden1 = "6180339887498948482045868343656381177203091798057628621354486227052604628189024497072072041893911374";
//    const char *golden2 = "......";
//    const QrCode qr2 = QrCode::encodeText(
//        (std::string(golden0) + golden1 + golden2).c_str(),
//        QrCode::Ecc::LOW);
//    printQr(qr2);

//    std::vector<uint8_t> bytes(golden0, golden0 + std::strlen(golden0));
//    const QrCode qr3 = QrCode::encodeSegments(
//        {QrSegment::makeBytes(bytes), QrSegment::makeNumeric(golden1), QrSegment::makeAlphanumeric(golden2)},
//        QrCode::Ecc::LOW);
//    printQr(qr3);

//    // Illustration "Madoka": kanji, kana, Cyrillic, full-width Latin, Greek characters
//    const char *madoka =  // Encoded in UTF-8
//        "\xE3\x80\x8C\xE9\xAD\x94\xE6\xB3\x95\xE5"
//        "\xB0\x91\xE5\xA5\xB3\xE3\x81\xBE\xE3\x81"
//        "\xA9\xE3\x81\x8B\xE2\x98\x86\xE3\x83\x9E"
//        "\xE3\x82\xAE\xE3\x82\xAB\xE3\x80\x8D\xE3"
//        "\x81\xA3\xE3\x81\xA6\xE3\x80\x81\xE3\x80"
//        "\x80\xD0\x98\xD0\x90\xD0\x98\xE3\x80\x80"
//        "\xEF\xBD\x84\xEF\xBD\x85\xEF\xBD\x93\xEF"
//        "\xBD\x95\xE3\x80\x80\xCE\xBA\xCE\xB1\xEF"
//        "\xBC\x9F";
//    const QrCode qr4 = QrCode::encodeText(madoka, QrCode::Ecc::LOW);
//    printQr(qr4);

//    const std::vector<int> kanjiChars{  // Kanji mode encoding (13 bits per character)
//        0x0035, 0x1002, 0x0FC0, 0x0AED, 0x0AD7,
//        0x015C, 0x0147, 0x0129, 0x0059, 0x01BD,
//        0x018D, 0x018A, 0x0036, 0x0141, 0x0144,
//        0x0001, 0x0000, 0x0249, 0x0240, 0x0249,
//        0x0000, 0x0104, 0x0105, 0x0113, 0x0115,
//        0x0000, 0x0208, 0x01FF, 0x0008,
//    };
//    qrcodegen::BitBuffer bb;
//    for (int c : kanjiChars)
//        bb.appendBits(static_cast<std::uint32_t>(c), 13);
//    const QrCode qr5 = QrCode::encodeSegments(
//        {QrSegment(QrSegment::Mode::KANJI, static_cast<int>(kanjiChars.size()), bb)},
//        QrCode::Ecc::LOW);
//    printQr(qr5);
//}


//// Creates QR Codes with the same size and contents but different mask patterns.
//void Create_qrcode::doMaskDemo() {
//    // Project Nayuki URL
//    std::vector<QrSegment> segs0 = QrSegment::makeSegments("https://www.nayuki.io/");
//    printQr(QrCode::encodeSegments(segs0, QrCode::Ecc::HIGH, QrCode::MIN_VERSION, QrCode::MAX_VERSION, -1, true));  // Automatic mask
//    printQr(QrCode::encodeSegments(segs0, QrCode::Ecc::HIGH, QrCode::MIN_VERSION, QrCode::MAX_VERSION, 3, true));  // Force mask 3

//    // Chinese text as UTF-8
//    std::vector<QrSegment> segs1 = QrSegment::makeSegments(
//        "\xE7\xB6\xAD\xE5\x9F\xBA\xE7\x99\xBE\xE7\xA7\x91\xEF\xBC\x88\x57\x69\x6B\x69\x70"
//        "\x65\x64\x69\x61\xEF\xBC\x8C\xE8\x81\x86\xE8\x81\xBD\x69\x2F\xCB\x8C\x77\xC9\xAA"
//        "\x6B\xE1\xB5\xBB\xCB\x88\x70\x69\xCB\x90\x64\x69\x2E\xC9\x99\x2F\xEF\xBC\x89\xE6"
//        "\x98\xAF\xE4\xB8\x80\xE5\x80\x8B\xE8\x87\xAA\xE7\x94\xB1\xE5\x85\xA7\xE5\xAE\xB9"
//        "\xE3\x80\x81\xE5\x85\xAC\xE9\x96\x8B\xE7\xB7\xA8\xE8\xBC\xAF\xE4\xB8\x94\xE5\xA4"
//        "\x9A\xE8\xAA\x9E\xE8\xA8\x80\xE7\x9A\x84\xE7\xB6\xB2\xE8\xB7\xAF\xE7\x99\xBE\xE7"
//        "\xA7\x91\xE5\x85\xA8\xE6\x9B\xB8\xE5\x8D\x94\xE4\xBD\x9C\xE8\xA8\x88\xE7\x95\xAB");
//    printQr(QrCode::encodeSegments(segs1, QrCode::Ecc::MEDIUM, QrCode::MIN_VERSION, QrCode::MAX_VERSION, 0, true));  // Force mask 0
//    printQr(QrCode::encodeSegments(segs1, QrCode::Ecc::MEDIUM, QrCode::MIN_VERSION, QrCode::MAX_VERSION, 1, true));  // Force mask 1
//    printQr(QrCode::encodeSegments(segs1, QrCode::Ecc::MEDIUM, QrCode::MIN_VERSION, QrCode::MAX_VERSION, 5, true));  // Force mask 5
//    printQr(QrCode::encodeSegments(segs1, QrCode::Ecc::MEDIUM, QrCode::MIN_VERSION, QrCode::MAX_VERSION, 7, true));  // Force mask 7
//}



///*---- Utilities ----*/

//// Returns a string of SVG code for an image depicting the given QR Code, with the given number
//// of border modules. The string always uses Unix newlines (\n), regardless of the platform.
//QString Create_qrcode::toSvgString(const QrCode &qr, int border) {
//    if (border < 0)
//        throw std::domain_error("Border must be non-negative");
//    if (border > INT_MAX / 2 || border * 2 > INT_MAX - qr.getSize())
//        throw std::overflow_error("Border too large");

//    QString sb;
//    sb + "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
//    sb + "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
//    sb + "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"0 0 ";
//    sb + (qr.getSize() + border * 2) + " " + (qr.getSize() + border * 2) + "\" stroke=\"none\">\n";
//    sb + "\t<rect width=\"100%\" height=\"100%\" fill=\"#FFFFFF\"/>\n";
//    sb + "\t<path d=\"";
//    for (int y = 0; y < qr.getSize(); y++) {
//        for (int x = 0; x < qr.getSize(); x++) {
//            if (qr.getModule(x, y)) {
//                if (x != 0 || y != 0)
//                    sb + " ";
//                sb + "M" + (x + border) + "," + (y + border) + "h1v1h-1z";
//            }
//        }
//    }
//    sb + "\" fill=\"#000000\"/>\n";
//    sb + "</svg>\n";
//    return sb;
//}


//// Prints the given QrCode object to the console.
//void Create_qrcode::printQr(const QrCode &qr) {

//    QImage QrCode_Image=QImage(qr.getSize(),qr.getSize(),QImage::Format_RGB888);
//    for (int y = 0; y < qr.getSize(); y++) {
//        for (int x = 0; x < qr.getSize(); x++) {
//            if(qr.getModule(x, y)==0){
//                QrCode_Image.setPixel(x,y,qRgb(255,255,255));
//            }else{
//                QrCode_Image.setPixel(x,y,qRgb(0,0,0));
//            }
//        }
//        //图像大小转换为适当的大小
//        QrCode_Image=QrCode_Image.scaled(ui->label->width(),ui->label->height(),
//                                         Qt::KeepAspectRatio);
//        //转换为QPixmap在Label中显示
//        ui->label->setPixmap(QPixmap::fromImage(QrCode_Image));
//    }
//    qDebug()<<toSvgString(qr, 4);
//}

//生成二维码按钮
void Create_qrcode::on_generator_qrcode_pbtn_clicked()
{
    doBasicDemo();
}

//复制二维码按钮
void Create_qrcode::on_copy_pic_pbtn_clicked()
{
     doBasicDemo(true);
}

//二维码扫描
void Create_qrcode::on_qrcode_scan_pbtn_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,"file","./","any files(*.*)"";;any files(*.png)");
    QImage img;
    if(!(img.load(filename))){
            return;
    }
    img = img.scaled(ui->label->width(),ui->label->height(),Qt::KeepAspectRatio);
    ui->label->setPixmap(QPixmap::fromImage(img));

//    QImage imageToDecode(filename);
    QZXing decoder;
    decoder.setDecoder( QZXing::DecoderFormat_QR_CODE | QZXing::DecoderFormat_EAN_13 );
    decoder.setSourceFilterType(QZXing::SourceFilter_ImageNormal);
    decoder.setTryHarderBehaviour(QZXing::TryHarderBehaviour_ThoroughScanning | QZXing::TryHarderBehaviour_Rotate);


//    decoder.setDecoder( QZXing::DecoderFormat_CODE_128 | QZXing::DecoderFormat_QR_CODE);
    QString result = decoder.decodeImage(img);
    if(result.isEmpty()){
        QMessageBox::about(this,"warning","Wrong picture format!");
        return;
    }
    ui->qrcode_texEdit->setText(result);
}


void Create_qrcode::on_actionluzhi_triggered()
{
//    QProcess pPrecess = QProcess(this);
//    pPrecess.start("./screenshot/screencapture.exe");
      QDesktopServices::openUrl(QUrl("file:///"+QCoreApplication::applicationDirPath()+"/screenshot/screencapture.exe"));
}


void Create_qrcode::on_actionjei_triggered()
{
    this->showMinimized();
    QProcess *pPrecess = new QProcess(new GifWidget());
    pPrecess->start();
    delete pPrecess;
}

//生成二维码
void Create_qrcode::on_actionshengc_triggered()
{
     doBasicDemo(true);
}

