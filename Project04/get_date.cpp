#include "my_header.h"
void get_date(int& month, int&day,  int& year)
{
	time_t my_time=time(0);
	tm* now=localtime(&my_time);
	year=now->tm_year+1900;
	month=now->tm_mon+1;
	day=now->tm_mday;
}
