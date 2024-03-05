#ifndef FLOWNODE_H
#define FLOWNODE_H

//#include "flowuiarea.h"

#include <QWidget>
#include <QScrollArea>
#include <QDebug>
#include <QString>
#include <QEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QDebug>
#include <QApplication>


#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDrag>


//#include "ui_flownode.h"

namespace Ui {
class FlowNode;
}

class FlowNode : public QWidget
{
    Q_OBJECT

public:
    explicit FlowNode(QWidget *parent = 0);
    FlowNode(QString name, QWidget *parent = 0);
    FlowNode(FlowNode &&node);
    ~FlowNode();

    static void setDraingNode(FlowNode *node);
    void setName(QString str);

    /**
      * @brief mousePressEvent 鼠标按下事件
      * @param ev 鼠标事件对象
      */
    void mousePressEvent(QMouseEvent *ev);

    /**
      * @brief mouseMoveEvent 鼠标移动事件
      * @param ev 鼠标事件对象
      */
    void mouseMoveEvent(QMouseEvent *ev);

    /**
      * @brief dragEnterEvent 拖拽进入事件
      * @param ev 拖拽事件对象
      */
    void dragEnterEvent(QDragEnterEvent *ev);

    /**
      * @brief dragMoveEvent 拖拽移动事件
      * @param ev 拖拽事件对象
      */
    void dragMoveEvent(QDragMoveEvent *ev);

    /**
     * @brief dragLeaveEvent 拖拽离开事件
     * @param ev 拖拽事件对象
     */
    void dragLeaveEvent(QDragLeaveEvent *ev);

    /**
      * @brief dropEvent 放下事件
      * @param ev 放下事件对象
      */
    void dropEvent(QDropEvent *ev);

public:
    Ui::FlowNode *ui;

    static QPoint dragStartCursorPos;
    static QPoint dragStartWidgetPos;

    static FlowNode *dragingNode;
};


#endif // FLOWNODE_H
