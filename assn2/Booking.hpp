#ifndef BOOKING_H
#define BOOKING_H

#include "Bike.hpp"
#include "Customer.hpp"

class Booking {
private:
    ID          id;
    std::shared_ptr<Bike>        _bike;
    std::shared_ptr<Customer>    _customer;
    bool        _valid;

    inline bool
    valid_booking()
    {
        return _bike.get()->_bstatus == FRE && \
            _customer.get()->_bstatus == FRE && \
            _customer.get()->_lstatus == HAS;
    }

    inline bool
    make_booking()
    {
        return _bike.get()->book() && _customer.get()->book();
    }

public:
    Booking(ID xid, Bike &xbike, Customer &xcustomer):
    id(xid), _bike(std::make_shared<Bike>(xbike)), \
    _customer(std::make_shared<Customer>(xcustomer))
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
            return std::string("Booking: INVALID\n");

        char buf[BUF_SIZ] = {0};
        bzero(buf, BUF_SIZ);

        snprintf(buf, BUF_SIZ, \
            "Booking{id=%d}:\n\t%s\t%s", \
            id, _bike.get()->show().c_str(),
            _customer.get()->show().c_str()
        );
        return std::string(buf);
    }

    inline bool
    equal(ID eid)
    {
        return id == eid;
    }

    std::string
    finalized()
    {
        char buf[BUF_SML];
        bzero(buf, BUF_SML);
        snprintf(buf, BUF_SML, "Booking{id=%u} with %s %s Finalized", \
            id, _customer.get()->finalized().c_str(), \
            _bike.get()->finalized().c_str());
        return std::string(buf);
    }

};

#endif //BOOKING_H