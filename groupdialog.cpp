#include "groupdialog.h"
#include "ui_groupdialog.h"
#include<QDebug>
#include"ldapcatalogue.h"
#include"basetreenode.h"
#include<QVBoxLayout>
#include<QPushButton>
//在哪里用？
GroupDialog::GroupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GroupDialog)
{
    ui->setupUi(this);
    name = ui->groupName;
    connect(ui->btn,&QPushButton::clicked,this,&GroupDialog::tempFunction);
}

GroupDialog::~GroupDialog()
{
    delete ui;
}

void GroupDialog::tempFunction()
{
    getMoveCn(ip,path);
}

void GroupDialog::getMoveCn(QString host,QString basePath)
{
    QStringList  moveList;
    //设置选择树
    LDAPCatalogue *ld=new LDAPCatalogue();
    BaseTreeNode *LDAPTree = ld->GetCatalogue(host,basePath);
    QTreeWidget *showTree = new QTreeWidget;
    showTree->setMinimumHeight(800);
    showTree->setMinimumWidth(500);
    showTree->setHeaderLabels(QStringList("单位/部门"));
    QTreeWidgetItem * item = new QTreeWidgetItem(showTree,QStringList()<<LDAPTree->getName());
    item->setStatusTip(0,item->text(0));
    item->setText(0,item->text(0).split("=")[1]);
    showTree->addTopLevelItem(item);
    LDAPTree->addCheck(LDAPTree,item);
//    connect(showTree,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(treeItemChanged(QTreeWidgetItem*,int)));
    showTree->expandAll();
    //设置窗口
    QWidget *treeWidget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    QPushButton *confirmBtn = new QPushButton;
    confirmBtn->setText("确定");
    layout->addWidget(showTree);
    layout->addWidget(confirmBtn);
    treeWidget->setLayout(layout);
    treeWidget->show();
    //点击确定后统计勾选内容
    connect(confirmBtn,&QPushButton::clicked,[=]{
        getItemPath(item);
        treeWidget->close();
    });
}

QList<QTreeWidgetItem*> GroupDialog::traTree(QTreeWidgetItem *item,QList<QTreeWidgetItem*>itemList)
{
    if(Qt::Checked==item->checkState(0))
        itemList.append(item);
    for(int i=0;i<item->childCount();i++)
    {
        itemList=traTree(item->child(i),itemList);
    }
    return itemList;
}

void GroupDialog::getItemPath(QTreeWidgetItem *item)
{
    QList<QTreeWidgetItem*>itemList;
    itemList=traTree(item,itemList);
    for(int i=0;i<itemList.size();i++)
    {
        QTreeWidgetItem* checkItem = itemList.at(i);
        QStringList nodeList;
        while(checkItem!=NULL)
        {
            nodeList<<checkItem->statusTip(0);
            checkItem=checkItem->parent();
        }
        QString nodePath;
        for(int i=0;i<nodeList.size();i++)
        {
            //将nodeList正向输出，相应的加入','
            nodePath+=nodeList.at(i);
            if(i!=nodeList.size()-1)
                nodePath+=",";
        }
        treeList.append(nodePath);
    }
}

QStringList GroupDialog::getTreeList()
{
    return treeList;
}

QString GroupDialog::getName()
{
    return name->text();
}

void GroupDialog::updateParentItem(QTreeWidgetItem* item)
{
    QTreeWidgetItem* parent = item->parent();
    if(parent == NULL)
    {
        return;
    }
    //选中的子节点个数
    int selectCount =0;
    int childCount=parent->childCount();
    for(int i=0;i<childCount;i++)
    {
        QTreeWidgetItem* childItem = parent->child(i);
        if(childItem->checkState(0)== Qt::Checked)
        {
            selectCount++;
        }
    }
    if(selectCount == childCount)
    {
        //全选状态
        parent->setCheckState(0,Qt::Checked);
    }
}


void GroupDialog::treeItemChanged(QTreeWidgetItem* item,int column)
{
    QString itemCheck = item->text(0);
    //选中时
    if(Qt::Checked == item->checkState(0))
    {
        QTreeWidgetItem* parent= item->parent();
        int count = item->childCount();
        if(count>0)
        {
            for(int i=0;i<count;i++)
            {
                //子节点也选中
                item->child(i)->setCheckState(0,Qt::Checked);
            }
        }
        else
        {
            //是子节点
            updateParentItem(item);
        }
    }
    else if(Qt::Unchecked == item->checkState(0))
    {
        int count = item->childCount();
        if(count>0)
        {
            for(int i=0;i<count;i++)
            {
                item->child(i)->setCheckState(0,Qt::Unchecked);
            }
        }
        else
        {
           updateParentItem(item);
        }
    }
}

