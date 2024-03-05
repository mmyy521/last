#include "configwindow.h"

ConfigWindow::ConfigWindow(QWidget *parent) : QDialog(parent)
{
    QLabel *iplabel =new QLabel(QString("IP"),this);
    inputLine=new QLineEdit(this);
    QFormLayout *layout = new QFormLayout;
    layout->addRow(iplabel, inputLine);
    //按钮
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel,Qt::Horizontal,this);
    layout->addWidget(buttonBox);
    bool conn = connect(buttonBox,&QDialogButtonBox::accepted,this,&ConfigWindow::accept);
    Q_ASSERT(conn);
    conn = connect(buttonBox,&QDialogButtonBox::rejected,this,&ConfigWindow::reject);
    Q_ASSERT(conn);
    setLayout(layout);
    this->setWindowTitle("配置IP地址");
    this->setMinimumSize(400,200);
}
QString ConfigWindow::getIP()
{
    return inputLine->text();
}
