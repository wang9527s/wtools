#ifndef RSSUPDATEOPML_H
#define RSSUPDATEOPML_H

#include <QObject>

class RSSUpdateOPML
{

public:
    // 更新自己的rss
    static void updateBilibili(const QString & rssServer,const QString & vmid,const QString & opmlPath);
    // 将他人哔哩哔哩的关注列表生成rss
    static void createFollowingsOPML(const QString & rssServer,const QString & vmid);

private:
    static QStringList getFollowings(const QString & rssServer,const QString & vmid);
};

#endif // RSSUPDATEOPML_H
