#include "modifydialog.h"
#include<QDebug>
#include<QFormLayout>
#include<QLabel>
#include<QPushButton>
#include<QDialogButtonBox>
#include<QImage>
#include<QFileDialog>
#include<QBuffer>
//修改？
ModifyDialog::ModifyDialog(QWidget *parent,QStringList list):QDialog(parent)
{
    QFormLayout *lay = new QFormLayout(this);
    int flag=0;
    for(int i=0;i<list.size();i++)
    {
        QStringList attrs=list.at(i).split(":");
        QLabel *attrName = new QLabel(QString(attrs[0]),this);
        QLineEdit *inputLine=new QLineEdit(this);
        if(attrs[0]=="uidNumber" || attrs[0]=="title")
            inputLine->setEnabled(false);
        inputLine->setText(attrs[1]);
        lay->addRow(attrName,inputLine);
        fields.append(inputLine);
    }

    QDialogButtonBox *buttonBox=new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel,Qt::Horizontal,this);
    lay->addWidget(buttonBox);
    bool conn = connect(buttonBox,&QDialogButtonBox::accepted,this,&ModifyDialog::accept);
    Q_ASSERT(conn);
    conn = connect(buttonBox,&QDialogButtonBox::rejected,this,&ModifyDialog::reject);
    Q_ASSERT(conn);
    setLayout(lay);
    this->setMinimumWidth(800);
    this->setWindowTitle("修改信息");
}

QStringList ModifyDialog::getStrings()
{
    QStringList list;
    QListIterator<QLineEdit*> it2(fields);
    for(it2.toFront(); it2.hasNext();)
    {
        list.append(it2.next()->text());
    }
    return list;
}

QString ModifyDialog::getPicBuff()
{
    return pBase64;
}


