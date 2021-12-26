#ifndef BOOKING_H
#define BOOKING_H

#include "Bike.hpp"
#include "Customer.hpp"

class Booking {
private:
    ID          id;
    Bike        &_bike;
    Customer    &_customer;
    bool        _valid;

    inline bool
    valid_booking()
    {
        return _bike._bstatus == FRE && \
            _customer._bstatus == FRE && \
            _customer._lstatus == HAS;
    }

    inline bool
    make_booking()
    {
        return _bike.book() && _customer.book();
    }

public:
    Booking(ID xid, Bike &xbike, Customer &xcustomer):
    id(xid), _bike(xbike), _customer(xcustomer)
    {
        _valid = valid_booking();
        if (_valid) {
            make_booking();
        }
    }

    inline bool
    valid()
    {
        return _valid;
    }

    inline std::string
    show()
    {
        if (!valid())
            return std::string("Booking: INVALID");

        char buf[BUF_SIZ] = {0};
        bzero(buf, BUF_SIZ);

        snprintf(buf, BUF_SIZ, \
            "Booking{id=%d}:\n\t%s\t%s", \
            id, _bike.show().c_str(),
            _customer.show().c_str()
        );
        return std::string(buf);
    }
};

#endif //BOOKING_H