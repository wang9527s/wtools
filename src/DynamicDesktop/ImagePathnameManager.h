#ifndef IMAGEPATHNAMEMANAGER_H
#define IMAGEPATHNAMEMANAGER_H

#include <QRandomGenerator>
#include <QFileInfo>
#include <QStringList>
#include <QDebug>

#define g_random_bg_path() ImagePathnameManager::instance()->pathname()

class ImagePathnameManager
{
public:
    static ImagePathnameManager *instance()
    {
        static ImagePathnameManager *pIns = nullptr;
        if (pIns == nullptr) {
            pIns = new ImagePathnameManager;
        }
        return pIns;
    }

    void init_rcc()
    {
        pathnames.append(":/img/default.JPG");
        QString pathname;
        for (int i = 0; i < 10; i++) {
            pathname = QString(":/bgs/h%1.JPG").arg(i, 2, 10, QLatin1Char('0'));

            if (QFileInfo::exists(pathname)) {
                pathnames.append(pathname);
            }
        }
    }

    QString pathname()
    {
        // [a, b)
        int idx = QRandomGenerator::global()->bounded(0, pathnames.size());
        return pathnames.at(idx);
    }
    QString pathname(int max_index)
    {
        // [a, b)
        if (max_index >= pathnames.size()) {
            return pathname();
        }
        return pathnames.at(max_index);
    }

public:
    QStringList pathnames;
};

#endif // IMAGEPATHNAMEMANAGER_H