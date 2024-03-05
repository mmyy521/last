#ifndef LDAPSERVER_H
#define LDAPSERVER_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include<QMenu>
#include<QMouseEvent>
#include "ldapcatalogue.h"
#include "basetreenode.h"
#include "serverconnect.h"
class LdapServer : public QTreeWidget
{
    Q_OBJECT

public:
    LdapServer(QWidget *parent = nullptr);
    void setUpConfig();
    void setUpTree(QString host, QString userName);
    QTreeWidgetItem* activeItem = nullptr;
    void mousePressEvent(QMouseEvent* ev);
    void mouseReleaseEvent(QMouseEvent* ev);

signals:
    void leftButtonClicked(const QPoint& pos = QPoint(0, 0));
    void rightButtonClicked(const QPoint& pos = QPoint(0, 0));
    void ModifyNode(QString path);
    void ModifyPwd(QString path);

public slots:
    void onMenuRequested(const QPoint& pos);
    void onModifyInfo();
    void onModifyPwd();

private:
};

#endif // LDAPSERVER_H
