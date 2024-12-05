#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include <QWidget>
#include <QDebug>

#include "../global_setting/SettingConfig.h"

class SettingWidget : public QWidget
{
    Q_OBJECT

public:
    SettingWidget(QWidget *parent = nullptr);

    ~SettingWidget();

    SettingConfig *cfg;
};
#endif // SETTINGWIDGET_H
