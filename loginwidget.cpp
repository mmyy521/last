#include "loginwidget.h"
//第一个，登录界面
LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::LoginWidgetClass)
{
    ui->setupUi(this);
    this->setWindowTitle("登录");

    //QMenu* configMenu = ui->menuBar->addMenu("配置");
    // 添加一个动作，当菜单项被点击时触发
//    QAction *configAction = new QAction("", this);
//    configMenu->addAction(configAction);
//    connect(configAction, &QAction::triggered, this, &LoginWidget::onConfigMenuTriggered);

    connect(this->ui->configButton, &QPushButton::clicked,this,&LoginWidget::onConfigMenuTriggered);

    this->ui->pwdinput->setEchoMode(QLineEdit::Password);
    QLabel* time_label = new QLabel("", this);
    //time_label->setAlignment(Qt::AlignCenter);	// 时间文字居中
    ui->timeBar->addWidget(time_label);
    // 设置永久组件，默认右对齐。伸缩因子参数 >0 尽可能占用可用空间，与其他具有相同或更高值的伸缩项成比例，
    // 为了将状态组件铺满状态栏达到标签居中和右对齐效果

    // 时间
    QTimer* timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, [time_label]()
        // 值传递不修改值，函数会调用销毁，保持对象生命周期
        // 引用传递修改值
        {
            QDateTime cur_time = QDateTime::currentDateTime();

            QString cur_time_str = cur_time.toString("yyyy-MM-dd hh:mm:ss");

            time_label->setText(cur_time_str);
        });
    timer->start(1000); // 每1000毫秒（1秒）更新一次
    //底部
    QLabel* copyright_label = new QLabel("2019-2023 北林网工技术支持", this);
    ui->statusBar->addPermanentWidget(copyright_label, 1);
    copyright_label->setAlignment(Qt::AlignCenter);
    //目录部分
//    connect(ui->loginButton, &QPushButton::clicked, [&]() {//捕获外部作用域中所有变量, 并作为引用在函数体内使用 (按引用捕获)
//        //获取输入的ip地址
//        this->host = "211.71.149.249";
//        //从输入框获取账号
//        //this->userName = ui->userinput->text();
//        this->userName = "cn=Manager,dc=bjfu,dc=edu.cn";
//        //从输入框获取密码
//        //this->userPwd = ui->pwdinput->text();
//        this->userPwd = "secret";
//        qDebug() << this->host << this->userName << this->userPwd;
//        //后来被重写了吗
//        PWCHAR host_P=(PWCHAR)this->host.toStdWString().c_str();
//        ServerConnect* login = new ServerConnect(this);   //建立连接实体
//        login->ipAddress = this->host;    //IP地址
//        login->dn = this->userName;       //dn
//        login->cred = this->userPwd;      //密码
//        login->host=(PWCHAR)this->host.toStdWString().c_str();
//        int flag = login->LDAPConnect();  //连接服务器
//        //QThread::msleep(5000);
//        //qDebug()<<flag;
//        //int flag=1;
//        if (flag == 1)         //连接成功
//        {
//            //savename = login->dn; //dn名字
//            //qDebug() << "savename:"<<savename;     //这里输出了用户名。
////            Widget *w = new Widget(this, this->host, this->userName);  //这里定义了新的窗口，这个类的实现在widget中
////            //这几个connect是什么作用？
////            //添加节点
////            connect(w, &Widget::addNodeToLdap, [=](QString path) {//[=]捕获外部作用域中所有变量, 并作为副本在函数体内使用 (按值捕获)
////                if (login->addUserNode(path))        //调用函数
////                {
////                    qDebug() << "addNodeToLdap";
////                    w->refreshTree();  //更新目录树
////                }
////                });
////            //添加组织单位
////            connect(w, &Widget::addOuToLdap, [=](QString path) {
////                if (login->addOuNode(path))
////                {
////                    qDebug() << "addOuToLdap";
////                    w->refreshTree();
////                }
////                });
////            //修改节点
////            connect(w, &Widget::modifyNodeToLdap, [=](QString path) {
////                if (login->modifyNode(path))
////                {
////                    qDebug() << "modifyNodeToLdap";
////                    w->refreshTree();
////                }
////                });
////            //删除节点
////            connect(w, &Widget::deleteNodeToLdap, [=](QString path) {
////                if (login->deleteNode(path))
////                {
////                    qDebug() << "deleteNodeToLdap";
////                    w->refreshTree();
////                }
////                });
////            //修改密码
////            connect(w, &Widget::modifyPwdToLdap, [=](QString path) {
////                qDebug() << "modifyPwdToLdap";
////                login->modifyPwd(path);
////                });
////            //展示细节
////            connect(w, &Widget::detailNodeToLdap, [=](QString path) {
////                qDebug() << "detailNodeToLdap";
////                detailWidget = new DetailWidget;
////                login->detailNode(path, detailWidget);
////                detailWidget->setDetail();
////                detailWidget->show();
////                });
////            //添加组
////            connect(w, &Widget::addGroupToLdap, [=](QString path) {
////                groupWidget = new GroupDialog;
////                if (login->addGroupNode(path, groupWidget))
////                {
////                    qDebug() << "addGroupToLdap";
////                    w->refreshTree();
////                }
////                });
//        //    //自身隐藏
//        //    this->hide();   //隐藏登陆窗口
//        //    //界面显示
//        //    w->show();  //显示新窗口
//        //    //退出登录
//        //    connect(w, &Widget::quitLogin, [=]() {
//        //        w->hide();
//        //        ui->userName->clear();
//        //        ui->userPwd->clear();
//        //        this->show();  //退出登陆之后再显示登陆界面
//        //        });
//            mw = new MainWindow(this);
//            this->hide();   //隐藏登陆窗口
//            mw->show();  //显示新窗口
//        }
//        else
//        {
//            QMessageBox::critical(this, "错误信息", "登录失败");
//        }
//        });
}

LoginWidget::~LoginWidget()
{}

void LoginWidget::onConfigMenuTriggered() {
    configWindow = new ConfigWindow(this);
    QString cfgip;
    if(configWindow->exec()==QDialog::Accepted)
    {
        cfgip= configWindow->getIP();

    }
    else if(configWindow->close()==true)
    {
    }
    QString filePath = "..\\config.ini";
    QSettings settings(filePath, QSettings::IniFormat);
    settings.setValue("IP", cfgip);
    qDebug() << "IP:" << cfgip;
    settings.sync();
    QMessageBox::information(this, "提示", "配置成功！");
}
