#ifndef MANAGEMENT_H
#define MANAGEMENT_H

#include "Bookings.hpp"

#include <iostream>
#include <vector>

#define BAD_ID      (ID)~0

class Management
{
private:
    std::vector<Bike>       bikes;
    std::vector<Customer>   customers;
    Bookings                bookings;

    ID  c_bikes;
    ID  c_customers;

    static Bike
    scan_bikedata(ID cid)
    {
        char line[BUF_SML];/*, \
            *ret = NULL;*/
        uint32_t btype = ~0;
        BikeType raw = XYZ;

        bzero(line, BUF_SML);

        printf("Model %s?: ", Bike::all_types().c_str());
        /*ret = */
        fgets(line, BUF_SML, stdin);
        sscanf(line, "%d", &btype);
        if (btype <= XYZ && btype >= SUZ) {
            raw = BikeType(btype);
        }

        return Bike(cid, raw);
    }

    static Customer
    scan_cusdata(ID cid)
    {
        char buf[BUF_SML][CUS_NDATA], \
            line[BUF_SML], \
            *ret = NULL;
        std::string data[CUS_NDATA];
        LicenseStatus lstat = LicenseStatus::NOT;

        for (uint8_t i = 0; i < CUS_NDATA; i++) {
            bzero(line, BUF_SML);
            printf("%s?: ", Customer::type(i));
            ret = fgets(line, BUF_SML, stdin);
            bzero(buf[i], BUF_SML);
            ret = (char *)memccpy(buf[i], line, '\n', BUF_SML);
            *(ret - 1) = '\0';
            data[i] = std::string(buf[i]);
        }
        printf("License {y,n}?: ");
        bzero(line, BUF_SML);
        ret = fgets(line, BUF_SML, stdin);
        if(line[0] == 'y') {
            lstat = LicenseStatus::HAS;
        }
        return Customer(cid, data, lstat);
    }

    bool
    scan_bookingdata(ID (store)[2])
    {
        char buf[BUF_SML];
        int ret = 0;
        
        printf("Booking: {BikeID or x:exit}:? ");
        bzero(buf, BUF_SML);
        fgets(buf, BUF_SML, stdin);
        if (buf[0] == 'x')
            return false;
        ret = sscanf(buf, "%d", (store + 0));
        if (ret != 1)
            return false;

        printf("Booking: {CustomerID or x:exit}:? ");
        bzero(buf, BUF_SML);
        fgets(buf, BUF_SML, stdin);
        if (buf[0] == 'x')
            return false;
        ret = sscanf(buf, "%d", (store + 1));
        return ret == 1;
    }

public:
    Management(): c_bikes(0), c_customers(0)
    {}

    inline bool
    add(Bike &abike, Customer &acus)
    {
        return bookings.add(abike, acus);
    }

    inline bool
    search(Bike &sbike)
    {
        return bfind_remove(sbike.get_id(), false);
    }

    inline ID
    _bfind_remove(ID rid, bool sure)
    {
        for(ID i = 0; i < bikes.size(); i++) {
            if (bikes.at(i).equal(rid)) {
                if (sure) {
                    bikes.erase(bikes.begin() + i);
                }
                return i;
            }
        }
        return BAD_ID;
    }

    inline bool
    bfind_remove(ID rid, bool sure)
    {
        return _bfind_remove(rid, sure) != BAD_ID;
    }

    inline bool
    add(Bike &abike)
    {
        if (search(abike)) {
            return false;
        }
        bikes.push_back(abike);
        c_bikes++;
        return true;
    }


    inline bool
    search(Customer &scus)
    {
        return cfind_remove(scus.get_id(), false);
    }

    inline ID
    _cfind_remove(ID rid, bool sure)
    {
        for(ID i = 0; i < customers.size(); i++) {
            if (customers.at(i).equal(rid)) {
                if (sure) {
                    customers.erase(customers.begin() + i);
                }
                return i;
            }
        }
        return BAD_ID;
    }

    inline bool
    cfind_remove(ID rid, bool sure)
    {
        return _cfind_remove(rid, sure) != BAD_ID;
    }

    inline bool
    add(Customer &acus)
    {
        if (search(acus)) {
            return false;
        }
        customers.push_back(acus);
        c_customers++;
        return true;
    }

    void
    show_bikes()
    {
        char buf[BUF_BIG];
        uint32_t wrote = 0, i = 0;

        bzero(buf, BUF_SIZ);
        wrote += snprintf(buf + wrote, \
            BUF_SIZ - wrote, "BIKES___{%zu}\n", bikes.size());
        printf("%s", buf);
        for (Bike ibike: bikes) {
            bzero(buf, BUF_SIZ);
            wrote = 0;
            wrote += snprintf(buf + wrote, BUF_SIZ - wrote, \
                "pos@[%d]=>%s", i, ibike.show().c_str());
            printf("%s", buf);
            i++;
        }
    }

    void
    show_customers()
    {
        char buf[BUF_BIG];
        uint32_t wrote = 0, i = 0;

        bzero(buf, BUF_BIG);
        wrote += snprintf(buf + wrote, BUF_BIG - wrote, \
            "CUSTOMERS___{%zu}\n", customers.size());
        printf("%s", buf);
        for (Customer icus: customers) {
            bzero(buf, BUF_BIG);
            wrote = 0;
            wrote += snprintf(buf + wrote, BUF_BIG - wrote, \
                "pos@[%d]=>\n%s", i, icus.show().c_str());
            printf("%s", buf);
            i++;
        }
    }

    inline void
    show_bookings()
    {
        bookings.show();
    }

    inline void
    show()
    {
        show_bikes();
        show_customers();
        show_bookings();
    }

    void
    make_bike()
    {
        Bike bike = scan_bikedata(c_bikes);
        add(bike);
    }

    void
    make_customer()
    {
        Customer cus = scan_cusdata(c_customers);
        add(cus);
    }

    bool
    make_booking()
    {
        ID store[2];
        if (!scan_bookingdata(store)) {
            return false;
        }
        uint32_t bpos = _bfind_remove(store[0], false);
        uint32_t cpos = _cfind_remove(store[1], false);

        if (bpos == BAD_ID || cpos == BAD_ID || \
            !bookings.add(bikes.at(bpos), customers.at(cpos))) {
            return false;
        }
        cfind_remove(store[0], true);
        bfind_remove(store[1], true);
        return true;
    }

    inline bool
    remove_booking(ID rid)
    {
        return bookings.remove(rid);
    }
};

#endif //MANAGEMENT_H
