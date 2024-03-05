#include "modifypwd.h"
//修改密码，这个只能在管理员用户登录时修改
ModifyPwd::ModifyPwd(QWidget *parent,bool newPwd):QDialog(parent)
{
    QFormLayout *lay = new QFormLayout(this);
    QLabel *name;
    //输入新密码面板
    if(newPwd)
    {
        name = new QLabel(QString("请输入新密码"),this);
        QLineEdit *inputLine=new QLineEdit(this);
        inputLine->setEchoMode(QLineEdit::Password);
        lay->addRow(name,inputLine);
        QLabel *confirmName=new QLabel(QString("请确认新密码"),this);
        QLineEdit *confirmPwd=new QLineEdit(this);
        confirmPwd->setEchoMode(QLineEdit::Password);
        lay->addRow(confirmName,confirmPwd);
        newField.append(inputLine);
        newField.append(confirmPwd);
    }
    else
    {
        name=new QLabel(QString("请输入当前密码"),this);
        QLineEdit *inputLine=new QLineEdit(this);
        inputLine->setEchoMode(QLineEdit::Password);
        lay->addRow(name,inputLine);
        oldField=inputLine;
    }
    QDialogButtonBox *buttonBox=new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel,Qt::Horizontal,this);
    lay->addWidget(buttonBox);
    bool conn = connect(buttonBox,&QDialogButtonBox::accepted,this,&ModifyPwd::accept);
    Q_ASSERT(conn);
    conn = connect(buttonBox,&QDialogButtonBox::rejected,this,&ModifyPwd::reject);
    Q_ASSERT(conn);
    setLayout(lay);
    this->setWindowTitle("修改密码");
}

QString ModifyPwd::getString()
{
    return oldField->text();
}

//获取两次新密码结果
QStringList ModifyPwd::getPwd()
{
    QStringList pwdList;
    QListIterator<QLineEdit*> it2(newField);
    for(it2.toFront(); it2.hasNext();)
    {
        pwdList.append(it2.next()->text());
    }
    return pwdList;
}
