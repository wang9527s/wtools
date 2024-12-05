#ifndef WATERMARKS_H
#define WATERMARKS_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QApplication>
#include <QScreen>
#include <QStyleOption>
#include "../utils/instance_header.h"

class WaterMarks : public QWidget
{
    Q_OBJECT
    DECLARE_SINGLETON(WaterMarks)
public:
    QRect screen_rt;

    WaterMarks(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setWindowFlags(Qt::WindowTransparentForInput | Qt::Tool | Qt::X11BypassWindowManagerHint |
                       Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        setAttribute(Qt::WA_TransparentForMouseEvents);
        setAttribute(Qt::WA_TranslucentBackground);

        // setAttribute(Qt::WA_StyledBackground, true);
        setStyleSheet("background:rgb(200, 200, 200, 200);"
                      "border-radius: 5px;");

        // mTitle = QString(u8"生命一天天减少，你却无所谓");
        // mContent =
        //         // "setWindowFlags(Qt::WindowTransparentForInput\n"
        //         // "| Qt::Tool\n"
        //         // "| Qt::X11BypassWindowManagerHint"
        //         // "| Qt::FramelessWindowHint"
        //         // "| Qt::WindowStaysOnTopHint);\n"
        //         // "setAttribute(Qt::WA_TransparentForMouseEvents);"

        //         "static int text_height(const QFont &ft, const QString &line, const int w) {\n"
        //         "    QFontMetrics fm(ft);\n"
        //         "    int flag = Qt::AlignLeft | Qt::TextWordWrap;\n"
        //         "    return fm.boundingRect(QRect(0, 0, 1000, 0), flag, line).height();\n"
        //         "}"
        //         ;

        setFixedWidth(210);
        screen_rt = qApp->primaryScreen()->geometry();
    }
    void setText(QString title, QString content)
    {
        if (title == mTitle && content == mContent)
            return;

        mTitle = title;
        mContent = content;
        update();
    }
    static int text_height(const QFont &ft, const QString &line, const int w)
    {
        QFontMetrics fm(ft);
        int flag = Qt::AlignLeft | Qt::TextWordWrap;
        return fm.boundingRect(QRect(0, 0, w, 0), flag, line).height();
    }
    void paintEvent(QPaintEvent *evt)
    {
        QPainter p(this);
        QStyleOption option;
        option.initFrom(this);
        style()->drawPrimitive(QStyle::PE_Widget, &option, &p, this);

        const int border = 9;
        const int text_w = width() - 2 * border;

        p.setPen(QPen(Qt::yellow));
        QFont ft = p.font();
        // ft.setFamily("Arial");
        ft.setPixelSize(25);

        QPoint draw_tl(border, border);
        p.setFont(ft);

        int text_h = text_height(ft, mTitle, text_w);
        QRect draw_rt(draw_tl.x(), draw_tl.y(), text_w, text_h);
        p.drawText(draw_rt, Qt::TextWordWrap, mTitle);
        // p.drawRect(draw_rt);

        ft.setPixelSize(16);
        p.setFont(ft);

        const int spacing = 6;

        draw_tl.setY(draw_tl.y() + text_h + spacing);
        text_h = text_height(ft, mContent, text_w);
        draw_rt = QRect(draw_tl.x(), draw_tl.y(), text_w, text_h);
        p.drawText(draw_rt, Qt::TextWordWrap, mContent);
        // p.drawRect(draw_rt);

        // QPen pen;
        // pen.setWidth(2);
        // pen.setColor(Qt::red);
        // p.setPen(pen);
        // p.drawRect(rect());

        setFixedHeight(draw_rt.bottom() + border);

        QPoint screen_br = screen_rt.bottomRight();
        move(screen_br.x() - width() - 20, screen_br.y() - height() - 260);

        return QWidget::paintEvent(evt);
    };

    QString mTitle;
    QString mContent;
};

#endif // WATERMARKS_H
