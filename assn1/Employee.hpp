#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
#include <string.h>

class Date;
class Person;

#define BUF_SIZE 		256

#define MAX_HDAY_OLD	32
#define MAX_HDAY_YNG	30

class Date {
public:
	Date(): day(0), month(0), year(0) {};
	Date(uint32_t xday, uint32_t xmonth, uint32_t xyear)
	{
		day		= xday;
		month	= xmonth;
		year	= xyear;
	}

	uint32_t	day;
	uint32_t	month;
	uint32_t	year;

	std::string
	show()
	{
		char buff[BUF_SIZE];
		snprintf(buff, sizeof(buff), "Date{day=%d, month=%d, year=%d}", \
			day, month, year);
		return std::string(buff);
	}

	bool
	isFifty(Date current)
	{
		return current.year - year > 50;
	}
};

Date CURRENT;

enum EmployeeType
{
	HOURLY		= 0,
	SALARIED	= 1,
	MANAGER		= 2,
	BAD			= ~0
};

class Employee
{
public:
	Employee(uint32_t xid, std::string xlastName, std::string xfirstName, \
		uint32_t xday, uint32_t xmonth, uint32_t xyear)
	{
		id 				= xid;
		lastName		= xlastName;
		firstName		= xfirstName;
		dateOfBirth		= Date(xday, xmonth, xyear);
		holidaysTaken	= 0;
		type			= BAD;
		extra_hdays		= dateOfBirth.isFifty(CURRENT);
	}

	Employee(uint32_t xid, std::string xlastName, std::string xfirstName, \
		uint32_t xday, uint32_t xmonth, uint32_t xyear, uint32_t xholidaysTaken)
	{
		id 				= xid;
		lastName		= xlastName;
		firstName		= xfirstName;
		dateOfBirth		= Date(xday, xmonth, xyear);
		holidaysTaken	= xholidaysTaken;
		type			= BAD;
		extra_hdays		= dateOfBirth.isFifty(CURRENT);
	}

	uint32_t		id;
	std::string		lastName;
	std::string		firstName;
	Date			dateOfBirth;
	uint8_t			holidaysTaken;
	EmployeeType	type;
	bool			extra_hdays;
	Date			holidays[MAX_HDAY_OLD];

	uint8_t
	hdays_left()
	{
		return extra_hdays ? \
			MAX_HDAY_OLD - holidaysTaken : \
			MAX_HDAY_YNG - holidaysTaken;
	}

	std::string
	_show_hdays()
	{
		char buff[BUF_SIZE];
		bzero(buff, BUF_SIZE);
		uint32_t wrote = 0;
		for (uint8_t i = 0; i < holidaysTaken; i++) {
			wrote += snprintf(buff + wrote, sizeof(buff) - wrote, \
				"%s, ", holidays[i].show().c_str());
		}
		return std::string(buff);
	}

	void
	add_hday(Date date)
	{
		if (hdays_left() > 0 && \
			date.day != 0 && date.month != 0 && date.year != 0) {
			holidays[holidaysTaken++] = date;
		}
	}

	void
	add_hday(uint32_t xday, uint32_t xmonth, uint32_t xyear)
	{
		if (hdays_left() > 0) {
			holidays[holidaysTaken++] = Date(xday, xmonth, xyear);
		}
	}

	virtual std::string
	_show()
	{
		char buff[BUF_SIZE * 2];
		snprintf(buff, sizeof(buff), \
			"id=%d, firstName=%s, lastName=%s, "
			"holidaysLeft=%d, DOB=%s, \n\t\t\tholidays=%s\n", \
			id, firstName.c_str(), lastName.c_str(), \
			hdays_left(), dateOfBirth.show().c_str(), \
			_show_hdays().c_str() \
		);
		return std::string(buff);
	}

	virtual std::string
	show()
	{
		char buff[BUF_SIZE];
		snprintf(buff, sizeof(buff), \
			"%s{%s}", \
			typeid(Employee).name() + 1, Employee::_show().c_str() \
		);
		return std::string(buff);
	}

	bool
	match(uint8_t which, std::string name)
	{
		if (which == 0) {
			return name == lastName;
		} else {
			return name == firstName;
		}
		return false;
	}

	bool
	match(uint32_t xid)
	{
		return id == xid;
	}
};

