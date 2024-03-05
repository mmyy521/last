#ifndef BASETREENODE_H
#define BASETREENODE_H
#include<QString>
#include<QList>
#include<QTreeWidget>
#include<QStringList>
class BaseTreeNode
{
private:
    //父节点指针
    BaseTreeNode *parent =nullptr;
    //子节点
    QList<BaseTreeNode*> child;
    //节点名称
    QString name;
public:
    BaseTreeNode(QString name,BaseTreeNode *parent=nullptr);
    void setParentNode(BaseTreeNode *parent);
    BaseTreeNode* getParentNode();
    void addChildNode(BaseTreeNode *node);
    void setName(QString name);
    QString getName();
    QList<BaseTreeNode*> childs();
    void traverse(BaseTreeNode*root);
    //遍历树
    void traverseTree(BaseTreeNode*root,QTreeWidgetItem *item);
    //遍历树添加勾选项
    void addCheck(BaseTreeNode*root,QTreeWidgetItem *item);
    BaseTreeNode* findNode(BaseTreeNode*root,QString name);
    BaseTreeNode* findNode(BaseTreeNode*root,QStringList list,int j);
};

#endif // BASETREENODE_H
