#include "opacityWidget.h"

#include "windowsctrl.h"
#include "src/tool/wconfig.h"
#include "src/tool/wtool.h"
#include <QDebug>
#include <QCheckBox>
#include <QListWidget>
#include <QSlider>
#include <QVBoxLayout>

opacityWidget::opacityWidget(QWidget *parent)
    : QWidget(parent)
{
    mlistWidget = new QListWidget(this);
    mSlider = new QSlider(Qt::Orientation::Horizontal, this);
    mSlider->setValue(mSlider->maximum());

    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->addWidget(mlistWidget);
    pLayout->addWidget(mSlider);

    connect(mSlider, &QSlider::sliderReleased, this, &opacityWidget::onSliderChanged);
    startTimer(99);

    QJsonObject json = WTool::getJsonFromConfig(WConfig::opacityCfgPath());
    for (auto key : json.keys()) {
        mOpacitys.insert(key, json.value(key).toDouble());
    }
}

QListWidgetItem *opacityWidget::item(QString text)
{
    for (int row = 0; row < mlistWidget->count(); ++row) {
        QListWidgetItem * pItem = mlistWidget->item(row);
        QWidget * p =mlistWidget->itemWidget(pItem);

        if (/*p && */text == qobject_cast<QCheckBox *>(p)->text()) {
            return pItem;
        }
    }
    return nullptr;
}

void opacityWidget::timerEvent(QTimerEvent *e)
{
    auto wminfos = WindowsCtrl::instance()->winInfo();
    // update list widget
    for (WindowInfo wm : wminfos) {
        QString text = wm.title;
        QVariant var;
        var.setValue(wm.wids);

        if (wm.status == WindowInfo::Add) {
            QListWidgetItem *pItem = new QListWidgetItem();
            mlistWidget->addItem(pItem);
            mlistWidget->setItemWidget(pItem, new QCheckBox(text));

            pItem->setData(Qt::UserRole, var);
        }
        else if (wm.status == WindowInfo::Remove){

            QListWidgetItem * pItem = item(text);
            if (pItem) {
                mlistWidget->removeItemWidget(pItem);
                delete pItem;
            }
        }
        else if (wm.status == WindowInfo::Change){

            QListWidgetItem * pItem = item(text);
            if (pItem)
                pItem->setData(Qt::UserRole, var);
        }
    }

    // set opacity
    for (WindowInfo wm : wminfos) {
        if (wm.status == WindowInfo::Remove)
            continue;

        QString text = wm.title;
        if (mOpacitys.contains(text) && mOpacitys[text] < 0.92) {
            WindowsCtrl::instance()->setOpacity(wm.wids, mOpacitys[text]);
        }
    }

    return QWidget::timerEvent(e);
}

void opacityWidget::onSliderChanged()
{
    double val = mSlider->value() / 100.0;

    for (int i = 0; i < mlistWidget->count(); ++i) {
        auto pItme = mlistWidget->item(i);
        auto pChecked = qobject_cast<QCheckBox *>(mlistWidget->itemWidget(pItme));
        if (!pChecked || !pChecked->isChecked())
            continue;

        QString name = pChecked->text();
        mOpacitys[name] = val;

        //设置透明度
        QList<int> winids = pItme->data(Qt::UserRole).value<QList<int>>();
        WindowsCtrl::instance()->setOpacity(winids, val);
    }

    QJsonObject json;
    for (auto d : mOpacitys.toStdMap()) {
        json.insert(d.first, d.second);
    }
    WTool::saveJsonToConfig(json, WConfig::opacityCfgPath());
}
