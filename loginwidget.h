#pragma once

#include <QWidget>
#include<QDebug>
#include<QHBoxLayout>
#include<QTimer>
#include<QDateTime>
#include<QMessageBox>
#include<QThread>
#include "ui_loginwidget.h"
#include <QSettings>
#include "configwindow.h"
class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();
    QString loginName;
    QString loginPwd;

    Ui::LoginWidgetClass* ui;
    QString userName;      //登录服务器
    QString userPwd;
    //QString host;
    ConfigWindow *configWindow;
private:
public slots:
    void onConfigMenuTriggered();
};
