#include <ctime>
#include <iostream>
#include <fstream>
using namespace std;
void get_date(int &m, int &d, int &y);

struct Date {
	int month;
	int day;
	int year;
};

struct Student {
	string EUID;
	int number_of_books;
};

struct Book {
	string author;
	string title;
	long long int ISBN;
	string EUID;
	Date borrowed;
};