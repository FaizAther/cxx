#include "Management.hpp"

void
show(Management &manage)
{
	char buf[BUF_SML] = {0}, \
		val = ~0, *ret = NULL;
	
	do {
		printf("Show " \
			"{c:customers, b:bikes" \
			", m:bookings, a:all or x:exit}:? ");
		bzero(buf, BUF_SML);
		ret = fgets(buf, BUF_SML, stdin);
		sscanf(buf, "%c", &val);
		switch (val) {
			case 'c':
				manage.show_customers();
				break;
			case 'b':
				manage.show_bikes();
				break;
			case 'm':
				manage.show_bookings();
				break;
			case 'a':
				manage.show();
				break;
			case 'x':
				return;
			default:
				printf("Try again!\n");
		}
	} while (ret != NULL);
}

void
make(Management &manage)
{
	char buf[BUF_SML] = {0}, \
		val = ~0, *ret = NULL;

	do {
		printf("Create " \
			"{c:customer, b:bike" \
			", m:booking or x:exit}:? ");
		bzero(buf, BUF_SML);
		ret = fgets(buf, BUF_SML, stdin);
		sscanf(buf, "%c", &val);
		switch (val) {
			case 'c':
				manage.make_customer();
				break;
			case 'b':
				manage.make_bike();
				break;
			case 'm':
				manage.make_booking();
				break;
			case 'x':
				return;
			default:
				printf("Try again!\n");
		}
	} while (ret != NULL);
}

char *
get_id(char *buf, ID &rid, const char *what)
{
	char *ret = NULL;
	printf("Remove %s {id}:? ", what);
	bzero(buf, 3);
	ret = fgets(buf, 3, stdin);
	sscanf(buf, "%d", &rid);
	return ret;
}

void
remove(Management &manage)
{
	char buf[BUF_SML] = {0}, \
		val = ~0, *ret = NULL;
	ID rid = BAD_ID;

	do {
		printf("Remove " \
			"{c:customer, b:bike" \
			", m:booking, s:show or x:exit}:? ");
		bzero(buf, BUF_SML);
		ret = fgets(buf, BUF_SML, stdin);
		sscanf(buf, "%c", &val);
		switch (val) {
			case 'c':
				ret = get_id(buf, rid, "customer");
				manage.cfind_remove(rid, true);
				break;
			case 'b':
				ret = get_id(buf, rid, "bike");
				manage.bfind_remove(rid, true);
				break;
			case 'm':
				ret = get_id(buf, rid, "booking");
				manage.remove_booking(rid);
				break;
			case 'x':
				return;
			default:
				printf("Try again!\n");
		}
	} while (ret != NULL);
}

void
reactor_loop(Management &manage)
{
	char buf[BUF_SML] = {0}, \
		val = ~0, *ret = NULL;

	do {
		printf("Which " \
			"{m:make, s:show, d:delete or x:exit}:? ");
		bzero(buf, BUF_SML);
		ret = fgets(buf, BUF_SML, stdin);
		sscanf(buf, "%c", &val);
		switch (val) {
			case 'm':
				make(manage);
				break;
			case 's':
				show(manage);
				break;
			case 'd':
				remove(manage);
				break;
			case 'x':
				return;
			default:
				printf("Try again!\n");
		}
	} while (ret != NULL);
}

int
main(void)
{
	Management manage;	

	reactor_loop(manage);

	return (0);
}
