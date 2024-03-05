#include "detailwidget.h"
//#include "ui_detailwidget.h"
#include<QDebug>
#include<QImage>
//细节展开
DetailWidget::DetailWidget(QWidget *parent) :
    QWidget(parent)
    //ui(new Ui::DetailWidget)
{
    //ui->setupUi(this);
}

DetailWidget::~DetailWidget()
{
    //delete ui;
}

//void DetailWidget::setDetail()
//{
//    ui->cnLabel->setText(cn);
//    if(cn=="")
//    {
//        ui->cnTitle->deleteLater();
//        ui->cnLabel->deleteLater();
//        ui->snTitle->deleteLater();
//        ui->phoneTitle->deleteLater();
//        ui->mailTitle->deleteLater();
//        ui->titleTitle->deleteLater();
//        ui->photoTitle->deleteLater();
//        ui->snLabel->deleteLater();
//        ui->mobileLabel->deleteLater();
//        ui->mailLabel->deleteLater();
//        ui->titleLabel->deleteLater();
//        ui->uidLabel->deleteLater();
//        ui->photoLabel->deleteLater();
//        ui->uidTitle->deleteLater();
//        ui->uidLabel->deleteLater();
//        ui->ouLabel->setText(ou);
//        ui->widget_2->deleteLater();
//    }
//    else if(member!="")
//    {
//        ui->snTitle->deleteLater();
//        ui->phoneTitle->deleteLater();
//        ui->mailTitle->deleteLater();
//        ui->titleTitle->deleteLater();
//        ui->photoTitle->deleteLater();
//        ui->snLabel->deleteLater();
//        ui->mobileLabel->deleteLater();
//        ui->mailLabel->deleteLater();
//        ui->titleLabel->deleteLater();
//        ui->uidLabel->deleteLater();
//        ui->photoLabel->deleteLater();
//        ui->uidTitle->deleteLater();
//        ui->uidLabel->deleteLater();
//        ui->ouTitle->deleteLater();
//        ui->ouLabel->deleteLater();
//    }
//    else{
//        ui->ouTitle->setVisible(false);
//        ui->ouLabel->setVisible(false);
//        ui->cnLabel->setText(cn);
//        ui->snLabel->setText(sn);
//        ui->mobileLabel->setText(mobile);
//        ui->mailLabel->setText(mail);
//        ui->titleLabel->setText(title);
//        ui->uidLabel->setText(uid);
//        //base64转为图片
//        QByteArray ba = QByteArray::fromBase64(photo.toLatin1());
//        QImage testImage;
//        testImage.loadFromData(ba);
//        ui->photoLabel->setMinimumWidth(500);
//        ui->photoLabel->setMinimumHeight(500);
//        ui->photoLabel->setPixmap(QPixmap::fromImage(testImage).scaled(ui->photoLabel->size()));
//    }
//}
void DetailWidget::setDetail(){
    setWindowTitle("详细信息");
    this->resize(500, 500);
    // 创建一个字体对象
    QFont font;
    // 设置字体大小为12
    font.setPointSize(12);
    setFont(font);

    QVBoxLayout *layout = new QVBoxLayout(this);

    // 检查并显示非空成员及其值
    if (!cn.isEmpty()) {
        QLabel *labelCn = new QLabel("cn: " + cn, this);
        layout->addWidget(labelCn);
    }

    if (!sn.isEmpty()) {
        QLabel *labelSn = new QLabel("sn: " + sn, this);
        layout->addWidget(labelSn);
    }

    if (!ou.isEmpty()) {
        QLabel *labelOu = new QLabel("Organization Unit: " + ou, this);
        layout->addWidget(labelOu);
    }

    if (!description.isEmpty()) {
        QLabel *labelDesc = new QLabel("description: " + description, this);
        layout->addWidget(labelDesc);
    }
    if (!uid.isEmpty()) {
        QLabel *labelDesc = new QLabel("uid: " + uid, this);
        layout->addWidget(labelDesc);
    }
    if (!uidNumber.isEmpty()) {
        QLabel *labelDesc = new QLabel("uidNumber: " + uidNumber, this);
        layout->addWidget(labelDesc);
    }
    if (!gidNumber.isEmpty()) {
        QLabel *labelDesc = new QLabel("gidNumber: " + gidNumber, this);
        layout->addWidget(labelDesc);
    }
    if (!homeDirectory.isEmpty()) {
        QLabel *labelDesc = new QLabel("homeDirectory: " + homeDirectory, this);
        layout->addWidget(labelDesc);
    }
    if (!title.isEmpty()) {
        QLabel *labelDesc = new QLabel("title: " + title, this);
        layout->addWidget(labelDesc);
    }
    if (!jid.isEmpty()) {
        QLabel *labelDesc = new QLabel("jid: " + jid, this);
        layout->addWidget(labelDesc);
    }
    if (!host.isEmpty()) {
        QLabel *labelDesc = new QLabel("host: " + host, this);
        layout->addWidget(labelDesc);
    }
    if (!l.isEmpty()) {
        QLabel *labelDesc = new QLabel("l: " + l, this);
        layout->addWidget(labelDesc);
    }
    if (!o.isEmpty()) {
        QLabel *labelDesc = new QLabel("o: " + o, this);
        layout->addWidget(labelDesc);
    }
    if (!initials.isEmpty()) {
        QLabel *labelDesc = new QLabel("initials: " + initials, this);
        layout->addWidget(labelDesc);
    }
    if (!labeledURI.isEmpty()) {
        QLabel *labelDesc = new QLabel("labeledURI: " + labeledURI, this);
        layout->addWidget(labelDesc);
    }
    if (!pager.isEmpty()) {
        QLabel *labelDesc = new QLabel("pager: " + pager, this);
        layout->addWidget(labelDesc);
    }
}
