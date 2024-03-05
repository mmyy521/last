#include "flowuiarea.h"

QPoint FlowUiArea::dragStartCursorPos(0,0);

FlowUiArea::FlowUiArea(QWidget *parent) : QScrollArea(parent)
{
    qDebug() << "ui create";
    setAcceptDrops(true);
}

/**
  * @brief mousePressEvent 鼠标按下事件
  * @param ev 鼠标事件对象
  */
void FlowUiArea::mousePressEvent(QMouseEvent *ev) {
    if (!(ev->buttons() & Qt::LeftButton))
        return;
    FlowUiArea::dragStartCursorPos = ev->globalPos();
}

/**
  * @brief mouseMoveEvent 鼠标移动事件
  * @param ev 鼠标事件对象
  */
void FlowUiArea::mouseMoveEvent(QMouseEvent *ev) {
    if ((ev->pos() - FlowUiArea::dragStartCursorPos).manhattanLength()
            < QApplication::startDragDistance())
        return;

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    mimeData->setText("TODO");
    drag->setMimeData(mimeData);
    drag->exec();
}

/**
  * @brief dragEnterEvent 拖拽进入事件
  * @param ev 拖拽事件对象
  */
void FlowUiArea::dragEnterEvent(QDragEnterEvent *ev) {
    ev->accept();
}

/**
  * @brief dragMoveEvent 拖拽移动事件
  * @param ev 拖拽事件对象
  */
void FlowUiArea::dragMoveEvent(QDragMoveEvent *ev) {

}

/**
 * @brief dragLeaveEvent 拖拽离开事件
 * @param ev 拖拽事件对象
 */
void FlowUiArea::dragLeaveEvent(QDragLeaveEvent *ev){

}

/**
  * @brief dropEvent 放下事件
  * @param ev 放下事件对象
  */
void FlowUiArea::dropEvent(QDropEvent *ev) {
    QString text = ev->mimeData()->text();

    if(FlowNode::dragingNode)
        FlowNode::dragingNode->move(ev->pos() - FlowNode::dragStartCursorPos);

    if(FlowNode::dragingNode) qDebug() << "curPos：" << FlowNode::dragingNode->pos();

    FlowNode::dragingNode = nullptr;
}
