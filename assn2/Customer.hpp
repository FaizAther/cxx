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
    "HasLicense", \
    "NoLicence", \
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

/*
	Customer:	id=0,		bstatus=(1,Booked),		lstatus=(0,HasLicense)
			data={
				Name:	Al-Ghazali
				Street:	Enlightenment St.
				Postal:	21012
				City:	Persia
				Number:	+44321
			}
*/
    static Customer *
    unshow(FILE *fbuf)
    {
        ID xid = ~0;
        uint8_t ret = ~0;

        char data[CUS_NDATA][BUF_SML];

        for (uint8_t i = 0; i < CUS_NDATA; i++) {
            bzero(data[i], BUF_SML);
        }

        ret = fscanf(fbuf, "\tCustomer:\t\tid=%u,\t\t" \
            "bstatus=(1,Booked),\t\tlstatus=(0,HasLicense)\n" \
            TAB4 "data={\n" \
            TAB4 "\tName:\t%s\n" \
            TAB4 "\tStreet:\t%s\n" \
            TAB4 "\tPostal:\t%s\n" \
            TAB4 "\tCity:\t%s\n" \
            TAB4 "\tNumber:\t%s\n\t\t\t}", \
            &xid, data[0], data[1], \
            data[2], data[3], data[4]);
        
        if (ret != 6)
            return NULL;

        return new Customer(xid, \
            CUST_DATA(data[0], data[1], data[2], \
                data[3], data[4]), \
            LicenseStatus(HAS));
    }

    inline std::string
    show()
    {
        char buf[BUF_SIZ] = {0};
        uint32_t wrote = 0;

        bzero(buf, BUF_SIZ);
        wrote += snprintf(buf, BUF_SIZ,
            "%s%d%s%d,%s)%s%d,%s)\n" TAB4 "data={\n", \
            "Customer:\tid=",
            id,
            ",\t\tbstatus=(",
            _bstatus,
            bstatus(),
            ",\t\tlstatus=(",
            _lstatus,
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

    std::string
    finalized()
    {
        char buf[BUF_SML];
        bzero(buf, BUF_SML);
        snprintf(buf, BUF_SML, \
            "Customer {id=%u, name=%s}", \
            id, data[NAME].c_str());
        return std::string(buf);
    }
};

#endif //CUSTOMER_H