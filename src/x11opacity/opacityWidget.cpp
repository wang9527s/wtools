#include "opacityWidget.h"

#include "windowsctrl.h"
#include "src/tool/wconfig.h"
#include "src/tool/wtool.h"
#include <QDebug>
#include <QCheckBox>
#include <QListWidget>
#include <QSlider>
#include <QVBoxLayout>

opacityWidget::opacityWidget(QWidget *parent) :
    QWidget(parent)
{
    mlistWidget=new QListWidget(this);
    mSlider =new QSlider(Qt::Orientation::Horizontal,this);
    mSlider->setValue(mSlider->maximum());

    QVBoxLayout * pLayout=new QVBoxLayout(this);
    pLayout->addWidget(mlistWidget);
    pLayout->addWidget(mSlider);

    connect(mSlider,&QSlider::sliderReleased,this,&opacityWidget::updateOpacity);
    startTimer(99);

    QJsonObject json=WTool::getJsonFromConfig(WConfig::OpacityCfgPath);
    for (auto key: json.keys()){
        mOpacitys.insert(key,json.value(key).toDouble());
    }
}

void opacityWidget::timerEvent(QTimerEvent *e)
{
     WindowsCtrl::instance()->update();
     if(WindowsCtrl::instance()->isChange()){
         updateList();
         updateOpacity();
     }

     return QWidget::timerEvent(e);
}

void opacityWidget::updateList()
{
    updateChecked();

    mlistWidget->clear();

    //添加item
    QMap<int,QString> wmInfo=WindowsCtrl::instance()->wmInfo();
    for(auto name: wmInfo.values().toSet()){
        QListWidgetItem * pItem=new QListWidgetItem();
        mlistWidget->addItem(pItem);

        QList<int> ids=wmInfo.keys(name);
        QVariant var;
        var.setValue(ids);
        pItem->setData(Qt::UserRole,var);

        QCheckBox * pCheck=new QCheckBox(name);
        pCheck->setChecked(mCheckeds.value(name));

        connect(pCheck,&QCheckBox::stateChanged,this,&opacityWidget::updateChecked);

        mlistWidget->setItemWidget(pItem,pCheck);
    }
}

void opacityWidget::updateOpacity()
{
    double val=mSlider->value()/100.0;

    for( int i=0;i<mlistWidget->count();++i){
        auto pItme = mlistWidget->item(i);
        auto pChecked = qobject_cast<QCheckBox*>(mlistWidget->itemWidget(pItme));
        if(!pChecked)
            continue;

        auto wName=pChecked->text();
        //更新mOpacitys
        if(pChecked->isChecked()){
            mOpacitys.insert(wName,val);
        }

        //设置透明度
        QList<int> winids=pItme->data(Qt::UserRole).value<QList<int>>();
        double opacity=mOpacitys.contains(wName)?mOpacitys[wName]:1.0;
        WindowsCtrl::instance()->setOpacity(winids,qMax(0.1,opacity));
    }

    QJsonObject json;
    for (auto d:mOpacitys.toStdMap()){
        json.insert(d.first,d.second);
    }
    WTool::saveJsonToConfig(json,WConfig::OpacityCfgPath);
}

void opacityWidget::updateChecked()
{
    for (int row=0;row<mlistWidget->count();++row){
        QWidget * p = mlistWidget->itemWidget(mlistWidget->item(row));
        QCheckBox * pc = qobject_cast<QCheckBox*>(p);
        if(pc){
            mCheckeds[pc->text()]=pc->isChecked();
        }
    }
}
