#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QObject>
#include <QSystemTrayIcon>

class SettingWidget;
class QHotkey;
class QProcess;
class SystemTray : public QObject
{
    Q_OBJECT
public:
    SystemTray(QObject *parent = nullptr);
    ~SystemTray();

private:
    QSystemTrayIcon *pIcon;
    QMenu *menu;
    SettingWidget *set;

    QHotkey *hotkey;
};

#endif // SYSTEMTRAY_H
