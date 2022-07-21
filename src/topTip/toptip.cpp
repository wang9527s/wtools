#include "toptip.h"

#include <QDesktopWidget>
#include <QApplication>
#include <QPainter>
#include <QTimer>
#include <QTime>
#include <QDebug>
#include "src/tool/wconfig.h"
#include <QFile>
#include "src/tool/wtool.h"
#include <QJsonObject>
#include <QPixmap>
#include <QBitmap>

QList<QColor> colors = QList<QColor>()
                       << QColor(95, 158, 160)
                       //        <<QColor(	255,240,245)
                       << QColor(30, 144, 255)
                       //        <<QColor(	220,20,60)
                       << QColor(216, 191, 216) << QColor(139, 0, 139) << QColor(148, 0, 211)
                       << QColor(0, 0, 255) << QColor(65, 105, 225) << QColor(135, 206, 250)
                       << QColor(0, 191, 255);

const int appHeight = 42;  // 软件高度
const int mainHeight = 18; // 文字色块高度
const qreal opacity = 0.66;

void TopTip::paintEvent(QPaintEvent *evt)
{
    drawPixmap(mPix);

    QPainter pp(this);
    QString path(WConfig::ConfigDir + "/ff.png");
    qInfo() << mPix.isNull();
    mPix.save(path);
    pp.drawPixmap(rect(), mPix);

    return QFrame::paintEvent(evt);
}

void TopTip::mouseReleaseEvent(QMouseEvent *evt)
{
    setVisible(false);
    mShowTimer->start(1000 * 60 * 5); // 5min
    QFrame::mouseReleaseEvent(evt);
}

void TopTip::drawPixmap(QPixmap &pixmap)
{
    //    pixmap = pixmap.scaled(width(),height());
    pixmap.fill(Qt::transparent); // 清空图片 否则 预留的空白区域会有残留
    QPainter p(&pixmap);
    p.setPen(Qt::black);
    //    p.fillRect(rect(),QColor(255,0,0,100));  setAttribute(Qt::WA_TranslucentBackground);
    //    p.fillRect(rect(),Qt::transparent);

    // 反锯齿
    p.setRenderHint(QPainter::Antialiasing, true);

    QLinearGradient linear;
    for (auto block : mBlocks) {
        linear.setStart(block.rt.topLeft());
        linear.setFinalStop(block.rt.topRight());
        linear.setColorAt(0, block.tlColor);
        linear.setColorAt(1, block.brColor);

        if (mPosition == ButtonRight) {
            block.rt.setTop(height() - mainHeight);
        }
        else if (mPosition == TopRight) {
            block.rt.setBottom(mainHeight);
        }

        p.fillRect(block.rt, QBrush(linear));

        p.setPen(Qt::black);
        p.drawText(block.rt, Qt::AlignCenter, block.text);
        qInfo() << block.rt << block.text;
    }

    //小气泡
    const int bubbleSharpCornersHeight = 5;
    const int bubbleWidth = 64;
    const int bubbleHeight = height() - mainHeight - bubbleSharpCornersHeight;
    const int timeLinePosx = hToPointX(QTime::currentTime().hour(), QTime::currentTime().minute());
    QRectF bubbleRect;
    if (mPosition == ButtonRight) {
        bubbleRect.setX(timeLinePosx - bubbleWidth / 2);
        bubbleRect.setY(0);
    }
    else if (mPosition == TopRight) {
        bubbleRect.setX(timeLinePosx - bubbleWidth / 2);
        bubbleRect.setY(height() - bubbleHeight);
    }
    bubbleRect.setWidth(bubbleWidth); // Tips: setWidth和setHeight，必须在setX和setY之后
    bubbleRect.setHeight(bubbleHeight);

    p.setBrush(Qt::yellow);
    p.setPen(Qt::yellow);
    p.drawRoundRect(bubbleRect, 8, 8);
    QPolygon triangle;

    if (mPosition == ButtonRight) {
        triangle.setPoints(3,
                           timeLinePosx - 5,
                           bubbleHeight,
                           timeLinePosx + 5,
                           bubbleHeight,
                           timeLinePosx,
                           bubbleHeight + bubbleSharpCornersHeight); //三点坐标
    }
    else if (mPosition == TopRight) {
        triangle.setPoints(3,
                           timeLinePosx,
                           bubbleHeight,
                           timeLinePosx - 5,
                           bubbleHeight + bubbleSharpCornersHeight,
                           timeLinePosx + 5,
                           bubbleHeight + bubbleSharpCornersHeight); //三点坐标
    }
    p.drawPolygon(triangle); //画三角形

    QString bubbleText = getButtleText();
    p.setPen(Qt::black);
    p.drawText(bubbleRect, Qt::AlignCenter, bubbleText);

    // 时间显示
    const int timeWidth = 80;
    QRect timeRt;
    if (mPosition == ButtonRight) {
        timeRt.setX(width() - timeWidth);
        timeRt.setY(height() - mainHeight);
    }
    else if (mPosition == TopRight) {
        timeRt.setX(width() - timeWidth);
        timeRt.setY(0);
    }
    timeRt.setWidth(80);
    timeRt.setHeight(mainHeight);

    p.fillRect(timeRt, Qt::white);
    p.drawText(timeRt, Qt::AlignCenter, QTime::currentTime().toString("hh:mm:ss"));
}

