#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QMouseEvent>
#include <QScrollArea>
#include <QMessageBox>
#include <QTimer>
#include <QPushButton>
#include <QScrollArea>

#include "organizationtree.h"
#include "ui_main_window.h"
#include"AuthorizedUser.h"
#include"ldapserver.h"
namespace Ui {
    class Widget;
}
// 在头文件中定义结构体

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget* parent = 0, QString host = "", QString userName = "");
    ~Widget();
    void setUpConfig();
    void refreshTree();
    QString host;
    QString userName;
    AuthorizedUser *aUser;
    LdapServer *ldapServer;
    OrganizationTree *treeWidget;
    QPushButton *logout;

signals:
    //给服务器添加用户的信号
    void addNodeToLdap(QString);
    //给服务器添加组织的信号
    void addOuToLdap(QString);
    //给服务器删除结点的信号
    void deleteNodeToLdap(QString);
    //给服务器显示详细信息的信号
    void detailNodeToLdap(QString);
    //给服务器修改信息的信号
    void modifyNodeToLdap(QString);
    //退出账户信号
    void quitLogin();
    //修改账号密码信号
    void modifyPwdToLdap(QString);
    //添加小组的信号
    void addGroupToLdap(QString);

    void modifyNodeToLdap_Auth(QString);
    void modifyPwdToLdap_Auth(QString);

    void modifyNodeToLdap_Server(QString);

public slots:
    //添加用户结点给serverconnect的槽函数
    void onAddUserNode(QString path);
    //添加组织结点给serverconnnect的槽函数
    void onAddOuNode(QString path);
    //删除结点给serverconnect的槽函数
    void onDeleteNode(QString path);
    //显示详情给serverconnect的槽函数
    void onDetailNode(QString path);
    //修改信息给serverconnect的槽函数
    void onModifyNode(QString path);
    //退出当前账号
    void onQuitAccount();
    //修改账户密码槽函数
    void onModifyPwd(QString path);
    //刷新树的槽函数
    void timeRefresh();
    //增加小组槽函数
    void onAddGroupNode(QString path);

    void onModifyNode_Auth(QString path);
    void onModifyPwd_Auth(QString path);

    void onModifyNode_Server(QString path);

private:
    //Ui::MainWindowClass* ui;
};

#endif // WIDGET_H
