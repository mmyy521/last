#include "muldialogou.h"
#include<QLabel>
//QLabel是QT界面中的标签类，它从QFrame下继承，是一个用于显示文本或图像的窗口部件。
//额卷头发的很卷。
#include<QLineEdit>
//QLineEdit是一个单行文本输入框。
//QLineEdit允许用户输入和编辑单行纯文本，提供了很多有用的编辑功能，包括：撤消和重做、剪切和粘贴、以及拖放（见setDragEnabled()）。
#include<QInputDialog>
//QInputDialog类提供了一个简单的便捷对话框，可以从用户那里获取用户录入的单个值。
//当前Qt提供了五个静态接口函数用于获取常用的数据类型参数：
//getText（…），getMultiLineText（…），getInt（…），getDouble（…）和getItem（…）
#include<QFormLayout>
//QFormLayout管理输入型控件和关联的标签组成的那些Form表单。它是一个方便的布局类，其中的控件以两列的形式被布局在表单中。
//左列包括标签，右列包含输入控件，例如：QLineEdit、QSpinBox等。
#include<QDialogButtonBox>
#include<QDebug>
#include<QSettings>
#include<QPushButton>

//serverconnect中
//增加节点时的文本输入框
MulDialogOu::MulDialogOu(QWidget *parent):QDialog(parent)
{
    QFormLayout *lay = new QFormLayout(this);       //Form表单
    QLabel *name =new QLabel(QString("ou"),this);   //标签类
    QLineEdit *inputLine=new QLineEdit(this);       //文本输入框
    QLabel *desc =new QLabel(QString("description"),this);   //标签类
    QLineEdit *inputLine_desc=new QLineEdit(this);
    lay->addRow(name,inputLine);    //在表单里增加一行包括标签和输入框
    fields.append(inputLine);       //这里的fields是QLineEdit的list？
    lay->addRow(desc,inputLine_desc);    //在表单里增加一行包括标签和输入框
    fields.append(inputLine_desc);
    QDialogButtonBox *buttonBox=new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel,Qt::Horizontal,this);
    lay->addWidget(buttonBox);  ///在表单里增加个按钮
    bool conn = connect(buttonBox,&QDialogButtonBox::accepted,this,&MulDialogOu::accept);
    //按钮作为消息源，如果是acc了就出发对应的函数
    Q_ASSERT(conn);     //为了调试中断，但并没有中断？
    conn = connect(buttonBox,&QDialogButtonBox::rejected,this,&MulDialogOu::reject);
    Q_ASSERT(conn);
    setLayout(lay);
    //用于设置QWidget中的布局管理器。它接受一个QLayout对象作为参数，并将该布局管理器应用于QWidget。
    this->setWindowTitle("新增组织");
}

QStringList MulDialogOu::getStrings()
{
    QStringList list;
    QListIterator<QLineEdit*> it2(fields);
    for(it2.toFront(); it2.hasNext();)
    {
        list.append(it2.next()->text());
    }
    return list;
}

