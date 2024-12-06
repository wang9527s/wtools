#include "encoding.h"

#include <QByteArray>
#include <QTextCodec>
#include <QDebug>
#include <QFile>

void log_format(QString type, QString tip, QString path)
{
    QString msg = QString("%1%2%3")
                      .arg(QString(type).leftJustified(10, ' '))
                      .arg(QString(tip).leftJustified(18, ' '))
                      .arg(path);
    qInfo() << msg;
    // QByteArray localMsg = msg.toLocal8Bit();
    // fprintf(stdout, "%s\n", localMsg.constData());
    // fflush(stdout);
}

bool isUtf8Bom(const QByteArray &data)
{
    return data.startsWith("\xEF\xBB\xBF");
}

// 如果格式是 utf8或者utf8bom 则返回true
bool isUtf8(const QByteArray &ba)
{
    QTextCodec::ConverterState state;

    // 尝试使用 UTF-8 编码解码字节数组
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    codec->toUnicode(ba.constData(), ba.size(), &state);

    // 如果发现有无效字符，则返回 false
    return (state.invalidChars == 0);
}

// 如果格式是 utf8或者utf8bom 则返回true （QTextCodec的底层实现是否也是这个？）
bool isUtf8_2(const QByteArray &data)
{
    int i = 0;
    while (i < data.size()) {
        if ((data[i] & 0x80) == 0x00) {
            i += 1;
        }
        else if ((data[i] & 0xE0) == 0xC0) {
            if ((i + 1 >= data.size()) || ((data[i + 1] & 0xC0) != 0x80))
                return false;
            i += 2;
        }
        else if ((data[i] & 0xF0) == 0xE0) {
            if ((i + 2 >= data.size()) || ((data[i + 1] & 0xC0) != 0x80) ||
                ((data[i + 2] & 0xC0) != 0x80))
                return false;
            i += 3;
        }
        else if ((data[i] & 0xF8) == 0xF0) {
            if ((i + 3 >= data.size()) || ((data[i + 1] & 0xC0) != 0x80) ||
                ((data[i + 2] & 0xC0) != 0x80) || ((data[i + 3] & 0xC0) != 0x80))
                return false;
            i += 4;
        }
        else {
            return false;
        }
    }
    return true;
}

bool utf8_2_utf8bom(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadWrite)) {
        log_format("failed", u8"文件打开失败", path);
        return false;
    }

    QByteArray data = file.readAll();
    if (!isUtf8(data)) {
        log_format("ignore", u8"不支持的格式", path);
        return false;
    }

    if (isUtf8Bom(data)) {
        log_format("ignore", u8"不需要转换", path);
        file.close();
        return true;
    }
    else {
        // 添加bom
        file.resize(0);
        file.write("\xEF\xBB\xBF");
        file.write(data);

        file.close();
        log_format("success", u8"utf8 -> utf8bom", path);
        return true;
    }

    log_format("error", "", path);
    file.close();
    return false;
}

bool utf8bom_2_utf8(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadWrite)) {
        log_format("failed", u8"文件打开失败", path);
        return false;
    }

    QByteArray data = file.readAll();
    if (!isUtf8(data)) {
        log_format("ignore", u8"不支持的格式", path);
        return false;
    }

    // 检查文件是否以 UTF-8 BOM 开头
    if (data.startsWith("\xEF\xBB\xBF")) {
        // 删除 UTF-8 BOM
        data.remove(0, 3);

        // 将删除 BOM 后的数据写回文件
        file.resize(0);
        if (file.write(data) == -1) {
            log_format("failed", u8"写入文件失败", path);
            file.close();
            return false;
        }

        log_format("success", u8"utf8bom -> utf8 转换成功", path);
        file.close();
        return true;
    }
    else {
        log_format("ignore", u8"不需要转换", path);
        file.close();
        return true;
    }

    log_format("error", "", path);
    file.close();
    return false;
}

// void testFileEncoding(const QString &filePath) {
//     // 打开文件
//     QFile file(filePath);
//     if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//         qWarning() << "Failed to open file:" << filePath;
//         return ;
//     }

//     // 读取文件的开头部分，最多 1024 个字符
//     QByteArray data = file.readAll();
//     file.close();

//     // qInfo() << isUtf8_2(data);
//     qInfo() << isUtf8(data);
//     qInfo() << data.mid(0,16);
//     return ;
// }
