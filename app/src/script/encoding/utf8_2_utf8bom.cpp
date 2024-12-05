
#include <iostream>
#include <filesystem>
#include <vector>
#include <regex>
#include <QDebug>
#include "encoding.h"

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    if (argc <= 1) {
        qInfo() << u8"请输入路径";
        return 0;
    }
    qInfo() << argv[0] << argv[1];
    fs::path inputPath = argv[1];
    fs::path absolutePath = std::filesystem::absolute(inputPath);
    std::string path = absolutePath.string();
    std::cout << "absolutePath" << path << std::endl;

    std::list<std::string> pathnames;
    if (fs::is_directory(path)) {

        // 只是更改 hpp h 和 cpp文件
        std::regex pattern(".*\\.(hpp|cpp|h)$");
        for (const auto &entry : fs::recursive_directory_iterator(path)) {
            if (fs::is_regular_file(entry)) {
                std::string filename = entry.path().string();

                if (std::regex_match(filename, pattern)) {
                    pathnames.push_back(entry.path().string());
                }
            }
        }
    }
    else if (fs::is_regular_file(path)) {
        pathnames.push_back(path);
    }

    for (const std::string &pathname : pathnames) {
        utf8_2_utf8bom(QString::fromStdString(pathname));
    }

    return 0;
}
