#include "organizationtree.h"
//QString LoginWidget::savename;
//建立目录树，调用LDAPCatalogue类，设置右键展开菜单内容以及相关的槽函数
//找到dn并发送信号
OrganizationTree::OrganizationTree(QWidget* parent) :
    QTreeWidget(parent) {
    setUpConfig();
}
OrganizationTree::~OrganizationTree()
{
}
/* 鼠标按下事件 */
void OrganizationTree::mousePressEvent(QMouseEvent* ev) {
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

///* 鼠标释放事件 */
void OrganizationTree::mouseReleaseEvent(QMouseEvent* ev) {
    this->activeItem = nullptr;
    QTreeWidget::mouseReleaseEvent(ev);
}

/* 设置信号槽和配置 */
void OrganizationTree::setUpConfig() {
    //qDebug()<<"设置右键展开的功能";
    // 设置上下文菜单选项 --- 用于实现右键菜单
    QString savename="cn=Manager,dc=bjfu,dc=edu.cn";
    //if (LoginWidget::savename == "cn=admin,dc=nodomain") //这里设置右键展开的槽函数
    if(savename=="cn=Manager,dc=bjfu,dc=edu.cn")
    {
        qDebug()<<"管理员用户";
        setContextMenuPolicy(Qt::CustomContextMenu);
        connect(this, &OrganizationTree::rightButtonClicked, //&QWidget::customContextMenuRequested,
            this, &OrganizationTree::onCustomContextMenuRequestedManager);
    }
    else
    {
        qDebug()<<"其他用户";
        setContextMenuPolicy(Qt::CustomContextMenu);
        connect(this, &OrganizationTree::rightButtonClicked, //&QWidget::customContextMenuRequested,
            this, &OrganizationTree::onCustomContextMenuRequested);
    }
}

/* 建立组织机构目录树 */
void OrganizationTree::setUpTree(QString host, QString userName) {
    clear();
    this->setHeaderLabels(QStringList("系统登录用户管理"));  //目录头标签
    LDAPCatalogue* ld = new LDAPCatalogue();
    BaseTreeNode* LDAPTree = ld->GetCatalogue(host, userName);  //这里返回的是根节点
    // 将获取的结点添加到item中       根节点的名字
    QTreeWidgetItem* item = new QTreeWidgetItem(QStringList() << LDAPTree->getName());
    item->setStatusTip(0, item->text(0));
    item->setText(0, item->text(0).split(",")[0].split("=")[1]);  //这是第一个节点的处理
    addTopLevelItem(item);   //将跟节点加进目录树里面 显示
    LDAPTree->traverseTree(LDAPTree, item); //将自己定义的tree复制到QTree上 显示
}

/* 右键弹出菜单——管理员菜单 */
void OrganizationTree::onCustomContextMenuRequestedManager(const QPoint& pos) {
    if (!activeItem) return;

    QMenu* pMenu = new QMenu(this);
    QAction* moreMsg = new QAction(tr("详细信息"), this);
    QAction* modifyInfo = new QAction(tr("修改信息"), this);
    QAction* deleteInfo = new QAction(tr("删除结点"), this);
    QAction* addUserNode = new QAction(tr("添加用户"), this);
    QAction* addOuNode = new QAction(tr("添加组织"), this);
    //QAction* addGroupNode = new QAction(tr("添加小组"), this);
    /* 添加菜单项 */
    pMenu->addAction(moreMsg);
    pMenu->addAction(modifyInfo);
    pMenu->addAction(deleteInfo);
    //if(activeItem->text(0).contains("ou")||activeItem->text(0)=="yz")
    //{
    pMenu->addAction(addUserNode);
    pMenu->addAction(addOuNode);
    //pMenu->addAction(addGroupNode);
    //}
    //QAction* modifyPwd = new QAction(tr("修改当前用户密码"), this); //这是一个什么对象

    //pMenu->addAction(modifyPwd);

    /* 连接菜单信号槽 *///把右键菜单里的功能信号和函数相连接
    //connect(modifyPwd, &QAction::triggered, this, &OrganizationTree::onModifyPwd);

    /* 连接菜单信号槽 */
    connect(moreMsg, &QAction::triggered, this, &OrganizationTree::onMoreMsg);
    connect(deleteInfo, &QAction::triggered, this, &OrganizationTree::onDeleteInfo);
    connect(modifyInfo, &QAction::triggered, this, &OrganizationTree::onModifyInfo);
    connect(addUserNode, &QAction::triggered, this, &OrganizationTree::onAddInfo);
    connect(addOuNode, &QAction::triggered, this, &OrganizationTree::onAddOu);
    //connect(addGroupNode, &QAction::triggered, this, &OrganizationTree::onAddGroup);

    /* 在鼠标右键处显示菜单 */
    pMenu->exec(cursor().pos());

    /* 释放内存 */
    QList<QAction*> list = pMenu->actions();
    foreach(QAction * pAction, list) delete pAction;
    delete pMenu;
}

/* 右键弹出菜单——普通用户菜单 */
void OrganizationTree::onCustomContextMenuRequested(const QPoint& pos) {
    if (!activeItem) return;

    QMenu* pMenu = new QMenu(this);
    QAction* modifyPwd = new QAction(tr("修改当前用户密码"), this);

    /* 添加菜单项 */
    pMenu->addAction(modifyPwd);

    /* 连接菜单信号槽 */
    connect(modifyPwd, &QAction::triggered, this, &OrganizationTree::onModifyPwd);

    /* 在鼠标右键处显示菜单 */
    pMenu->exec(cursor().pos());

    /* 释放内存 */
    QList<QAction*> list = pMenu->actions();
    foreach(QAction * pAction, list) delete pAction;
    delete pMenu;
}

/* 详细信息 -- 槽函数 */
void OrganizationTree::onMoreMsg() {
//    QStringList nodeList;
//    while (activeItem != NULL)
//    {
//        qDebug() << "反推："<<activeItem->statusTip(0);    //这里取出来的就是dn
//        nodeList << activeItem->statusTip(0);
//        activeItem = activeItem->parent();
//    }
//    QString nodePath;
//    for (int i = 0; i < nodeList.size(); i++)
//    {
//        //将nodeList正向输出，相应的加入','
//        nodePath += nodeList.at(i);
//        if (i != nodeList.size() - 1)
//            nodePath += ",";
//    }
    //qDebug() << "查询条目："<<activeItem->statusTip(0);
    emit DetailNode(activeItem->statusTip(0));
}

/* 添加用户新结点 -- 槽函数 */
void OrganizationTree::onAddInfo() {
//    QStringList nodeList;
//    while (activeItem != NULL)
//    {
//        nodeList << activeItem->statusTip(0);
//        activeItem = activeItem->parent();
//    }
//    QString nodePath;
//    for (int i = 0; i < nodeList.size(); i++)
//    {
//        //将nodeList正向输出，相应的加入','
//        nodePath += nodeList.at(i);
//        if (i != nodeList.size() - 1)
//            nodePath += ",";
//    }
    emit AddUserNode(activeItem->statusTip(0));
}

/* 添加组织信息节点槽函数*/
void OrganizationTree::onAddOu() {
//    QStringList nodeList;
//    while (activeItem != NULL)
//    {
//        nodeList << activeItem->statusTip(0);
//        activeItem = activeItem->parent();
//    }
//    QString nodePath;
//    for (int i = 0; i < nodeList.size(); i++)
//    {
//        //将nodeList正向输出，相应的加入','
//        nodePath += nodeList.at(i);
//        if (i != nodeList.size() - 1)
//            nodePath += ",";
//    }
    emit AddOuNode(activeItem->statusTip(0));
}

/* 修改信息槽函数 */
void OrganizationTree::onModifyInfo() {
//    QStringList nodeList;
//    while (activeItem != NULL)
//    {
//        nodeList << activeItem->statusTip(0);
//        activeItem = activeItem->parent();
//    }
//    QString nodePath;
//    for (int i = 0; i < nodeList.size(); i++)
//    {
//        //将nodeList正向输出，相应的加入','
//        nodePath += nodeList.at(i);
//        if (i != nodeList.size() - 1)
//            nodePath += ",";
//    }
    emit ModifyNode(activeItem->statusTip(0));
}

/* 删除信息 -- 槽函数 */
void OrganizationTree::onDeleteInfo() {
    QMessageBox::StandardButton confirm;
    confirm = QMessageBox::question(this, "提示", "确认要删除吗？", QMessageBox::Yes | QMessageBox::No);
    if (confirm == QMessageBox::Yes)
    {
//        QStringList nodeList;
//        while (activeItem != NULL)
//        {
//            nodeList << activeItem->statusTip(0);
//            activeItem = activeItem->parent();
//        }
//        QString nodePath;
//        for (int i = 0; i < nodeList.size(); i++)
//        {
//            //将nodeList正向输出，相应的加入','
//            nodePath += nodeList.at(i);
//            if (i != nodeList.size() - 1)
//                nodePath += ",";
//        }
        emit DeleteNode(activeItem->statusTip(0));
    }
    if (confirm == QMessageBox::No)
        return;
}

/* 修改密码 -- 槽函数 */
void OrganizationTree::onModifyPwd() {
//    QStringList nodeList;
//    while (activeItem != NULL)  //根据树把这个路径存进nodeList
//    {
//        nodeList << activeItem->statusTip(0);
//        activeItem = activeItem->parent();
//    }
//    QString nodePath;
//    for (int i = 0; i < nodeList.size(); i++)
//    {
//        //将nodeList正向输出，相应的加入','
//        nodePath += nodeList.at(i);
//        if (i != nodeList.size() - 1)
//            nodePath += ",";
//    }
    emit ModifyPwd(activeItem->statusTip(0));       //发送信息的作用，是一个QAction signal
}

/* 添加小组——槽函数  */
void OrganizationTree::onAddGroup() {
//    QStringList nodeList;
//    while (activeItem != NULL)
//    {
//        nodeList << activeItem->statusTip(0);
//        activeItem = activeItem->parent();
//    }
//    QString nodePath;
//    for (int i = 0; i < nodeList.size(); i++)
//    {
//        //将nodeList正向输出，相应的加入','
//        nodePath += nodeList.at(i);
//        if (i != nodeList.size() - 1)
//            nodePath += ",";
//    }
    emit AddGroupNode(activeItem->statusTip(0));
}

