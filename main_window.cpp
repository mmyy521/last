#pragma execution_character_set("utf-8")

#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QTimer>
#include <QtCore/QDateTime>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QPushButton>
#include <QDebug>
#include "main_window.h"
#include "widget.h"
#include "detailwidget.h"
#include "groupdialog.h"
#include <QThread>
MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowClass())
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
bool MainWindow::loginldap()
{
    LoginWidget *loginwidget= new LoginWidget;
    loginwidget->show();

    connect(loginwidget->ui->loginButton, &QPushButton::clicked, [&]() {//捕获外部作用域中所有变量, 并作为引用在函数体内使用 (按引用捕获)
        //从输入框获取账号
        loginwidget->userName = loginwidget->ui->userinput->text();
        //从输入框获取密码
        loginwidget->userPwd = loginwidget->ui->pwdinput->text();
        //从配置文件读取ip
        QString cfgip;
        QString filePath = "..\\config.ini";
        if (QFile(filePath).exists()) {
            // 如果文件存在，则读取配置
            QSettings settings(filePath, QSettings::IniFormat);
            cfgip = settings.value("IP").toString();
            //"211.71.149.249"
            qDebug() << "IP:" << cfgip;

//        QString dn = "cn=manager,dc=bjfu,dc=edu.cn";
//        QString Pwd = "secret";
            login = new ServerConnect(this);   //建立连接实体
            login->ipAddress = cfgip;    //IP地址-String
            QString dn="cn=";
            dn+=loginwidget->userName;
            dn+=",ou=1,ou=2,ou=000,dc=bjfu,dc=edu.cn";       //dn
            login->dn = dn;
            login->cred = loginwidget->userPwd;      //密码
            int flag = login->LDAPConnect();  //连接服务器
            if (flag == 1)         //连接成功
            {
                //当年授权用户
    //            aUser = new AuthorizedUser(this);
    //            aUser->resize(500,130);  //大小的控制固定了…… 待
    //            aUser->setUpTree(ldaphost, dn);
    //            //服务器
    //            ldapServer = new LdapServer(this);
    //            ldapServer->resize(500,160);
    //            ldapServer->move(0,140);
    //            ldapServer->setUpTree(ldaphost, dn);
                left_widget = new Widget(this,cfgip,dn);
                //授权用户
                connect(left_widget, &Widget::modifyNodeToLdap_Auth, [=](QString path) {//[=]捕获外部作用域中所有变量, 并作为副本在函数体内使用 (按值捕获)
                    if (login->modifyNode(path))
                    {
                        qDebug() << "modifyNode";
                        //left_widget->refreshTree();
                    }
                    });
                connect(left_widget, &Widget::modifyPwdToLdap_Auth, [=](QString path) {
    //                if (login->addUserNode(path))        //调用函数
    //                {
    //                    qDebug() << "addNodeToLdap";
    //                    left_widget->refreshTree();  //更新目录树
    //                }
                    });
                //服务器
                connect(left_widget, &Widget::modifyNodeToLdap_Server, [=](QString path) {
                    if (login->modifyNode(path))
                    {
                        qDebug() << "modifyNode";
                        //left_widget->refreshTree();
                    }
                    });

                //添加节点
                connect(left_widget, &Widget::addNodeToLdap, [=](QString path) {
                    if (login->addUserNode(path))        //调用函数
                    {
                        qDebug() << "addNodeToLdap";
                        left_widget->refreshTree();  //更新目录树
                    }
                    });
                //添加组织单位
                connect(left_widget, &Widget::addOuToLdap, [=](QString path) {
                    if (login->addOuNode(path))
                    {
                        qDebug() << "addOuToLdap";
                        left_widget->refreshTree();
                    }
                    });
                //修改节点
                connect(left_widget, &Widget::modifyNodeToLdap, [=](QString path) {
                    if (login->modifyNode(path))
                    {
                        qDebug() << "modifyNodeToLdap";
                        left_widget->refreshTree();
                    }
                    });
                //删除节点
                connect(left_widget, &Widget::deleteNodeToLdap, [=](QString path) {
                    if (login->deleteNode(path))
                    {
                        qDebug() << "deleteNodeToLdap";
                        left_widget->refreshTree();
                    }
                    });
                //修改密码
                connect(left_widget, &Widget::modifyPwdToLdap, [=](QString path) {
                    qDebug() << "modifyPwdToLdap";
                    login->modifyPwd(path);
                    });
                //展示细节
                connect(left_widget, &Widget::detailNodeToLdap, [=](QString path) {
                    qDebug() << "detailNodeToLdap";
                    DetailWidget *detailWidget = new DetailWidget;
                    login->detailNode(path, detailWidget);
                    detailWidget->setDetail();
                    detailWidget->show();       //展示窗口
                    });
                //添加组
    //            connect(left_widget, &Widget::addGroupToLdap, [=](QString path) {
    //                GroupDialog *groupWidget = new GroupDialog;
    //                if (login->addGroupNode(path, groupWidget))
    //                {
    //                    qDebug() << "addGroupToLdap";
    //                    left_widget->refreshTree();
    //                }
    //                });
                loginwidget->hide();   //隐藏登陆窗口
                this->show();  //显示新窗口
                return true;
            }
            else
            {
                QMessageBox::critical(this, "错误信息", "登录失败");
                return false;
            }

        }
        else
        QMessageBox::critical(this, "错误信息", "请进行IP配置");
        });
    return false;
}
void MainWindow::showEvent(QShowEvent* event)
{
    QMainWindow::showEvent(event);	// 调用父类的 showEvent

    // 一、标题
    this->setWindowTitle("研究生招生专职招生客户端");

    // 二、状态栏
    // 布局
    QLabel* copyright_label = new QLabel("2019-2023 北林网工技术支持", this);
    QLabel* time_label = new QLabel("", this);
    // time_label->setAlignment(Qt::AlignCenter);	// 时间文字居中

    QHBoxLayout* status_layout = new QHBoxLayout(this);	// 状态布局
    status_layout->addStretch(1);	// 伸缩（弹簧。参数 >0 尽可能占用可用空间，与其他具有相同或更高值的伸缩项成比例
    status_layout->addWidget(copyright_label);
    status_layout->addStretch(1);
    status_layout->addWidget(time_label);

    QWidget* status_widget = new QWidget(this);	// 状态组件，因为 QStatusBar 不支持布局，需要一个中间组件
    status_widget->setLayout(status_layout);

    this->statusBar()->addPermanentWidget(status_widget, 1);
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


    // 三、菜单
    QMenu* configMenu = this->menuBar()->addMenu("配置");
    QMenu* user_action = this->menuBar()->addMenu("管理员");

    // 四、二分屏
   /* QString host = "211.71.149.249";
    QString userName = "cn=Manager,dc=bjfu,dc=edu.cn";
    Widget* left_widget = new Widget(this,host,userName);*/
    //如果已经定义好了UI，在这里会被重写，所以 不能事先定义好UI，
    //需要在这里实例化类，之后调用对象操作。

    QWidget* right_widget = new QWidget(this);

    QSplitter* h_splitter = new QSplitter(Qt::Horizontal, this);	// 水平分裂器
    h_splitter->addWidget(left_widget);
    h_splitter->addWidget(right_widget);
    //不显示完整
    this->setCentralWidget(h_splitter);


    // 设置水平分裂器左右占比：1:3
    // 参数：子部件索引，比例
    int left_index = h_splitter->indexOf(left_widget);	// 0
    int right_index = h_splitter->indexOf(right_widget);	// 1
//    h_splitter->setStretchFactor(left_index, 1);
//    h_splitter->setStretchFactor(right_index, 1);
     //修改：
     QList<int>lstSize;  // 保存宽度（如果是水平分割器）或高度（如果是垂直分割器）
     // 我这里是水平分割器。如果是垂直分割器，请把下面的width()改为height()
     lstSize << 500 << h_splitter->width() - 500;
     h_splitter->setSizes(lstSize);
    // 很奇怪，设置1:8才显示1:3，可能widget和textbrowser组件性质不同
    // 在程序修改不同情况时调整不同的比例

    // 五、按钮
    // 上水平布局
    QHBoxLayout* btn_top_hbox_layout = new QHBoxLayout(this);
    QPushButton* dir_service_btn = new QPushButton(QString("00 目录服务\n功能：编制目录\n范围：校内访问"), this);
    QPushButton* tm_system_btn = new QPushButton(QString("01 推免系统\n功能：推免招生\n范围：存在互联网流量"), this);
    QPushButton* kw_system_btn = new QPushButton(QString("02 考务系统\n功能：统考组织\n范围：校内访问"), this);

    int btn_height = dir_service_btn->height();
    QFont btn_font = dir_service_btn->font();
    btn_font.setPointSize(btn_height * 0.5);	// 通过调整比例，可以根据需要设置字体大小
    dir_service_btn->setFont(btn_font);
    tm_system_btn->setFont(btn_font);
    kw_system_btn->setFont(btn_font);

    dir_service_btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);	// 填充满整个组件
    tm_system_btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    kw_system_btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    btn_top_hbox_layout->addWidget(dir_service_btn);
    btn_top_hbox_layout->addWidget(tm_system_btn);
    btn_top_hbox_layout->addWidget(kw_system_btn);

    // 下水平布局
    QHBoxLayout* btn_bottom_hbox_layout = new QHBoxLayout(this);
    QPushButton* tkzs_tj_btn = new QPushButton(QString("03 统考招生与调剂\n功能：统考招生与调剂\n范围：存在互联网流量"), this);
    QPushButton* bszs_btn = new QPushButton(QString("04 博士招生\n功能：博士招生\n范围：存在互联网流量"), this);
    QPushButton* bd_system_btn = new QPushButton(QString("05 报到系统\n功能：审核报道者身份\n范围：校内访问"), this);

    tkzs_tj_btn->setFont(btn_font);
    bszs_btn->setFont(btn_font);
    bd_system_btn->setFont(btn_font);

    tkzs_tj_btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    bszs_btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    bd_system_btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    btn_bottom_hbox_layout->addWidget(tkzs_tj_btn);
    btn_bottom_hbox_layout->addWidget(bszs_btn);
    btn_bottom_hbox_layout->addWidget(bd_system_btn);

    // 垂直布局
    QVBoxLayout* btn_vbox_layout = new QVBoxLayout(this);
    btn_vbox_layout->addLayout(btn_top_hbox_layout);
    btn_vbox_layout->addLayout(btn_bottom_hbox_layout);

    right_widget->setLayout(btn_vbox_layout);
    return;
    //cn=manager,dc=bjfu,dc=edu.cn    secret
}
