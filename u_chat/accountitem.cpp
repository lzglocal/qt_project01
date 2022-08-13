#include "accountitem.h"
#include "qevent.h"

AccountItem::AccountItem(QWidget *parent)
    : QWidget{parent}
{
    m_mousePress = false ;

    //头像
    m_Icon = new QLabel() ;
    m_Icon->setMinimumSize(24,24);

    //用户名
    m_accountNumber = new QLabel() ;
    QFont font;
    font.setPointSize(11);
    m_accountNumber->setFont(font);


    //删除
    m_deleteButton = new QToolButton();
    m_deleteButton->setIcon(QIcon(":/images/delete.png"));
    m_deleteButton->setMinimumSize(24,24);
    m_deleteButton->setStyleSheet("background-color:transparent;");
    connect(m_deleteButton , &QToolButton::clicked , this , &AccountItem::onRemoveAccount);


    // 布局;
    QHBoxLayout *mainLayout = new QHBoxLayout(this) ;
    mainLayout->addWidget(m_Icon);
    mainLayout->addWidget(m_accountNumber) ;
    mainLayout->addStretch();
    mainLayout->addWidget(m_deleteButton);
    mainLayout->setContentsMargins(0, 0 , 2 , 0);
    mainLayout->setSpacing(6);
}

AccountItem::~AccountItem(){
    delete  m_accountNumber;
    delete m_deleteButton;
    delete m_Icon;
}

void AccountItem::setAccountInfo(int index , QString accountName, QString images_path){
    m_index = index;
    m_accountNumber->setText(accountName);
    m_Icon->setPixmap(QPixmap(images_path));
}



QString AccountItem::getAccountName(){
    return m_accountNumber->text() ;
}



void AccountItem::onRemoveAccount()
{
    emit signalRemoveAccount(m_index,m_accountNumber->text());
}


void AccountItem::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        m_mousePress = true;
    }
}

void AccountItem::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_mousePress){
        emit signalShowAccountInfo(m_index , m_accountNumber->text());
        m_mousePress = false;
    }
}
