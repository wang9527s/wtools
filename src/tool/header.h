#ifndef HEADER_H
#define HEADER_H

#include <QScopedPointer>

#define DECLARE_SINGLETON(Class) \
public: \
    static Class* instance() \
    { \
        static Class * pIns = nullptr; \
        if (pIns == nullptr) { \
            pIns = new Class; \
        } \
        return pIns; \
    }

#endif // HEADER_H
