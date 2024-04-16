#ifndef INSTANCE_HEADER_H
#define INSTANCE_HEADER_H

#define DECLARE_SINGLETON(Class)                                                                   \
public:                                                                                            \
    static Class *instance()                                                                       \
    {                                                                                              \
        static Class *pIns = nullptr;                                                              \
        if (pIns == nullptr) {                                                                     \
            pIns = new Class;                                                                      \
        }                                                                                          \
        return pIns;                                                                               \
    }

#endif // INSTANCE_HEADER_H
