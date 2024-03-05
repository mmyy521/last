#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>

#include "ui_main_window.h"
#include "loginwidget.h"
#include "serverconnect.h"
#include "widget.h"
#include"AuthorizedUser.h"
#include"ldapserver.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowClass; };
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool loginldap();
    ServerConnect *login;
    Widget* left_widget;
    AuthorizedUser *aUser;
    LdapServer *ldapServer;
protected:

    void showEvent(QShowEvent* event) override;
    // 绘制界面。show()显示时触发事件
    // 这个函数是QWidget的保护成员函数，因此在子类中可以重写它
    // 在该方法而不是构造方法中绘制界面，为了获取到正确的窗口尺寸

private:
    Ui::MainWindowClass *ui;
};
