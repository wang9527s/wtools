#include "drawerwidget.h"

#include "src/tool/wtool.h"

#include <QDebug>
#include <QIcon>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QDir>
#include <QSvgRenderer>
#include <QPainter>

DrawerWidget::DrawerWidget(QWidget *parent)
    : QWidget(parent)
{
    mName = new QLineEdit(this);
    mName->setPlaceholderText("请输入图标的名字");
    QPushButton *showIcon = new QPushButton("保存Icon", this);
    QHBoxLayout *pH1 = new QHBoxLayout;
    pH1->addWidget(mName);
    pH1->addWidget(showIcon);

    mSvgPath = new QLineEdit(this);
    mSvgPath->setPlaceholderText("请输入Svg图片的路径");
    QPushButton *showSvg = new QPushButton("显示Svg", this);
    QHBoxLayout *pH2 = new QHBoxLayout;
    pH2->addWidget(mSvgPath);
    pH2->addWidget(showSvg);

    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->addLayout(pH1);
    pLayout->addLayout(pH2);
    pLayout->addStretch();

    connect(mName, &QLineEdit::returnPressed, this, &DrawerWidget::onShowIcon);
    connect(showIcon, &QPushButton::clicked, this, &DrawerWidget::onShowIcon);
    connect(showSvg, &QPushButton::clicked, this, &DrawerWidget::onShowSvg);
}

void DrawerWidget::onShowIcon()
{
    const QString name = mName->text();
    /* tip
       QIcon::fromTheme("/opt/apps/x11opacity-wang/window.svg")和QIcon::fromTheme("computer")都可以正确加载图标
       QIcon("/opt/apps/x11opacity-wang/window.svg")可以正确加载图标，QIcon("computer")加载图标失败
    */
    QIcon icon = QIcon::fromTheme(name);
    if (icon.isNull()) {
        return;
    }

    QPixmap pix;
    const int s = 8;
    const int l[s] = {64, 16, 18, 24, 32, 96, 128, 256};
    for (int i(0); i != s; ++i) {
        pix = icon.pixmap(QSize(l[i], l[i]));
        if (!pix.isNull()) {
            QString path = QDir::homePath() + "/Desktop/wtool-tmp/";
            QDir d;
            d.mkpath(path);
            qInfo() << pix.save(path + QString("%1_%2x%3.png").arg(name).arg(l[i]).arg(l[i]));
        }
    }
}

void DrawerWidget::onShowSvg()
{
    if (m_showlabel == nullptr) {
        m_showlabel = new QLabel;
    }

    QString logopath = mSvgPath->text();
    m_showlabel->setPixmap(WTool::loadPixmap(logopath));
    m_showlabel->show();
}
