#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H
#include<QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QDialogButtonBox>
class ConfigWindow : public QDialog
{
    Q_OBJECT
public:
    explicit ConfigWindow(QWidget *parent = nullptr);
    QString getIP();
private:
    QLineEdit *inputLine;

};

#endif // CONFIGWINDOW_H
