#pragma once

#include <QWidget>
#include <QTreeWidget>
#include <QObject>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QList>
#include <QMessageBox>
#include <QMouseEvent>
#include <stack>
#include <functional>

#include "ldapcatalogue.h"
#include "basetreenode.h"
#include "serverconnect.h"

//在自定义控件所调用的 自定义类之前不能包含头文件

//构建树并发送信号
class OrganizationTree : public QTreeWidget
{
    Q_OBJECT
public:
    OrganizationTree(QWidget *parent = nullptr);
    ~OrganizationTree();
    void setUpConfig();
    void setUpTree(QString host, QString userName);
    void mousePressEvent(QMouseEvent* ev);
    void mouseReleaseEvent(QMouseEvent* ev);

signals:
    void leftButtonClicked(const QPoint& pos = QPoint(0, 0));
    void rightButtonClicked(const QPoint& pos = QPoint(0, 0));
    //发送给服务器添加用户结点的信号
    void AddUserNode(QString path);
    //发送给服务器添加组织结点的信号
    void AddOuNode(QString path);
    //发送给服务器删除结点的信号
    void DeleteNode(QString path);
    //发送给服务器查看详情的信号
    void DetailNode(QString path);
    //发送给服务器修改信息的信号
    void ModifyNode(QString path);
    //发送给服务器修改密码的信号
    void ModifyPwd(QString path);
    //增加小组的信号
    void AddGroupNode(QString path);

public slots:
    void onCustomContextMenuRequestedManager(const QPoint& pos);
    void onCustomContextMenuRequested(const QPoint& pos);
    void onMoreMsg();
    //右键参数widget添加用户结点槽函数
    void onAddInfo();
    //右键参数widget添加组织结点槽函数
    void onAddOu();
    //右键参数widget删除结点槽函数
    void onDeleteInfo();
    //右键参数widget修改信息槽函数
    void onModifyInfo();
    //修改用户修改密码槽函数
    void onModifyPwd();
    //添加小组的槽函数
    void onAddGroup();

public:
    QTreeWidgetItem* activeItem = nullptr;
private:
};