class HourlyEmployee: public Employee
{
public:
	HourlyEmployee(uint32_t xid, std::string xlastName, \
		std::string xfirstName, uint32_t xholidaysTaken, \
		uint32_t xday, uint32_t xmonth, uint32_t xyear, \
		uint32_t xhourlyWage, uint32_t xhoursWorked): \
		Employee(xid, xlastName, xfirstName, \
			xday, xmonth, xyear, xholidaysTaken)
	{
		hourlyWage	= xhourlyWage;
		hoursWorked	= xhoursWorked;
		type		= HOURLY;
	}

	HourlyEmployee(uint32_t xid, \
		std::string xlastName, std::string xfirstName, \
		uint32_t xday, uint32_t xmonth, uint32_t xyear): \
		Employee(xid, xlastName, xfirstName, xday, xmonth, xyear)
	{
			hourlyWage	= 0;
			hoursWorked	= 0;
			type		= HOURLY;

	}

	uint32_t	hourlyWage;
	uint32_t	hoursWorked;

	std::string
	_show() 
	{
		char buff[BUF_SIZE];
		snprintf(buff, sizeof(buff), \
			"%s\t\t\t, hourlyWage=%d, hoursWorked=%d", \
			Employee::_show().c_str(), hourlyWage, hoursWorked \
		);
		return std::string(buff);
	}

	std::string
	show()
	{
		char buff[BUF_SIZE];
		snprintf(buff, sizeof(buff), \
			"%s\t{%s}", \
			typeid(HourlyEmployee).name() + 2, \
			HourlyEmployee::_show().c_str() \
		);
		return std::string(buff);
	}
};

class SalariedEmployee: public Employee {
public:
	SalariedEmployee(uint32_t xid, std::string xlastName, \
		std::string xfirstName, uint32_t xholidaysTaken, \
		uint32_t xday, uint32_t xmonth, uint32_t xyear, \
		uint32_t xannualSalary): \
		Employee(xid, xlastName, xfirstName, xday, xmonth, xyear, xholidaysTaken)
	{
		annualSalary	= xannualSalary;
		type			= SALARIED;
	}

	SalariedEmployee(uint32_t xid, std::string xlastName, std::string xfirstName, \
		uint32_t xday, uint32_t xmonth, uint32_t xyear): \
		Employee(xid, xlastName, xfirstName, xday, xmonth, xyear) 
	{
		annualSalary	= 0;
		type			= SALARIED;
	}

	uint32_t	annualSalary;

	std::string
	_show()
	{
		char buff[BUF_SIZE];
		snprintf(buff, sizeof(buff), \
			"%s\t\t\t, annualSalary=%d", \
			Employee::_show().c_str(), annualSalary \
		);
		return std::string(buff);
	}

	std::string
	show()
	{
		char buff[BUF_SIZE];
		snprintf(buff, sizeof(buff), \
			"%s\t{%s}", \
			typeid(SalariedEmployee).name() + 2, SalariedEmployee::_show().c_str() \
		);
		return std::string(buff);
	}
};

class Manager: public Employee
{
public:
	Manager(uint32_t xid, std::string xlastName, \
		std::string xfirstName, uint32_t xholidaysTaken, \
		uint32_t xday, uint32_t xmonth, uint32_t xyear, \
		uint32_t xprofitSharing): \
		Employee(xid, xlastName, xfirstName, xday, xmonth, xyear, xholidaysTaken)
	{
			profitSharing	= xprofitSharing;
			type			= SALARIED;
	}

	Manager(uint32_t xid, std::string xlastName, std::string xfirstName, \
		uint32_t xday, uint32_t xmonth, uint32_t xyear): \
		Employee(xid, xlastName, xfirstName, xday, xmonth, xyear)
	{
			profitSharing	= 0;
			type			= SALARIED;
	}

	uint32_t	profitSharing;

	std::string
	_show()
	{
		char buff[BUF_SIZE];
		snprintf(buff, sizeof(buff), \
			"%s\t\t\t, profitSharing=%d", \
			Employee::_show().c_str(), profitSharing \
		);
		return std::string(buff);
	}

	std::string
	show()
	{
		char buff[BUF_SIZE];
		snprintf(buff, sizeof(buff), \
			"%s\t\t{%s}", \
			typeid(Manager).name() + 1, Manager::_show().c_str() \
		);
		return std::string(buff);
	}
};

#endif //EMPLOYEE_H