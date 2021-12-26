#include <iostream>
#include <vector>
#include <memory>
#include <time.h>

#include "Employee.hpp"

uint32_t POS 			= 0;
uint32_t SPACES[500] 	= {!0};

static inline void
___show(Employee *emp)
{
	printf("%s\n", emp->show().c_str());
}

static inline void
__show(uint32_t i, Employee *emp)
{
	printf("[%d] => ", i);
	printf("%s\n", emp->show().c_str());
}

static inline void
_show(uint32_t i, std::unique_ptr<Employee> &elem)
{
	__show(i, elem.get());
}

void
show(std::vector<std::unique_ptr<Employee> > &employees)
{
	printf("%s: start\n", __func__);
	int i = 0;
	printf("SIZE = %zu\n", employees.size());
	for(uint32_t e = 0; e < employees.size(); e++) {
		_show(i, employees.at(e));
		i++;
	}
	printf("%s: end\n", __func__);
}

Date
get_date()
{
	unsigned short int day = ~0, month = ~0;
	uint32_t year = ~0;

	printf("[DD MM YYYY]: ");
	scanf("%hd %hd %u", &day, &month, &year);

	return Date(day, month, year);
}

std::string
get_name()
{
	printf(" name: ");
	std::string name;
	std::cin >> name;
	return name;
}

void
create(EmployeeType which, \
	std::vector<std::unique_ptr<Employee> > &employees, uint32_t *var)
{
	printf("%s: start\n", __func__);
	Date date;
	const char *l_name = NULL, *f_name = NULL;

	if (employees.size() > 500)
		return;

	printf("Last");
	l_name = get_name().c_str();

	printf("First");
	f_name = get_name().c_str();

	date = get_date();

	uint32_t pos = POS;
	switch(which) {
		case MANAGER:
			employees.push_back( \
				std::unique_ptr<Manager>( \
				new Manager(pos, l_name, f_name, 0, \
				date.day, date.month, date.year, \
				var[0]))
			);
			break;
		case HOURLY:
			employees.push_back( \
				std::unique_ptr<HourlyEmployee>( \
				new HourlyEmployee( \
				pos, l_name, f_name, 0, \
				date.day, date.month, date.year, \
				var[0], var[1])) \
			);
			break;
		case SALARIED:
			employees.push_back( \
				std::unique_ptr<SalariedEmployee>( \
				new SalariedEmployee( \
				pos, l_name, f_name, 0, \
				date.day, date.month, date.year, \
				var[0])) \
			);
			break;
		case BAD:
			return;
			break;
	}
	POS++;
	printf("Added %s\n", employees[employees.size() - 1].get()->show().c_str());
	printf("%s: end\n", __func__);
}

void
remove(uint32_t id, std::vector<std::unique_ptr<Employee> > &employees)
{
	printf("%s: p(id=%d) start\n", __func__, id);
	uint32_t i = 0, rem = ~0;

	for(uint32_t e = 0; e < employees.size(); e++) {
		if (employees.at(e).get()->match(id)) {
			rem = id;
		}
	}
	if (rem != ~0) {
		employees.erase(employees.begin() + rem);
	}
	for (i = 0; i < 500; i++) {
		if (SPACES[i] == !0) {
			SPACES[i] = rem;
			printf("REM([%d]) => %d\n", rem, id);
		}
	}
	printf("%s: p(id=%d) end\n", __func__, id);
}

void
search(std::vector<std::unique_ptr<Employee> > &employees)
{
	unsigned short int which = 0, id = 0;
	uint32_t i = 0, rem = ~0;
	const char *name = NULL;

	printf("Select: [0:id, 1:last name, 2:first name] {default=0}: ");
	scanf("%hu", &which);

	printf("Enter ");
	switch (which) {
		case 2:
			name = get_name().c_str();
			break;
		default:
			printf(" id: ");
			scanf("%hu", &id);
			break;
	}

	printf("Searching selection=%d, id=%hu, name=%s...\n", which, id, name);
	for(uint32_t e = 0; e < employees.size(); e++) {
		if (employees.at(e)->match(id)) {
			_show(i, employees.at(e));
		}
		i++;
	}
}

