#include "ldapserver.h"
//第二栏服务器部分
LdapServer::LdapServer(QWidget *parent)
    : QTreeWidget(parent)
{
    setUpConfig();
}

/* 鼠标按下事件 */
void LdapServer::mousePressEvent(QMouseEvent* ev) {
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
void LdapServer::mouseReleaseEvent(QMouseEvent* ev) {
    this->activeItem = nullptr;
    QTreeWidget::mouseReleaseEvent(ev);
}

void LdapServer::setUpConfig()
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &LdapServer::rightButtonClicked, //&QWidget::customContextMenuRequested,
        this, &LdapServer::onMenuRequested);
}
void LdapServer::onMenuRequested(const QPoint& pos) {
    if (!activeItem) return;
    QMenu* pMenu = new QMenu(this);
//    QAction* modify_localityName = new QAction(tr("修改localityName"), this);
//    QAction* modify_host = new QAction(tr("修改host"), this);
//    QAction* modify_o = new QAction(tr("修改organizationName"), this);
//    QAction* modify_ou = new QAction(tr("修改organiztionalUnitName"), this);

//    /* 添加菜单项 */
//    pMenu->addAction(modify_localityName);
//    pMenu->addAction(modify_host);
//    pMenu->addAction(modify_o);
//    pMenu->addAction(modify_ou);
    /* 连接菜单信号槽 *///把右键菜单里的功能信号和函数相连接
//    connect(modifyPwd, &QAction::triggered, this, &OrganizationTree::onModifyPwd);
//    connect(moreMsg, &QAction::triggered, this, &OrganizationTree::onMoreMsg);
//    connect(deleteInfo, &QAction::triggered, this, &OrganizationTree::onDeleteInfo);
//    connect(modifyInfo, &QAction::triggered, this, &OrganizationTree::onModifyInfo);
    QAction* modify_info = new QAction(tr("修改信息"), this);
    pMenu->addAction(modify_info);
    connect(modify_info, &QAction::triggered, this, &LdapServer::onModifyInfo);

    /* 在鼠标右键处显示菜单 */
    pMenu->exec(cursor().pos());

    /* 释放内存 */
    QList<QAction*> list = pMenu->actions();
    foreach(QAction * pAction, list) delete pAction;
    delete pMenu;
}

void LdapServer::setUpTree(QString host, QString userName)
{
    this->setHeaderLabels(QStringList("服务器"));
    QTreeWidgetItem* item = new QTreeWidgetItem(QStringList("服务器"));
    item->setStatusTip(0, item->text(0));

    LDAPCatalogue* ld = new LDAPCatalogue();
    BaseTreeNode* LDAPTree = ld->GetCatalogue0(host, userName);  //这里返回的是根节点
    //这是第一个节点的处理
    addTopLevelItem(item);   //将跟节点加进目录树里面 显示
    LDAPTree->traverseTree(LDAPTree, item);
    //两个用户加进树里  ----手动，怎么从服务器获取信息加
//    QTreeWidgetItem* childItem1 = new QTreeWidgetItem(QStringList("00目录服务"));
//    item->addChild(childItem1);
//    QTreeWidgetItem* childItem2 = new QTreeWidgetItem(QStringList("01推免服务"));
//    item->addChild(childItem2);
//    QTreeWidgetItem* childItem3 = new QTreeWidgetItem(QStringList("02考务服务"));
//    item->addChild(childItem3);
//    QTreeWidgetItem* childItem4 = new QTreeWidgetItem(QStringList("03硕士招生服务"));
//    item->addChild(childItem4);
//    QTreeWidgetItem* childItem5 = new QTreeWidgetItem(QStringList("04博士招生服务"));
//    item->addChild(childItem5);
//    QTreeWidgetItem* childItem6 = new QTreeWidgetItem(QStringList("05报到服务"));
//    item->addChild(childItem6);

    this->expandAll();
}

void LdapServer::onModifyInfo() {
    emit ModifyNode(activeItem->statusTip(0));
}

void LdapServer::onModifyPwd() {
    emit ModifyPwd(activeItem->statusTip(0));
}
