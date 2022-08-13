#ifndef GIFWIDGET_H
#define GIFWIDGET_H

#include "core_common.h"
#include "gif.h"

namespace Ui
{
    class GifWidget;
}

#ifdef quc
#if (QT_VERSION < QT_VERSION_CHECK(5,7,0))
#include <QtDesigner/QDesignerExportWidget>
#else
#include <QtUiPlugin/QDesignerExportWidget>
#endif

class QDESIGNER_WIDGET_EXPORT GifWidget : public QDialog
#else
class GifWidget : public QDialog
#endif

{
    Q_OBJECT
//    Q_PROPERTY(int borderWidth READ getBorderWidth WRITE setBorderWidth)
//    Q_PROPERTY(QColor bgColor READ getBgColor WRITE setBgColor)

public:
    explicit GifWidget(QWidget *parent = 0);
    ~GifWidget();

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);

private:
    Ui::GifWidget *ui;

    int borderWidth;            //边框宽度
    QColor bgColor;             //背景颜色

    uint fps;                   //帧数 100为1s
    QString fileName;           //保存文件名称
    QRect rectGif;              //截屏区域
    QTimer *timer;              //截屏定时器

    Gif gif;                    //gif类对象
    Gif::GifWriter *gifWriter;  //gif写入对象

public:
    int getBorderWidth()        const;
    QColor getBgColor()         const;

private slots:
    void initControl();
    void initForm();
    void saveImage();
    void record();
    void closeAll();
    void resizeForm();

public Q_SLOTS:
    void setBorderWidth(int borderWidth);
    void setBgColor(const QColor &bgColor);
};

#endif // GIFWIDGET_H
