#pragma execution_character_set("utf-8")

#include "gifwidget.h"
#include "ui_gifwidget.h"


GifWidget::GifWidget(QWidget *parent) : QDialog(parent), ui(new Ui::GifWidget)
{
    ui->setupUi(this);
    this->initControl();
    this->initForm();
    this->show();
}

GifWidget::~GifWidget()
{
    delete ui;
}

bool GifWidget::eventFilter(QObject *watched, QEvent *event)
{
    static QPoint mousePoint;
    static bool mousePressed = false;

    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    if (mouseEvent->type() == QEvent::MouseButtonPress) {
        if (mouseEvent->button() == Qt::LeftButton) {
            mousePressed = true;
            mousePoint = mouseEvent->globalPos() - this->pos();
            return true;
        }
    } else if (mouseEvent->type() == QEvent::MouseButtonRelease) {
        mousePressed = false;
        return true;
    } else if (mouseEvent->type() == QEvent::MouseMove) {
        if (mousePressed && (mouseEvent->buttons() && Qt::LeftButton)) {
            this->move(mouseEvent->globalPos() - mousePoint);
            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}

void GifWidget::resizeEvent(QResizeEvent *e)
{
    //拉动右下角改变大小自动赋值
    ui->txtWidth->setText(QString::number(ui->widgetMain->width()));
    ui->txtHeight->setText(QString::number(ui->widgetMain->height()));
    QDialog::resizeEvent(e);
}

void GifWidget::paintEvent(QPaintEvent *)
{
    int width = ui->txtWidth->text().toInt();
    int height = ui->txtHeight->text().toInt();
    rectGif = QRect(borderWidth, ui->widgetTop->height(), width - (borderWidth * 2), height);

    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(bgColor);
    painter.drawRoundedRect(this->rect(), 5, 5);
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.fillRect(rectGif, Qt::SolidPattern);
}

int GifWidget::getBorderWidth() const
{
    return this->borderWidth;
}

QColor GifWidget::getBgColor() const
{
    return this->bgColor;
}

void GifWidget::initControl()
{

    connect(ui->btnStart, SIGNAL(clicked(bool)), this, SLOT(record()));
    connect(ui->btnClose, SIGNAL(clicked(bool)), this, SLOT(closeAll()));
    connect(ui->txtWidth, SIGNAL(editingFinished()), this, SLOT(resizeForm()));
    connect(ui->txtHeight, SIGNAL(editingFinished()), this, SLOT(resizeForm()));
}

void GifWidget::initForm()
{
    borderWidth = 3;
    bgColor = QColor(34, 163, 169);

    fps = 10;
    ui->txtFps->setText(QString::number(fps));
    gifWriter = 0;

    timer = new QTimer(this);
    timer->setInterval(100);
    connect(timer, SIGNAL(timeout()), this, SLOT(saveImage()));

    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint | Qt::WindowStaysOnTopHint);
    this->installEventFilter(this);

//    ui->btnIcon->setIcon(style()->standardIcon(QStyle::SP_ComputerIcon));
    ui->btnClose->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));

    QStringList qss;
    qss.append("QLabel{color:#ffffff;}");
    qss.append("#btnClose,#btnIcon{border:none;border-radius:0px;}");
    qss.append("#btnClose:hover{background-color:#ff0000;}");
    qss.append("#btnClose{border-top-right-radius:5px;}");
    qss.append("#labTitle{font:bold 16px;}");
    qss.append("#labStatus{font:15px;}");
    this->setStyleSheet(qss.join(""));
}

void GifWidget::saveImage()
{
    if (!gifWriter) {
        return;
    }

#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    //由于qt4没有RGBA8888,采用最接近RGBA8888的是ARGB32,颜色会有点偏差
    QPixmap pix = QPixmap::grabWindow(0, x() + rectGif.x(), y() + rectGif.y(), rectGif.width(), rectGif.height());
    QImage image = pix.toImage().convertToFormat(QImage::Format_ARGB32);
#else
    QScreen *screen = QApplication::primaryScreen();
    QPixmap pix = screen->grabWindow(0, x() + rectGif.x(), y() + rectGif.y(), rectGif.width(), rectGif.height());
    QImage image = pix.toImage().convertToFormat(QImage::Format_RGBA8888);
#endif

    gif.GifWriteFrame(gifWriter, image.bits(), rectGif.width(), rectGif.height(), fps);
}

void GifWidget::record()
{
    if (ui->btnStart->text() == "开始") {
        if (0 != gifWriter) {
            delete gifWriter;
            gifWriter = 0;
        }

        int width = ui->txtWidth->text().toInt();
        int height = ui->txtHeight->text().toInt();
        fps = ui->txtFps->text().toInt();
        fileName = qApp->applicationDirPath() + "/" + QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss.gif");

        gifWriter = new Gif::GifWriter;
        bool bOk = gif.GifBegin(gifWriter, fileName.toLocal8Bit().data(), width, height, fps);
        if (!bOk) {
            delete gifWriter;
            gifWriter = 0;
            return;
        }

        saveImage();
        timer->start(1000 / fps);
//        ui->labStatus->setText("开始录制...");
        ui->btnStart->setText("Stop");
    } else {
        timer->stop();
        gif.GifEnd(gifWriter);

        delete gifWriter;
        gifWriter = 0;
        QDesktopServices::openUrl(QUrl(fileName));
        QFile file = QFile(fileName);
        this->close();

        QMessageBox box(QMessageBox::Question, "saving", "是否保存gif文件？");
        box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        box.setWindowIcon(QPixmap(":/images/logo.ico"));
          if(box.exec() == QMessageBox::Yes){
              return;
          }else{
              file.remove();
          }
//        ui->labStatus->setText("录制完成：" + fileName);
//        ui->btnStart->setText("");
//        ui->btnStart->setDisabled(true);
    }
}

void GifWidget::closeAll()
{
    if (0 != gifWriter) {
        delete gifWriter;
        gifWriter = 0;
    }
    this->close();
}

void GifWidget::resizeForm()
{
    int width = ui->txtWidth->text().toInt();
    int height = ui->txtHeight->text().toInt();
    this->resize(width, height + ui->widgetTop->height() + ui->widgetBottom->height());
}

void GifWidget::setBorderWidth(int borderWidth)
{
    if (this->borderWidth != borderWidth) {
        this->borderWidth = borderWidth;
        this->update();
    }
}

void GifWidget::setBgColor(const QColor &bgColor)
{
    if (this->bgColor != bgColor) {
        this->bgColor = bgColor;
        this->update();
    }
}
