#ifndef ACCOUNTITEM_H
#define ACCOUNTITEM_H

#include <QWidget>
#include <QLabel>
#include <QToolButton>
#include <QHBoxLayout>

class AccountItem : public QWidget
{
    Q_OBJECT
public:
    explicit AccountItem(QWidget *parent = nullptr);
    ~AccountItem();

signals:
    void signalShowAccountInfo(int index , QString accountName);
    void signalRemoveAccount(int index,QString) ;

public:
    void setAccountInfo(int index, QString accountName, QString headFilePath);
    QString getAccountName();

public slots:
    void onRemoveAccount() ;

private:
    void mousePressEvent(QMouseEvent *event) ;
    void mouseReleaseEvent(QMouseEvent *event) ;
    QString getHeadImageDirPath();

private:
    bool m_mousePress;
    QLabel *m_accountNumber;
    QToolButton *m_deleteButton;
    QLabel *m_Icon;
    quint32 m_index;


};

#endif // ACCOUNTITEM_H
