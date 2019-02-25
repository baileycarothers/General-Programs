/*
Author: Bailey Carothers
Course Section: CSCE 1030.002
Date: 5/2/2018
Description: This program serves as a basic library management system.
It allows users to enter new books into the system, return books back into the system, 
look up books checked out to a student, and search for books by author and title.
*/


/*
Known bugs:
-Entering a new book works, but the output file formats incorrectly
-Checking out a book sometimes causes a core dump
To do:
-Fix add book function file output
-Finish book return
-Search by author and title
-Search by student does not currently output ISBN numbers
*/

#include "my_header.h"
using namespace std;


//Declaring our functions
void enter_book();
void check_out();
void return_book();
void search_by_author();
void search_by_title();
void search_by_student();
int number_of_students();
int number_of_books();
void search_book();
void get_date(int& month, int&day,  int& year);

int main()
{
	
	// display student information
	cout << "+------------------------------------------------------+" << endl;
	cout << "|           Computer Science and Engineering           |" << endl;
	cout << "|            CSCE 1030 - Computer Science I            |" << endl;
	cout << "| Bailey Carothers BSC0108 Bailey.Carothers@my.unt.edu |" << endl;
	cout << "+------------------------------------------------------+" << endl;
	cout << endl;

	int choice; //Menu selection	
	bool success; //For determining if user choice is acceptable

	do
	{
	cout<<"1. Add"<<endl;

	cout<<"2. Check Out"<<endl;
	
	cout<<"3. Return"<<endl;
	
	cout<<"4. Search"<<endl;
	
	cout<<"Enter your choice, 0 to Quit: ";
	
	cin>>choice;
	cin.ignore();
		do
		{
			switch(choice)
			{
			case 0:
				success=true;
				break;
			case 1:
				enter_book();
				success=true;
				break;
			case 2:
				check_out();
				success=true;
				break;
			case 3: 
				return_book();
				success=true;
				break;
			case 4: 
				search_book();
				success=true;
				break;
			default:
				cout<<"Wrong Choice. Enter again";
				success=false;
				break;
			}
		}while(!success);
	}while(choice!=0);
	return 0;
}


void enter_book()
{
	int nB = number_of_books(); //Calling our function to populate the integer nB for use in loops
	int month = 0, day = 0, year = 0; //We know the new book will have these values
	Book addnew;
	addnew.EUID = "NA"; //We know the new book will have this value

	//Time to populate the new book structure
	cout << "Please enter the book's author: ";
	getline(cin, addnew.author);
	cout << "Please enter the book's title: ";
	getline(cin, addnew.title);
	cout << "Please enter the book's ISBN #: ";
	cin >> addnew.ISBN;

	Book* ptr; //Pointer to store our books.txt file inside of
	ptr = new Book[nB + 1]; //Initializing it to the current number of books + 1, need room for new book

	ifstream inFS;
	inFS.open("books.txt");

	//Check to make sure the file opened correctly, if not displays error message
	if (!inFS.is_open()) 
	{
		cout << "Could not open file books.txt." << endl;
	}
	
/*
	//Made this loop to test the array
	//Uncommenting this loop populates the array correctly
	//So the issue is with the loop that copies elements into the array from the file
	for (int i = 0; i < nB; ++i)
	{
		ptr[i].author = "no author";
		ptr[i].title = "no title";
		ptr[i].ISBN = 00000000;
		ptr[i].EUID = "abcd123";
		ptr[i].borrowed.month = 0;
		ptr[i].borrowed.day = 0;
		ptr[i].borrowed.year = 0;
	}
*/

	//Loop that iterates through books.txt and copies the elements to our Book ptr
	for (int i = 0; i < nB; ++i)
	{
			getline(inFS, ptr[i].author);
			getline(inFS, ptr[i].title);
			inFS >> ptr[i].ISBN;
			getline(inFS, ptr[i].EUID);
			inFS >> ptr[i].borrowed.month;
			inFS >> ptr[i].borrowed.day;
			inFS >> ptr[i].borrowed.year;
	}

	inFS.close();

	//Open books.txt as output so we can compile our new list
	ofstream outFS;
	outFS.open("books.txt");

	//Check to make sure the file opened correctly, if not displays error message
	if (!outFS.is_open())
	{
		cout << "Could not open file books.txt." << endl;
	}

	//Iterate through our pointer array and output contents to the file
	for (int i = 0; i < nB; ++i)
	{
		outFS << ptr[i].author << '\t' << ptr[i].title << '\t' << ptr[i].ISBN << '\t' << ptr[i].EUID << '\t' << ptr[i].borrowed.month << '\t' << ptr[i].borrowed.day << '\t' << ptr[i].borrowed.year << '\n' ;
	}

	outFS << addnew.author << '\t' << addnew.title << '\t' << addnew.ISBN << '\t' << addnew.EUID << '\t' << month << '\t' << day << '\t' << year << '\n';
	outFS.close();
}

