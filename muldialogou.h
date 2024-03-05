#ifndef MULDIALOGOU_H
#define MULDIALOGOU_H

#include<QString>
#include<QWidget>
#include<QDialog>
class QLineEdit;
class QLable;

class MulDialogOu : public QDialog
{
public:
    explicit MulDialogOu(QWidget *parent=nullptr);
    QStringList getStrings();
private:
    QList<QLineEdit*>fields;
};

#endif // MULDIALOGOU_H