void
add_holiday(std::vector<std::unique_ptr<Employee> > &employees)
{
	unsigned short int id = 0, i = 0;
	Employee *emp = NULL;
	Date date = Date(!0, !0, !0);

	printf("Enter id: ");
	scanf("%hd", &id);

	for(uint32_t e = 0; e < employees.size(); e++) {
		if (employees.at(e)->match(id)) {
			emp = employees.at(e).get();
			break;
		}
		i++;
	}

	while (emp->hdays_left() > 0 && \
		date.day != 0 && date.month != 0 && date.year != 0) {
		__show(i, emp);
		printf("Give: [0 0 0] to exit or ");
		date = get_date();
		emp->add_hday(date);
	}
}

void
add(std::vector<std::unique_ptr<Employee> > &employees)
{
	printf("%s: start\n", __func__);
	char which = 0, \
		buff[BUF_SIZE] = {0};
	uint32_t var[2] = {0};
	EmployeeType type = BAD;

	while (employees.size() < 500) {
		which = 0;
		bzero(buff, BUF_SIZE);
		type = BAD;
		printf("Enter [h: hourly, m: manager, s: salary] or x to exit: ");
bad_in:
		fgets(buff, BUF_SIZE, stdin);
		sscanf(buff, " %c", &which);
		switch (which) {
			case 'x':
				goto bad;
			case 'h':
				type = HOURLY;
				bzero(buff, BUF_SIZE);
				printf("Please input [hourlyWage hoursWorked]: ");
				fgets(buff, BUF_SIZE, stdin);
				sscanf(buff, "%u %u", var + 0, var + 1);
				break;
			case 'm':
				type = MANAGER;
				bzero(buff, BUF_SIZE);
				printf("Please input [profitSharing]: ");
				fgets(buff, BUF_SIZE, stdin);
				sscanf(buff, "%u", var + 0);
				break;
			case 's':
				type = SALARIED;
				bzero(buff, BUF_SIZE);
				printf("Please input [annualSalary]: ");
				fgets(buff, BUF_SIZE, stdin);
				sscanf(buff, "%u", var + 0);
				break;
			default:
				goto bad_in;
		}
		create(type, employees, var);
	}
bad:
	printf("%s: end\n", __func__);
}

void
reactor_loop(std::vector<std::unique_ptr<Employee> > &employees)
{
	printf("%s: start\n", __func__);
	char which = 0, \
		buff[BUF_SIZE] = {0}, \
		val = ~0;
	uint32_t id = !0;

	while (true) {
		which = 0;
		bzero(buff, BUF_SIZE);
		printf("Enter [s: show, q: search, a: add, d: delete, h: holiday] or x to exit: ");
bad_in:
		fgets(buff, BUF_SIZE, stdin);
		sscanf(buff, " %c", &which);
		switch (which) {
			case 'x':
				goto bad;
			case 's':
				show(employees);
				break;
			case 'q':
				search(employees);
				break;
			case 'a':
				add(employees);
				break;
			case 'd':
				printf("ID to delete: ");
				val = getchar();
				sscanf(&val, "%u", &id);
				remove(id, employees);
				break;
			case 'h':
				add_holiday(employees);
				break;
			default:
				goto bad_in;
		}
	}
bad:
	printf("%s: end\n", __func__);
}

int
main(void)
{
	std::vector<std::unique_ptr<Employee> > employees;

	time_t 		rawtime;
	struct tm 	*timeinfo;

	time (&rawtime);
	timeinfo = localtime ( &rawtime );

	CURRENT = Date( \
		1, \
		1, \
		(uint32_t)1900 + timeinfo->tm_year);

	printf("Current local time and date: %s\n", CURRENT.show().c_str());

	reactor_loop(employees);

	return (0);
}