void check_out()
{
	int nB = number_of_books();
	int numberOfStudents = number_of_students();
	int studentID, day = 0, month = 0, year = 0;
	bool euidFound = false; //Bool value for searching students.txt
	bool bookfound = false; //Bool value for looking up ISBN
	Book bookCheckout; //Declaring a book struct for the book to be checked out
	Student booksBorrowed; //Declaring a student struct for updating their total

	get_date(month, day, year);//Call get_date to get values for current month, day, and year

	cout << "Please enter the EUID of the student checking out the book: ";
	cin >> bookCheckout.EUID; //Storing in the book now so that we dont have to transfer it later

	ofstream outFS;
	ifstream inFS;
	inFS.open("students.txt");

	//Check to make sure the file opened correctly, if not displays error message
	if(!inFS.is_open())
	{
		cout << "Could not open the file students.txt." << endl;
	}

	Student* studentpointer = new Student[numberOfStudents]; //Array to store our students and the number of books they have

	for (int i = 0; i < numberOfStudents; ++i)
	{
		inFS >> studentpointer[i].EUID >> studentpointer[i].number_of_books;
	}

	//For loop to check that the EUID exists
	for (int i = 0; i < numberOfStudents; ++i)
	{
		if (bookCheckout.EUID == studentpointer[i].EUID)
		{
			studentID = i;
			euidFound = true;
			//studentpointer[i].number_of_books = studentpointer[i].number_of_books + 1;
			cout << "Student found." << endl;
		}
	}

	inFS.close();
	outFS.open("students.txt");

	//Let the user know if an invalid EUID was entered
	if (!euidFound)
		cout << "Not a valid EUID, book cannot be checked out." << endl;

	if (euidFound)
	{

		if (studentpointer[studentID].number_of_books < 5)
		{
			cout << "Please enter the ISBN of the book being checked out: ";
			cin >> bookCheckout.ISBN;
			inFS.open("books.txt");

			Book* ptr; //Pointer to store our books.txt file inside of
			ptr = new Book[nB]; //Initializing it to the current number of books

			//Loop that iterates through books.txt and copies the elements to our Book ptr
			for (int i = 0; i < nB; ++i)
			{
				if (i < nB)
				{
					getline(inFS, ptr[i].author);
					getline(inFS, ptr[i].title);
					inFS >> ptr[i].ISBN;
					getline(inFS, ptr[i].EUID);
					inFS >> ptr[i].borrowed.month >> ptr[i].borrowed.day >> ptr[i].borrowed.year;
				}
			}

			//Search book database file using book structure
			//and search for a match with the ISBN entered
			//When a match is found, update the book database's file corresponding
			//EUID entry with the student's EUID and the date with the current date
			for (int i = 0; i < nB; ++ i)
			{
				if (bookCheckout.ISBN == ptr[i].ISBN)
					{
						ptr[i].EUID = bookCheckout.EUID;
						ptr[i].borrowed.day = day;
						ptr[i].borrowed.month = month;
						ptr[i].borrowed.year = year;
						bookfound = true;
						//Increment number of books checked out to student by 1
						studentpointer[studentID].number_of_books = studentpointer[studentID].number_of_books + 1;
					}
			}

			for (int i = 0; i < nB + 1; ++i)
			{
				outFS << ptr[i].author << '\t' << ptr[i].title << '\t' << ptr[i].ISBN << '\t' << ptr[i].EUID << '\t' << ptr[i].borrowed.month << '\t' << ptr[i].borrowed.day << '\t' << ptr[i].borrowed.year << '\n' ;
			}
		}

		//If 5 or more books are already borrowed, book cannot be checked out
		else if (studentpointer[studentID].number_of_books >= 5)
			cout << "Too many books are already checked out to the student." << endl << "Please return a book to check out more." << endl;
	}

	if (!bookfound)
		cout << "Book was not found. Please try again.";

	for (int i = 0; i < 4; ++i)
	{
		outFS << studentpointer[i].EUID << '\t' << studentpointer[i].number_of_books << '\n';
	}
	outFS.close();
	delete[] studentpointer;
}

