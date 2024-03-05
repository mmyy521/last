#ifndef SERVERCONNECT_H
#define SERVERCONNECT_H
#include <windows.h>
#include <windef.h>
#include <winldap.h>
#include <winber.h>
#include <QWidget>
#include"multipledialog.h"
#include"muldialogou.h"
#include"modifydialog.h"
#include"modifypwd.h"
#include"detailwidget.h"
#include"groupdialog.h"
#include"ldapcatalogue.h"

//接收到信号而进行的操作
class ServerConnect : public QWidget
{
public:
    QString dn;         //名称
    QString cred;       //密码
    QString ipAddress;  //ip地址
    PWSTR host;         //初始化所用host
    LDAP *ld=NULL;      //保存连接LDAP的信息
    ServerConnect(QWidget* parent = 0);
    int LDAPConnect();  //返回当前所有实体
    void LDAPDisconnect();
    //进行操作的函数
    bool addUserNode(QString path);
    bool addUserNode0(QString path);
    bool addOuNode(QString path);
    bool deleteNode(QString path);
    void detailNode(QString path,DetailWidget *deWidget);
    bool modifyNode(QString path);
    bool modifyConnect(QString path,QStringList attrName,QStringList attrValue);

    bool modifyNode0(QString path);
    bool modifyConnect0(QString path,QStringList attrName,QStringList attrValue);

    bool addGroupNode(QString path,GroupDialog *groupWidget);
    void modifyPwd(QString path);
    bool my_ldap_delete_s(BaseTreeNode *curr_tree);
    MultipleDialog *mDialog;
    MulDialogOu *mOuDialog;
    ModifyDialog *modDialog;
    ModifyPwd *mPwd;
    ModifyPwd *newPwd;

private:
    ULONG port = LDAP_PORT;  // 端口
    ULONG version=LDAP_VERSION3;//版本号
    //认证信息
    ULONG method=LDAP_AUTH_SIMPLE;//简单认证
};

#endif // SERVERCONNECT_H
