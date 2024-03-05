#ifndef MULTIPLEDIALOG_H
#define MULTIPLEDIALOG_H

#include<QString>
#include<QWidget>
#include<QDialog>
class QLineEdit;
class QLable;
class MultipleDialog : public QDialog
{
public:
    explicit MultipleDialog(QWidget *parent=nullptr);
    QStringList getStrings();
    QString getPicBuff();
    int getgid();

private:
    QList<QLineEdit*>fields;
    QImage image;
    char *pBuff;
    QString pBase64;
    int gid;

public slots:
    void handleRadioButtonClicked(int id);
};

#endif // MULTIPLEDIALOG_H
