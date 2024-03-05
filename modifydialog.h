#ifndef MODIFYDIALOG_H
#define MODIFYDIALOG_H

#include <QWidget>
#include<QLineEdit>
#include<QDialog>
class ModifyDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ModifyDialog(QWidget *parent = nullptr,QStringList list={""});
    QStringList getStrings();
    QString getPicBuff();



private:
    QList<QLineEdit*>fields;
    QImage image;
    QString pBase64;
};

#endif // MODIFYDIALOG_H
