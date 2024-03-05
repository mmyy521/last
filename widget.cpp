#include "widget.h"
//目录树结构的设置
Widget::Widget(QWidget* parent, QString host, QString userName) :
    QWidget(parent){
    QString authUser = "ou=1,ou=2,ou=000,dc=bjfu,dc=edu.cn";
    QString serverName="ou=2,ou=2,ou=000,dc=bjfu,dc=edu.cn";
    //当年授权用户
    aUser = new AuthorizedUser(this);
    aUser->resize(500,130);  //大小的控制固定了…… 待
    aUser->setUpTree(host, authUser);
    //服务器
    ldapServer = new LdapServer(this);
    ldapServer->resize(500,160);
    ldapServer->move(0,140);
    ldapServer->setUpTree(host, serverName);
    //系统登录用户管理
    treeWidget = new OrganizationTree(this);
    treeWidget->move(0,320);
    treeWidget->resize(500,450);
//    logout = new QPushButton(this);
//    logout->setText("退出此账号");
//    logout->move(100,720);  //调整位置  怎样按比例.
    setUpConfig();
    treeWidget->setUpTree(host, userName);  //----->每个模块各自建一个树
    treeWidget->expandAll();
    this->host = host;  //IP
    this->userName = userName;  //用户名
}

Widget::~Widget()
{}

void Widget::setUpConfig() {
    connect(aUser, &AuthorizedUser::ModifyNode, this, &Widget::onModifyNode_Auth);
    connect(aUser, &AuthorizedUser::ModifyPwd, this, &Widget::onModifyPwd_Auth);

    connect(ldapServer, &LdapServer::ModifyNode, this, &Widget::onModifyNode_Server);

    //连接增删改查信号和槽  发送方，信号，接收方，接受处理函数
    connect(treeWidget, &OrganizationTree::AddUserNode, this, &Widget::onAddUserNode);
    connect(treeWidget, &OrganizationTree::DeleteNode, this, &Widget::onDeleteNode);
    connect(treeWidget, &OrganizationTree::DetailNode, this, &Widget::onDetailNode);
    connect(treeWidget, &OrganizationTree::AddOuNode, this, &Widget::onAddOuNode);
    connect(treeWidget, &OrganizationTree::ModifyNode, this, &Widget::onModifyNode);
    //connect(pushButton, &QPushButton::clicked, this, &Widget::onQuitAccount);
    connect(treeWidget, &OrganizationTree::ModifyPwd, this, &Widget::onModifyPwd);
    //connect(treeWidget, &OrganizationTree::AddGroupNode, this, &Widget::onAddGroupNode);
    //计时刷新树结构
    QTimer* timer = new QTimer(this); //this 为parent类, 表示当前窗口
    connect(timer, SIGNAL(timeout()), this, SLOT(timeRefresh())); // SLOT填入一个槽函数
    timer->start(600000); // 1000毫秒, 等于 1 秒
}

void Widget::onAddUserNode(QString path) {
    emit addNodeToLdap(path);
}

void Widget::onAddOuNode(QString path) {
    emit addOuToLdap(path);
}

void Widget::onDeleteNode(QString path) {
    emit deleteNodeToLdap(path);
}

void Widget::onDetailNode(QString path) {
    emit detailNodeToLdap(path);
}

void Widget::onModifyNode(QString path)
{
    emit modifyNodeToLdap(path);
}
void Widget::refreshTree() {
    treeWidget->setUpTree(host, userName);
    treeWidget->expandAll();
}

void Widget::timeRefresh() {
    treeWidget->setUpTree(host, userName);
    treeWidget->expandAll();
}

void Widget::onQuitAccount() {
    QMessageBox::StandardButton confirm;
    confirm = QMessageBox::question(this, "提示", "确认要退出吗？", QMessageBox::Yes | QMessageBox::No);
    if (confirm == QMessageBox::Yes)
    {
        emit quitLogin();
    }
    if (confirm == QMessageBox::No)
        return;
}

void Widget::onModifyPwd(QString path) {
    emit modifyPwdToLdap(path);
}

void Widget::onAddGroupNode(QString path) {
    qDebug() << "进入widget";
    emit addGroupToLdap(path);
}

void Widget::onModifyNode_Auth(QString path)
{
    emit modifyNodeToLdap_Auth(path);
}

void Widget::onModifyPwd_Auth(QString path) {
    emit modifyPwdToLdap_Auth(path);
}

void Widget::onModifyNode_Server(QString path) {
    emit modifyNodeToLdap_Server(path);
}
