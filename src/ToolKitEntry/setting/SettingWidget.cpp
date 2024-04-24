#include "SettingWidget.h"
#include <QCheckBox>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QFileDialog>

#include "../utils/wtool.h"
#include "CallExternal.h"

SettingWidget::SettingWidget(QWidget *parent)
    : QWidget(parent)
    , cfg(new SettingConfig())
{
    // 相册/动态壁纸
    QWidget *w_album = new QWidget;
    QLineEdit *lineedit_album = new QLineEdit;
    QPushButton *btn_album = new QPushButton(u8"更新");
    QHBoxLayout *pl_album = new QHBoxLayout(w_album);
    pl_album->addWidget(new QLabel(u8"相册路径: ", this));
    pl_album->addWidget(lineedit_album);
    pl_album->addWidget(btn_album);

    // 截屏
    QWidget *w_ss = new QWidget;
    QLabel *label_ss = new QLabel(u8"快捷键");
    QLineEdit *edit_ss = new QLineEdit;
    QHBoxLayout *pl_ss = new QHBoxLayout(w_ss);
    pl_ss->addWidget(label_ss);
    pl_ss->addWidget(edit_ss);

    QCheckBox *check_autoStart = new QCheckBox(u8"开机自启");
    QCheckBox *check_ScreenShot = new QCheckBox(u8"启用截屏");
    QCheckBox *check_album = new QCheckBox(u8"启用动态桌面");

    // 整体布局
    QGridLayout *pl = new QGridLayout(this);
    pl->addWidget(check_autoStart, 0, 0);

    pl->addWidget(check_ScreenShot, 1, 0);
    pl->addWidget(w_ss, 1, 1);

    pl->addWidget(check_album, 2, 0);
    pl->addWidget(w_album, 2, 1);

    connect(check_autoStart, &QCheckBox::stateChanged, [=](int status) {
        bool checked = Qt::Checked == status;

        WTool::setAutoStart(checked);
        cfg->d.autoStart = checked;
        cfg->save();
    });
    connect(check_ScreenShot, &QCheckBox::stateChanged, [=](int status) {
        bool checked = Qt::Checked == status;
        if (checked) {
            CallExternal::instance()->startAlbum();
        }
        else {
            CallExternal::instance()->exitAlbum();
        }
        // WTool::setAutoStart(checked);
        cfg->d.screen_shot.enable = checked;
        cfg->save();
    });
    connect(check_album, &QCheckBox::stateChanged, [=](int status) {
        bool checked = Qt::Checked == status;

        // WTool::setAutoStart(checked);
        cfg->d.album.enable = checked;
        cfg->save();
    });

    connect(btn_album, &QPushButton::clicked, this, [=] {
        QString selectedDir = QFileDialog::getExistingDirectory(
            nullptr, "选择文件夹", QDir::homePath(), QFileDialog::ShowDirsOnly);
        if (!selectedDir.isEmpty()) {
            lineedit_album->setText(selectedDir);
            cfg->d.album.dir = selectedDir;
            cfg->save();
        }
    });

    check_autoStart->setChecked(cfg->d.autoStart);
    check_ScreenShot->setChecked(cfg->d.screen_shot.enable);
    check_album->setChecked(cfg->d.album.enable);

    edit_ss->setPlaceholderText(cfg->d.screen_shot.hotkey.toString());
    lineedit_album->setText(cfg->d.album.dir);
}

SettingWidget::~SettingWidget()
{
    delete cfg;
}
