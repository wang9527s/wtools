#include "iconwidget.h"

#include <QDebug>
#include <QIcon>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QDir>

IconWidget::IconWidget(QWidget *parent)
    : QWidget(parent)
{
    mName = new QLineEdit(this);
    mName->setPlaceholderText("请输入图标的名字");
    QPushButton *showIcon = new QPushButton("显示Icon", this);

    QHBoxLayout *pLayout = new QHBoxLayout(this);
    pLayout->addWidget(mName);
    pLayout->addWidget(showIcon);

    connect(mName, &QLineEdit::returnPressed, this, &IconWidget::onShowIcon);
    connect(showIcon, &QPushButton::clicked, this, &IconWidget::onShowIcon);
}

void IconWidget::onShowIcon()
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
