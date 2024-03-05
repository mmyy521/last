#include "basetreenode.h"
#include<QDebug>

BaseTreeNode::BaseTreeNode(QString name,BaseTreeNode *parent) //添加孩子
{
    this->parent=parent;
    this->name=name;
}

void BaseTreeNode::setParentNode(BaseTreeNode *parent)  //设置父母亲节点
{
    this->parent=parent;
}

BaseTreeNode* BaseTreeNode::getParentNode()  //获取父节点
{
    return this->parent;
}

void BaseTreeNode::addChildNode(BaseTreeNode *node)  //添加孩子节点
{
    this->child.append(node);
    node->setParentNode(this);
}

void BaseTreeNode::setName(QString name)  //设置节点名字
{
    this->name=name;
}

QString BaseTreeNode::getName()  //获取节点名字
{
    return this->name;
}

QList<BaseTreeNode*> BaseTreeNode::childs()  //返回孩子节点
{
    return this->child;
}

//遍历结点
void BaseTreeNode::traverse(BaseTreeNode *root)  //从根开始遍历
{
    qDebug()<<"parent:"<<root->getName();
    for(int i=0;i<root->childs().length();i++)
    {
        BaseTreeNode *child;
//        qDebug()<<"parent";
//        qDebug()<<root->childs().at(i)->parent->getName();  //输出根名称
        //qDebug()<<root->getName();
        qDebug()<<"child:"<< root->childs().at(i)->getName();  //输出孩子节点名称
        traverse(root->childs().at(i));  //遍历孩子节点以下的节点
    }
}

//遍历树 同时将结点添加到树形结构中
//这个是目录节点添加进树里，显示目录用的，Qt自带的结构
void BaseTreeNode::traverseTree(BaseTreeNode *root,QTreeWidgetItem *item)
{
    for(int i=0;i<root->childs().length();i++)  //对于根节点的每一个孩子
    {
        //定义树节点，并初始化                               赋值一个列表为孩子节点的第i个的名字
        QTreeWidgetItem * childItem = new QTreeWidgetItem(QStringList()<<root->childs().at(i)->getName());
        //设置啥
        childItem->setStatusTip(0,childItem->text(0));  //这是一个名
        //设置文本信息  here!
        //这是显示出来的名
        childItem->setText(0,childItem->text(0).split(',')[0].split('=')[1]);
        //childItem->setText(0,childItem->text(0));
        //添加孩子信息
        item->addChild(childItem); //给目录树添加节点
        //继续遍历添加
        traverseTree(root->childs().at(i),childItem);  //对于这个节点和新添加的节点，继续递归添加
    }
}

//遍历树 同时将结点添加到树形结构中
void BaseTreeNode::addCheck(BaseTreeNode *root,QTreeWidgetItem *item)
{
    for(int i=0;i<root->childs().length();i++)
    {
        QTreeWidgetItem * childItem = new QTreeWidgetItem(QStringList()<<root->childs().at(i)->getName());
        //当不是组织且子节点为0时为用户
        if(QString::compare(childItem->text(0).split('=')[0],"ou")!=0)
            childItem->setCheckState(0,Qt::Unchecked);  //这啥意思
        childItem->setStatusTip(0,childItem->text(0));
        childItem->setText(0,childItem->text(0).split('=')[1]);
        item->addChild(childItem);
        addCheck(root->childs().at(i),childItem);
    }
}

//查找结点是否存在
BaseTreeNode* BaseTreeNode::findNode(BaseTreeNode *root,QString name)
{
    if(!root)
        return nullptr;
    BaseTreeNode* node=nullptr;
    //qDebug()<<"------root:"<<root->getName();
    if(root->getName()==name)
        return root;
    for(int i=0;i<root->childs().length();i++)
    {
        //qDebug()<<"node:"<<root->childs().at(i)->getName();
        if(root->childs().at(i)->getName()==name)
        {
            //qDebug()<<"YES!!!";
             node = root->childs().at(i);
             //return root->childs().at(i);
             break;
        }
        else if(root->childs().at(i))
        {
            //qDebug()<<"NO!!!";
            node = findNode(root->childs().at(i),name);  //!!又进了这一个循环，然后在根的位置被清空了。!!
            if(node)     //大bug
                break;
        }
    }
    return node;
}
//最后一个参数是指？从根部开始查
BaseTreeNode* BaseTreeNode::findNode(BaseTreeNode* root,QStringList list,int j)
{
    if(!root)
        return nullptr;
    if(list.length()==3 || j == 0) //=3的话，就是根节点下的第二层节点，j=0时这个root就是要找的
        return root;
    BaseTreeNode* node=nullptr;
    for(int i=0;i<root->childs().length();i++)
    {
        QString dn=root->childs().at(i)->getName();
        //qDebug()<<"node:"<<dn;
        if(dn.split(',')[0]==list[j])   //这里是为了一层一层向下匹配
        //if(root->childs().at(i)->getName()==list[j])
        {
            j--;
            node = findNode(root->childs().at(i),list,j);  //!!又进了这一个循环，然后在根的位置被清空了。!!
            break;
        }
    }
    return node;
}
