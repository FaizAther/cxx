#ifndef BIKE_H
#define BIKE_H

#include "Commons.hpp"

const char *TYPES[] = { \
    "Suzuki Bandit", \
    "Honda TransAlp", \
    "BMW F 650 GS", \
    "Kawasaki ZZR1400", \
    "Undefined"
};

enum BikeType
{
    SUZ = 0,
    HON = 1,
    BMW = 2,
    KSI = 3,
    XYZ = 4
};

class Bike
{
private:
    ID          id;
    BikeType    _type;

public:
    Bike(ID xid, BikeType x_type): \
    id(xid), _type(x_type)
    {}

    Status      _bstatus  = FRE;

    inline bool
    book()
    {
        return _sbook(_bstatus);
    }

    inline std::string
    show()
    {
        char buf[BUF_SIZ] = {0};
        bzero(buf, BUF_SIZ);
        snprintf(buf, BUF_SIZ, "%s%d%s%s%s%s\n", \
            "Bike:\t\tid=",
            id,
            ",\t\tbstatus=",
            bstatus(),
            ",\t\ttype=",
            type()
        );
        return std::string(buf);
    }

    inline const char *
    type()
    {
        return TYPES[_type];
    }

    inline const char *
    bstatus()
    {
        return _sstatus(_bstatus);
    }

    inline bool
    equal(Bike &check)
    {
        return equal(check.get_id());
    }
    
    inline bool
    equal(ID rid)
    {
        return rid == id;
    }

    inline ID
    get_id()
    {
        return id;
    }

    static std::string
    all_types()
    {
        char buf[BUF_SML];
        bzero(buf, BUF_SML);
        uint32_t wrote = 0;

        wrote += snprintf(buf + wrote, BUF_SML - wrote, "{");
        for (uint8_t bts = SUZ; bts < XYZ + 1; bts++) {
            wrote += snprintf(buf + wrote, BUF_SML - wrote, \
                "%d:\'%s\'", bts, TYPES[bts]);
            if (bts != XYZ) {
                wrote += snprintf(buf + wrote, \
                    BUF_SML - wrote, ", ");
            }

        }
        wrote += snprintf(buf + wrote, BUF_SML - wrote, "}");
        return std::string(buf);
    }
    
    std::string
    finalized()
    {
        char buf[BUF_SML];
        bzero(buf, BUF_SML);
        snprintf(buf, BUF_SML, \
            "Bike {id=%u, name=%s}", \
            id, type());
        return std::string(buf);
    }
};

#endif //BIKE_H