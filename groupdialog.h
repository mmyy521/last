#ifndef GROUPDIALOG_H
#define GROUPDIALOG_H

#include <QDialog>
#include<QTreeWidget>
#include<QLineEdit>

namespace Ui {
class GroupDialog;
}

class GroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GroupDialog(QWidget *parent = 0);
    ~GroupDialog();
    QString ip,path;
    void getMoveCn(QString host,QString basePath);
    void updateParentItem(QTreeWidgetItem* item);
    QList<QTreeWidgetItem*> traTree(QTreeWidgetItem* item,QList<QTreeWidgetItem*>itemList);
    QStringList getTreeList();
    QString getName();



public slots:
    void tempFunction();
    void treeItemChanged(QTreeWidgetItem* item,int column);
    void getItemPath(QTreeWidgetItem*item);


private:
    Ui::GroupDialog *ui;
    QLineEdit* name;
    QString memberName;
    QStringList treeList;
};

#endif // GROUPDIALOG_H
