#ifndef ENCODING_H
#define ENCODING_H

#include <QString>

extern bool utf8_2_utf8bom(QString path);
extern bool utf8bom_2_utf8(QString path);

#endif // ENCODING_H
