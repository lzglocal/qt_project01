QT       += core gui network multimedia sql axcontainer multimedia multimediawidgets


TRANSLATIONS = translations_en.ts \
               translations_ph.ts \
               translations_ms.ts \

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17
#把所有警告都关掉眼不见为净
CONFIG += warn_off
#开启大资源支持
CONFIG += resources_big
#开启后会将打印信息用控制台输出`
#CONFIG += console
#开启后不会生成空的 debug release 目录
#CONFIG -= debug_and_release

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


#指定生成的应用程序名
TARGET      = mobile_legend

#建立一个类型为应用程序
TEMPLATE    = app

#应用程序ico
RC_ICONS = "main_logo.ico"

#指定编译产生的文件分门别类放到对应目录
#MOC_DIR     = temp/moc
#RCC_DIR     = temp/rcc
#UI_DIR      = temp/ui
#OBJECTS_DIR = temp/obj

#指定编译生成的可执行文件放到源码上一级目录下的bin目录
!android {
!wasm {
#DESTDIR = $$PWD/../bin
}}

#如果是linux或者苹果添加相关库
unix:!macx {
contains(DEFINES, arma7) {
INCLUDEPATH += /usr/local/openssl-1.0.2m-h3-gcc-4.9.2/include
LIBS += -L/usr/local/openssl-1.0.2m-h3-gcc-4.9.2/lib -lssl -lcrypto
LIBS += -L/usr/local/h3_rootfsv -lXdmcp
}}




SOURCES += \
    main.cpp \
    mainmenu.cpp \
    menu_control.cpp

HEADERS += \
    mainmenu.h \
    menu_control.h

FORMS += \
    mainmenu.ui

#头文件包含路径
INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/u_chat
INCLUDEPATH += $$PWD/core_common
INCLUDEPATH += $$PWD/u_gif

include ($$PWD/u_chat/u_chat.pri)
include ($$PWD/core_common/core_common.pri)
include ($$PWD/u_gif/u_gif.pri)

include(qrcode/QZXing/QZXing.pri)
include(qrcode/generator_qrcode/create_qrcode.pri)

#工程中包含的资源文件
RESOURCES += $$PWD/images.qrc \
    screenshot.qrc \
    translations.qrc




# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    translations_en.ts \
    translations_ph.ts \
    translations_ms.ts

