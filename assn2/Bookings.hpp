#ifndef BOOKINGS_H
#define BOOKINGS_H

#include "Booking.hpp"

#include <fstream>
#include <vector>

#define DEFAULT_FILE    "bookings.txt"

class Bookings
{
private:
    std::vector<Booking>    list;
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

    inline bool
    add(Bike &bike, Customer &customer)
    {
        Booking booking = \
            Booking(bookings, bike, customer);
        return add(booking);
    }

    inline bool
    add(Booking &booking)
    {
        if (booking.valid()) {
            list.push_back(booking);
            write(booking.show());
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
        for (Booking bkng: list) {
            wrote = 0;
            bzero(buf, BUF_BIG);
            wrote += snprintf(buf + wrote, BUF_BIG - wrote, \
                "pos@[%d]=>\t%s", i , bkng.show().c_str());
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