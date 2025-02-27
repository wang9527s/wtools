﻿#ifndef RSSWIDGET_H
#define RSSWIDGET_H

#include <QWidget>
#include <QJsonObject>
#include "src/tool/header.h"

class QLineEdit;
class QPushButton;
class QTextEdit;
class RssWidget : public QWidget
{
    Q_OBJECT
    DECLARE_SINGLETON(RssWidget)

private:
    explicit RssWidget(QWidget *parent = nullptr);

private slots:
    void onCreateBiibiliFollowingOPML();
    void onUpdateBilibiliOPML();

private:
    void save_json();

private:
    QLineEdit *mRssService;
    QLineEdit *mBilibiliVmid;

    QPushButton *mCreateFollowingOpml;
    QPushButton *mUpdateOpml;

    QJsonObject mJsonConfig;
    QTextEdit *pEdit;

    QLineEdit *pOpalPath;
};

#endif // RSSWIDGET_H
