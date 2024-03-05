#ifndef MODIFYPWD_H
#define MODIFYPWD_H

#include<QDialog>
#include<QWidget>
#include<QLineEdit>
#include<QFormLayout>
#include<QLabel>
#include<QDialogButtonBox>
#include<QPushButton>

//用于用户修改密码的函数
class ModifyPwd : public QDialog
{
public:
    explicit ModifyPwd(QWidget *parent=nullptr,bool newPwd=false);
    QString getString();
    QStringList getPwd();

private:
    QList<QLineEdit*>newField;
    QLineEdit *oldField;

};

#endif // MODIFYPWD_H
