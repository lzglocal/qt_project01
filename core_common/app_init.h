#ifndef APP_INIT_H
#define APP_INIT_H

#include <QObject>

class App_init : public QObject
{
    Q_OBJECT
public:
    explicit App_init(QObject *parent = nullptr);
    static App_init *Instance();


protected:
    //设置过滤事件
    bool eventFilter(QObject *watched, QEvent *event);

private:
    static QScopedPointer<App_init> self;

public slots:
    void start();
signals:

};

#endif // APP_INIT_H
