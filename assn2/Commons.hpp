#ifndef COMMONS_H
#define COMMONS_H

#include <string>
#include <string.h>

typedef uint32_t    ID;

#define BUF_SML     128
#define BUF_SIZ     (BUF_SML * 4)
#define BUF_BIG     (BUF_SIZ * 4)

#define TAB4        "\t\t\t"

const char *STATS[] = { \
    "Available",
    "Booked",
    "Processed",
    "Undefined"
};

enum Status
{
    FRE = 0,
    BKD = 1,
    DNE = 2,
    WXY = 3
};

inline bool
_sbook(Status &_status)
{
    if (_status == FRE) {
        _status = BKD;
        return true;
    }
    return false;
}

inline const char *
_sstatus(Status _status)
{
    return STATS[_status];
}

#endif //COMMONS_H