QString TopTip::getButtleText()
{
    QString buttleText("Hello");

    QTime t = QTime::currentTime();
    int timeLinePosx = hToPointX(t.hour(), t.minute());
    for (auto block : mBlocks) {
        if (block.rt.contains(QPoint(timeLinePosx, height()))) {
            buttleText = block.text;
        }
    }

    // 应该休息了
    if (t.hour() == 10 || t.hour() == 14 || t.hour() == 15 || t.hour() == 16) {
        if (t.minute() >= 25 && t.minute() <= 35)
            buttleText = "休息一下";
    }

    if (t.hour() == 10) {
        if (t.minute() >= 35 && t.minute() <= 50)
            buttleText = "点外卖";
    }

    // 加班
    if (t.hour() >= 18) {
        buttleText = "加班快乐";
    }

    static QString judgeStr;
    if (/*judgeStr != "" &&*/ judgeStr != buttleText) {
        WTool::sendNotice(buttleText);
        judgeStr = buttleText;
    }

    return buttleText;
}

qreal TopTip::hToPointX(int h, int min)
{
    const int hourScaleWidth = 90;
    qreal scalecount = h + min / 60.0 - 8;
    // 大于18点 移动到12点的位置
    return (scalecount > 10.0 ? 4 : scalecount) * hourScaleWidth;
}

void TopTip::initBlock()
{
    int coloridx = 0;
    TopTipBlock block;
    block.brColor = Qt::red;

    // 子项
    block.rt = QRectF(QPointF(hToPointX(8), 0), QPointF(hToPointX(9, 30), height()));
    block.tlColor = block.brColor;
    block.brColor = Qt::green;
    block.text = "查资料";
    mBlocks << block;

    block.rt = QRectF(QPointF(hToPointX(9, 30), 0), QPointF(hToPointX(10), height()));
    block.tlColor = block.brColor;
    block.brColor = Qt::blue;
    block.text = "email";
    mBlocks << block;

    block.rt = QRectF(QPointF(hToPointX(10), 0), QPointF(hToPointX(11, 30), height()));
    block.tlColor = block.brColor;
    block.brColor = Qt::yellow;
    block.text = "work";
    mBlocks << block;

    block.rt = QRectF(QPointF(hToPointX(11, 30), 0), QPointF(hToPointX(13), height()));
    block.tlColor = block.brColor;
    block.brColor = Qt::darkGreen;
    block.text = "Rest";
    mBlocks << block;

    block.rt = QRectF(QPointF(hToPointX(13), 0), QPointF(hToPointX(17), height()));
    block.tlColor = block.brColor;
    block.brColor = Qt::gray;
    block.text = "work";
    mBlocks << block;

    block.rt = QRectF(QPointF(hToPointX(17), 0), QPointF(hToPointX(18), height()));
    block.tlColor = block.brColor;
    block.brColor = Qt::yellow;
    block.text = "日报";
    mBlocks << block;
}

void TopTip::readConfig()
{
    if (!QFile::exists(WConfig::topTipCfgPath())) {
        QJsonObject js;
        WTool::saveJsonToConfig(js, WConfig::topTipCfgPath());
    }

    QJsonObject js = WTool::getJsonFromConfig(WConfig::topTipCfgPath());
    if (js.contains("position")) {
        mPosition = TopTipPosition(js.value("position").toInt());
    }
    qInfo() << "positon:" << mPosition;
}

TopTip::TopTip(QWidget *parent)
    : QFrame(parent)
{
    setWindowFlag(Qt::WindowStaysOnTopHint);    // 置顶
    setWindowFlag(Qt::FramelessWindowHint);     // 无标题栏
    setWindowFlags(Qt::Tool | windowFlags());   // 不在任务栏上显示
    setAttribute(Qt::WA_TranslucentBackground); //背景全透明
    //    setAttribute(Qt::WA_TransparentForMouseEvents);
    setWindowOpacity(opacity);

    QTimer *pTimer = new QTimer(this);
    connect(pTimer, &QTimer::timeout, this, [=] { QWidget::update(); });
    pTimer->start(1000);
    QWidget::update();

    mShowTimer = new QTimer(this);
    mShowTimer->setSingleShot(true);
    connect(mShowTimer, &QTimer::timeout, this, [=] { QWidget::show(); });

    QFont ft = font();
    ft.setPixelSize(16);
    setFont(ft);

    readConfig();

    setFixedSize(1024 - 40, appHeight);
    QRect screenRect = qApp->desktop()->screenGeometry(qApp->desktop()->numScreens() - 1);
    qInfo() << screenRect;

    QPoint pos; // 默认 ButtonRight
    pos.setX(screenRect.x() + screenRect.width() - width());
    pos.setY(screenRect.y() + screenRect.height() - height());
    if (mPosition == TopRight) {
        pos.setY(screenRect.y());
    }
    move(pos);

    setVisible(true);

    initBlock();
    mPix = QPixmap(width(), height());
}