void return_book()
{
	Book bookreturn;
	bool bookMatch = false;

	cout << "Please enter the ISBN of the book being returned: ";
	cin >> bookreturn.ISBN;

	if (!bookMatch)
		cout << "This book does not belong to the library.";

	if (bookMatch)
	{
		bookreturn.EUID = "NA";
		bookreturn.borrowed.month = 0;
		bookreturn.borrowed.day = 0;
		bookreturn.borrowed.year = 0;
	}
	//Decrement number of books checked out by the student by 1
}

void search_by_author()
{
	string name;
	cout << "Please enter part or all of the author's name: ";
	cin >> name;

	//If there is a match, display the author, title, ISBN, and borrowed date
	//Do NOT display the EUID
}

void search_by_title()
{
	string title;
	cout << "Please enter part or all of the title of the book: ";
	cin >> title;

	//If there is a match, display the author, title, ISBN, and borrowed date
	//Do NOT display the EUID
}

void search_by_student()
{
	int numberOfStudents = number_of_students();
	bool euidFound = false; //Bool value for searching students.txt
	string EUID;
	int numBooksOut;
	cout << "Please enter the EUID of the student you wish to look up: ";
	cin >> EUID;

	ifstream inFS;
	inFS.open("students.txt");

	//Check to make sure the file opened correctly, if not displays error message
	if(!inFS.is_open())
	{
		cout << "Could not open the file students.txt." << endl;
	}

	Student* studentpointer = new Student[numberOfStudents]; //Array to store our students and the number of books they have

	for (int i = 0; i < numberOfStudents; ++i)
	{
		inFS >> studentpointer[i].EUID >> studentpointer[i].number_of_books;
	}

	//For loop to check that the EUID exists
	for (int i = 0; i < numberOfStudents; ++i)
	{
		if (EUID == studentpointer[i].EUID)
		{
			numBooksOut = studentpointer[i].number_of_books;
			euidFound = true;
			if (numBooksOut == 0)
				cout << "No books are currently checked out to this student.";
			else
			{
			cout << "Student found." << endl << "The student currently has " << numBooksOut << " books checked out." << endl;
			}
		}
	}

	//Let the user know if an invalid EUID was entered
	if (!euidFound)
		cout << "Not a valid EUID, book cannot be checked out." << endl;

	//If books have been checked out, use the long integer pointer
	//of Student structure to create a dynamic array of the size of the # of books borrowed
	//Read books.txt, copy ISBN of matches to the array
	//Read till the array is full

	//Display contents of the array
	//cout << "Here are the ISBNs of the books checked out to the student.";
}

int number_of_students()
{
	string line;int count=0;
	ifstream fin;fin.open("students.txt");
	while(getline(fin, line))
	{
		++count;
	}
	return count;
}

int number_of_books()
{
	string line;int count=0;
	ifstream fin;fin.open("books.txt");
	while(getline(fin, line))
	{
		++count;
	}
	return count;
}


void search_book()
{
	bool success=false;
	do
	{
	cout<<"What do you want to search with?"<<endl;
	cout<<"1. Author"<<endl;
	cout<<"2. Title"<<endl;
	cout<<"3. Student"<<endl;
	cout<<"Enter your choice: ";
	int choice;
	cin>>choice;
	cin.ignore();

		switch(choice)
		{
		case 1:
			search_by_author();
			success=true;
			break;	
		case 2:
			search_by_title();
			success=true;
			break;
		case 3:
			search_by_student();
			success=true;
			break;
		default:
			cout<<"Wrong choice. Try again." << endl;
			success=false;
			break;
		}
	}while(!success);
}

void get_date(int& month, int&day,  int& year)
{
	time_t my_time=time(0);
	tm* now=localtime(&my_time);
	year=now->tm_year+1900;
	month=now->tm_mon+1;
	day=now->tm_mday;
}