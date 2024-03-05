#ifndef FLOWUIAREA_H
#define FLOWUIAREA_H
/*
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
*/

#include "flownode.h"
/*
 * 需要手动提升类以便接收事件
 */
class FlowUiArea : public QScrollArea
{
public:
    FlowUiArea(QWidget *parent = 0);

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
    static QPoint dragStartCursorPos;

private:
    QPoint dragStartPos;

};


#endif // FLOWUIAREA_H
