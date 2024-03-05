#include "authorizeduser.h"
//第一栏当年授权用户部分
AuthorizedUser::AuthorizedUser(QWidget *parent)
    : QTreeWidget(parent)
{
    setUpConfig();
}
AuthorizedUser::~AuthorizedUser(){}
/* 鼠标按下事件 */
void AuthorizedUser::mousePressEvent(QMouseEvent* ev) {
    if (ev->button() == Qt::LeftButton) {
        QTreeWidget::mousePressEvent(ev);
        this->activeItem = itemAt(ev->pos());
        emit leftButtonClicked(ev->pos());
    }

    if (ev->button() == Qt::RightButton) {
        this->activeItem = itemAt(ev->pos());
        emit rightButtonClicked(ev->pos());
    }
}

/* 鼠标释放事件 */
void AuthorizedUser::mouseReleaseEvent(QMouseEvent* ev) {
    this->activeItem = nullptr;
    QTreeWidget::mouseReleaseEvent(ev);
}

void AuthorizedUser::setUpConfig()
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &AuthorizedUser::rightButtonClicked, //&QWidget::customContextMenuRequested,
        this, &AuthorizedUser::onMenuRequested);
}
void AuthorizedUser::onMenuRequested(const QPoint& pos) {
    if (!activeItem) return;
    QMenu* pMenu = new QMenu(this);
//    QAction* modify_userPassword = new QAction(tr("修改userPassword"), this);
//    QAction* modify_initials = new QAction(tr("修改initials"), this);
//    QAction* modify_labeledURI = new QAction(tr("修改labeledURI"), this);
//    QAction* modify_pager = new QAction(tr("修改pager"), this);

//    /* 添加菜单项 */
//    pMenu->addAction(modify_userPassword);
//    pMenu->addAction(modify_initials);
//    pMenu->addAction(modify_labeledURI);
//    pMenu->addAction(modify_pager);
    /* 连接菜单信号槽 *///把右键菜单里的功能信号和函数相连接
//    connect(modifyPwd, &QAction::triggered, this, &OrganizationTree::onModifyPwd);
//    connect(moreMsg, &QAction::triggered, this, &OrganizationTree::onMoreMsg);
//    connect(deleteInfo, &QAction::triggered, this, &OrganizationTree::onDeleteInfo);
//    connect(modifyInfo, &QAction::triggered, this, &OrganizationTree::onModifyInfo);

    QAction* modify_info = new QAction(tr("修改信息"), this);
    QAction* modify_pwd = new QAction(tr("修改密码"), this);

    pMenu->addAction(modify_info);
    pMenu->addAction(modify_pwd);

    connect(modify_info, &QAction::triggered, this, &AuthorizedUser::onModifyInfo);
    connect(modify_pwd, &QAction::triggered, this, &AuthorizedUser::onModifyPwd);

    /* 在鼠标右键处显示菜单 */
    pMenu->exec(cursor().pos());

    /* 释放内存 */
    QList<QAction*> list = pMenu->actions();
    foreach(QAction * pAction, list) delete pAction;
    delete pMenu;
}
//ou=1,ou=2,ou=000,dc=bjfu,dc=edu.cn 在这个dn下获取
void AuthorizedUser::setUpTree(QString host, QString userName)
{
    this->setHeaderLabels(QStringList("当年授权用户"));

    LDAPCatalogue* ld = new LDAPCatalogue();
    BaseTreeNode* LDAPTree = ld->GetCatalogue0(host, userName);  //这里返回的是根节点
    //这是第一个节点的处理
    QTreeWidgetItem* item = new QTreeWidgetItem(QStringList("授权用户"));
    item->setStatusTip(0, item->text(0));
    addTopLevelItem(item);   //将跟节点加进目录树里面 显示
    LDAPTree->traverseTree(LDAPTree, item);

    //两个用户加进树里  ----手动，怎么从服务器获取信息加
//    QTreeWidgetItem* childItem1 = new QTreeWidgetItem(QStringList("Admin@yz.edu.cn"));
//    item->addChild(childItem1);
//    QTreeWidgetItem* childItem2 = new QTreeWidgetItem(QStringList("yz@yz.edu.cn"));
//    item->addChild(childItem2);
    this->expandAll();
}
/* 修改信息槽函数 */
void AuthorizedUser::onModifyInfo() {
    emit ModifyNode(activeItem->statusTip(0));
}

void AuthorizedUser::onModifyPwd() {
    emit ModifyPwd(activeItem->statusTip(0));
}
