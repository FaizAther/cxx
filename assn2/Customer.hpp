#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "Commons.hpp"

#define CUS_NDATA   5

extern "C" {
#define CUST_DATA(v, w, x, y, z) \
    ((std::string [CUS_NDATA]) { \
        std::string(v), \
		std::string(w), \
		std::string(x), \
		std::string(y), \
		std::string(z) \
	})
}

/* Example 
    Customer cus = Customer( \
        0,
        CUST_DATA( \
            "John", \
            "1 Infinate Loop", \
            "7002", \
            "Cupertino", \
            "+1-000-111-1234" \
        ), \
        LicenseStatus::HAS
    );
*/

const char *CUS_FIELDS[] = { \
    "Name:\t",
    "Street:\t",
    "Postal:\t",
    "City:\t",
    "Number:\t"
};

enum DataType
{
    NAME    = 0,
    STREET  = 1,
    POSTAL  = 2,
    CITY    = 3,
    DIGITS  = 4
};

enum LicenseStatus
{
    HAS = 0,
    NOT = 1,
};

const char *LIS_STATS[] = { \
    "Has License", \
    "Does not have Licence", \
};

class Customer
{
private:
    ID              id;
    std::string     data[CUS_NDATA];

public:
    LicenseStatus   _lstatus = NOT;
    Status          _bstatus = FRE;

    Customer( \
        ID xid, std::string (xdata)[CUS_NDATA], \
        LicenseStatus xlstatus): \
        id(xid), _lstatus(xlstatus)
    {
        for (uint8_t i = 0; i < CUS_NDATA; i++) {
            data[i] = xdata[i];
        }
    }

    inline std::string
    show()
    {
        char buf[BUF_SIZ] = {0};
        uint32_t wrote = 0;

        bzero(buf, BUF_SIZ);
        wrote += snprintf(buf, BUF_SIZ,
            "%s%d%s%s%s%s\n" TAB4 "data={\n", \
            "Customer:\tid=",
            id,
            ",\t\tbstatus=",
            bstatus(),
            ",\t\tlstatus=",
            lstatus()
        );

        for (uint8_t i = 0; i < CUS_NDATA; i++) {
            wrote += \
                snprintf(buf + wrote, BUF_SIZ - wrote, \
                    TAB4 "\t%s%s\n", \
                    type(i), \
                    data[i].c_str() \
                );
        }
        wrote += \
            snprintf(buf + wrote, \
                BUF_SIZ - wrote, TAB4 "}\n" \
            );
        return std::string(buf);
    }

    static inline const char *
    type(uint8_t _type)
    {
        return CUS_FIELDS[_type];
    }

    inline const char *
    bstatus()
    {
        return _sstatus(_bstatus);
    }

    inline const char *
    lstatus()
    {
        return LIS_STATS[_lstatus];
    }

    inline bool
    book()
    {
        return _sbook(_bstatus);
    }

    inline bool
    equal(Customer &check)
    {
        return equal(check.get_id());
    }

    inline bool
    equal(ID cid)
    {
        return cid == id;
    }

    inline ID
    get_id()
    {
        return id;
    }
};

#endif //CUSTOMER_H