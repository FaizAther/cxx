#ifndef BOOKINGS_H
#define BOOKINGS_H

#include "Booking.hpp"

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
        file.open(_filename);
    }

    Bookings(std::string xfilename): \
    bookings(0), _filename(xfilename)
    {
        file.open (_filename);
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

    bool
    add(Bike &bike, Customer &customer)
    {
        std::unique_ptr<Booking> booking = \
            std::unique_ptr<Booking>(new Booking(bookings, bike, customer));
        if (booking.get()->valid()) {
            write(booking.get()->show());
            list.push_back(std::move(booking));
            bookings++;
            return true;
        }
        return false;
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