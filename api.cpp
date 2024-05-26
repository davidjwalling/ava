#include "api.h"

char* tokstrx(char* s, const char* d, const char* x, char** p)
{
    char* t, * f;
    const char* e;
    if (p) {
        if (!s)
            s = *p;
        *p = nullptr;
    }
    if (!s || !(*s))
        return nullptr;
    t = s;
    if (d) {
        for (; *t; t++) {
            for (e = d; *e && *e != *t; e++);
            if (!(*e))
                break;
        }
        if (!(*t))
            return nullptr;
    }
    f = t;
    if (x) {
        for (; *t; t++) {
            for (e = x; *e && *t != *e; e++);
            if (*e) {
                *t = 0;
                if (p)
                    *p = t + 1;
                break;
            }
        }
    }
    return f;
}
