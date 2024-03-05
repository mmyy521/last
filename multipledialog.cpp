#include "multipledialog.h"
#include<QLabel>
#include<QLineEdit>
#include<QInputDialog>
#include<QFormLayout>
#include<QDialogButtonBox>
#include<QDebug>
#include<QSettings>
#include<QPushButton>
#include<QFileDialog>
#include<QBuffer>
#include<QFile>
#include<QMessageBox>
#include<QRadioButton>
#include<QButtonGroup>

MultipleDialog::MultipleDialog(QWidget *parent):QDialog(parent)
{
    QFormLayout *lay = new QFormLayout(this);
    QButtonGroup *buttonGroup = new QButtonGroup(this);
    for(int i=0;i<6;i++)
    {
        QLabel *name;
        if(i==0)
            name =new QLabel(QString("cn"),this);
        else if(i==1)
            name =new QLabel(QString("sn"),this);
        else if(i==2)
            name =new QLabel(QString("uid"),this);
        else if(i==3)
        {
            // 创建单选按钮
            QRadioButton *radioButton100 = new QRadioButton("100(教学组)");
            QRadioButton *radioButton200 = new QRadioButton("200(行政组)");
            QRadioButton *radioButton300 = new QRadioButton("300(科研组)");
            QRadioButton *radioButton400 = new QRadioButton("400(教师组)");
            QRadioButton *radioButton500 = new QRadioButton("500(本科生组)");
            QRadioButton *radioButton600 = new QRadioButton("600(研究生组)");

            // 创建按钮组，确保只能选择一个单选按钮

            buttonGroup->addButton(radioButton100, 100);
            buttonGroup->addButton(radioButton200, 200);
            buttonGroup->addButton(radioButton300, 300);
            buttonGroup->addButton(radioButton400, 400);
            buttonGroup->addButton(radioButton500, 500);
            buttonGroup->addButton(radioButton600, 600);

            // 将单选按钮添加到表单布局
            lay->addRow("gidNumber:", radioButton100);
            lay->addRow("", radioButton200);
            lay->addRow("", radioButton300);
            lay->addRow("", radioButton400);
            lay->addRow("", radioButton500);
            lay->addRow("", radioButton600);
            buttonGroup->setExclusive(true);
            connect(buttonGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &MultipleDialog::handleRadioButtonClicked);
            continue;
        }
            //name =new QLabel(QString("telephoneNumber"),this);
        else if(i==4)
            name = new QLabel(QString("jid"),this);
        else if(i==5)
        {
            name=new QLabel(QString("userPassword"),this);
            QLineEdit *inputLine=new QLineEdit(this);
            inputLine->setEchoMode(QLineEdit::Password);
            lay->addRow(name,inputLine);
            fields.append(inputLine);
            continue;
        }
        QLineEdit *inputLine=new QLineEdit(this);
        lay->addRow(name,inputLine);
        fields.append(inputLine);
    }
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel,Qt::Horizontal,this);
    lay->addWidget(buttonBox);
    bool conn = connect(buttonBox,&QDialogButtonBox::accepted,this,&MultipleDialog::accept);
    Q_ASSERT(conn);
    conn = connect(buttonBox,&QDialogButtonBox::rejected,this,&MultipleDialog::reject);
    Q_ASSERT(conn);
    setLayout(lay);
    this->setWindowTitle("新增用户");
    this->setMinimumWidth(800);
}
QStringList MultipleDialog::getStrings()
{
    QStringList list;
    QListIterator<QLineEdit*> it2(fields);
    for(it2.toFront(); it2.hasNext();)
    {
        list.append(it2.next()->text());
    }
    return list;
}

QString MultipleDialog::getPicBuff()
{
    return pBase64;
}
int MultipleDialog::getgid()
{
    return gid;
}

void MultipleDialog::handleRadioButtonClicked(int id) {
    // 处理单选按钮点击事件，id即为对应的编号
    gid = id;
}

