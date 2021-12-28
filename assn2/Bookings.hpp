#ifndef BOOKINGS_H
#define BOOKINGS_H

#include "Booking.hpp"

#include <stdio.h>
#include <fstream>
#include <vector>

#define DEFAULT_FILE    "bookings.txt"

class Bookings
{
private:
    std::vector<std::unique_ptr<Booking> >    list;
    std::ofstream           file;
    ID                      bookings;
    std::string             _filename;

public:
    Bookings():
    bookings(0), _filename(DEFAULT_FILE)
    {
        read(_filename.c_str());
        file.open(_filename);
        dump();
    }

    Bookings(std::string xfilename): \
    bookings(0), _filename(xfilename)
    {
        read(_filename.c_str());
        file.open(_filename);
        dump();
    }

    void
    read(const char *xname)
    {
        FILE *fbuf = fopen(xname, "r");
        if (fbuf == NULL)
            return;
        char buf[BUF_BIG];
        uint8_t nscan = 0;
        ID xid = ~0;
        char *ret = NULL;

        do {
            bzero(buf, BUF_BIG);
            ret = fgets(buf, BUF_BIG, fbuf);
            if (ret == NULL)
                return;
        
            nscan = sscanf(buf, "Booking{id=%u}:\n", &xid);
            if (nscan == 1) {
                Bike *bik = Bike::unshow(fbuf);
                if (bik == NULL)
                    return;
                Customer *cus = Customer::unshow(fbuf);
                if (cus == NULL)
                    return;
                std::unique_ptr<Booking> booking = \
                    std::unique_ptr<Booking>(make_booking(xid, bookings, *bik, *cus));
                add(booking);
                //add(*bik, *cus);
            } else {
                nscan = fscanf(fbuf, "FinalizedBooking{id=%u}:\n", &xid);
                if (nscan == 1)
                    remove(xid);
            }
        } while (ret != NULL);
        fclose(fbuf);
    }

    bool
    remove(ID rid)
    {
        for (ID pos = 0; pos < list.size(); pos++) {
            if (list[pos].get()->equal(rid)) {
                write(list[pos].get()->finalized());
                list.erase(list.begin() + pos);
                return true;
            }
        }
        return false;
    }

    void
    dump()
    {
        for (auto &bk: list) {
            write(bk.get()->show());
        }
    }

    bool
    add(std::unique_ptr<Booking> &booking)
    {
        if (booking.get()->valid()) {
            write(booking.get()->show());
            list.push_back(std::move(booking));
            bookings++;
            return true;
        }
        return false;
    }

    static Booking *
    make_booking(ID xid, ID &max, Bike &bike, Customer &customer)
    {
        if (xid > max) {
            max = xid;
        }
        return new Booking(xid, bike, customer);
    }

    bool
    add(Bike &bike, Customer &customer)
    {
        std::unique_ptr<Booking> booking = \
            std::unique_ptr<Booking>(make_booking(bookings, bookings, bike, customer));
        return add(booking);
    }

    void
    show()
    {
        uint32_t wrote = 0, i = 0;
        char buf[BUF_BIG] = {0};
        bzero(buf, BUF_BIG);

        wrote += snprintf(buf + wrote, BUF_BIG - wrote, \
            "BOOKINGS____{%zu}\n", list.size());
        printf("%s", buf);
        for (std::unique_ptr<Booking> &booking: list) {
            wrote = 0;
            bzero(buf, BUF_BIG);
            wrote += snprintf(buf + wrote, BUF_BIG - wrote, \
                "pos@[%d]=>\t%s", i , booking.get()->show().c_str());
            printf("%s", buf);
            i++;
        }
    }

    inline void
    write(const std::string &buf)
    {
        file << buf;
    }
};

#endif //BOOKINGS_H