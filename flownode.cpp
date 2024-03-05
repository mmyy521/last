#include "flownode.h"

QPoint FlowNode::dragStartCursorPos(0,0);
QPoint FlowNode::dragStartWidgetPos(0,0);
FlowNode * FlowNode::dragingNode = nullptr;

FlowNode::FlowNode(QWidget *parent) :
    QWidget(parent), ui(new Ui::FlowNode) {
    static int x = 0, y = 0;
    ui->setupUi(this);
    setAcceptDrops(true);
    /* TODO: 默认生成的位置可以有更好的控制 */
    move(x, y);
    show();

    x += 140;
}

FlowNode::FlowNode(QString name, QWidget *parent) :
    FlowNode(parent) {
    ui->label->setText(name);
}

FlowNode::FlowNode(FlowNode &&node): ui(node.ui) {
    node.ui = nullptr;
    qDebug() << "移动构造调用";
}

void FlowNode::setDraingNode(FlowNode *node) {
    FlowNode::dragingNode = node;
}

FlowNode::~FlowNode()
{
    delete ui;
}

void FlowNode::setName(QString str) {
    ui->label->setText(str);
}

/**
  * @brief mousePressEvent 鼠标按下事件
  * @param ev 鼠标事件对象
  */
void FlowNode::mousePressEvent(QMouseEvent *ev) {
    if (!(ev->buttons() & Qt::LeftButton))
        return;
    dragStartCursorPos = ev->pos();
    dragStartWidgetPos = this->pos();

    FlowNode::setDraingNode(this);
}

/**
  * @brief mouseMoveEvent 鼠标移动事件
  * @param ev 鼠标事件对象
  */
void FlowNode::mouseMoveEvent(QMouseEvent *ev) {
    if ((ev->pos() - dragStartCursorPos).manhattanLength()
            < QApplication::startDragDistance())
        return;

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    mimeData->setText("Child Wieget 拖拽操作");
    drag->setMimeData(mimeData);

    qDebug() << "拖拽操作开始";
    drag->exec();
    qDebug() << "拖拽操作开结束";
}

/**
  * @brief dragEnterEvent 拖拽进入事件
  * @param ev 拖拽事件对象
  */
void FlowNode::dragEnterEvent(QDragEnterEvent *ev) {
    static int i1 = 0;
    ev->accept();

    qDebug() << __func__ << i1++;
}

/**
  * @brief dragMoveEvent 拖拽移动事件
  * @param ev 拖拽事件对象
  */
void FlowNode::dragMoveEvent(QDragMoveEvent *ev) {
    static int i2 = 0;
    qDebug()  << "FlowNodeArea ---- " << __func__ << i2++;
}

/**
 * @brief dragLeaveEvent 拖拽离开事件
 * @param ev 拖拽事件对象
 */
void FlowNode::dragLeaveEvent(QDragLeaveEvent *ev){
    static int i3 = 0;
    qDebug() << __func__ << i3++;
}

/**
  * @brief dropEvent 放下事件
  * @param ev 放下事件对象
  */
void FlowNode::dropEvent(QDropEvent *ev) {
    QString text = ev->mimeData()->text();
    if(FlowNode::dragingNode)
        FlowNode::dragingNode->move(
                    ev->pos() +
                    ( FlowNode::dragStartWidgetPos -
                      FlowNode::dragStartCursorPos )
                    );
    FlowNode::dragingNode = nullptr;
    qDebug() << "flowNode  --- " << text;
}
