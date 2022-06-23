#include "iconwidget.h"
#include "ui_iconwidget.h"

#include "src/tool/wtool.h"

#include <QDebug>
#include <QIcon>

IconWidget::IconWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IconWidget)
{
    ui->setupUi(this);
    connect(ui->path,&QLineEdit::returnPressed,this,&IconWidget::on_show_clicked);
}

IconWidget::~IconWidget()
{
    delete ui;
}

void IconWidget::on_show_clicked()
{
    const QString path = ui->path->text();
    ui->pix->setText("");

    QPixmap pix = WTool::getPixmapFromIcon(path);
    if (pix.isNull()){
        ui->pix->setText("该icon不存在");
        return;
    }

    ui->pix->setPixmap(pix);
}
