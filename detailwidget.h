#ifndef DETAILWIDGET_H
#define DETAILWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
namespace Ui {
class DetailWidget;
}

class DetailWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DetailWidget(QWidget *parent = 0);
    ~DetailWidget();
    void setDetail();
    QString cn="";
    QString sn="";
    QString ou="";
    QString description="";
    QString uid="";
    QString uidNumber="";
    QString gidNumber="";
    QString homeDirectory="";
    QString title="";
    QString jid="";
    QString host="";
    QString l="";
    QString o="";
    QString initials="";
    QString labeledURI="";
    QString pager="";

private:
    Ui::DetailWidget *ui;
};

#endif // DETAILWIDGET_H
