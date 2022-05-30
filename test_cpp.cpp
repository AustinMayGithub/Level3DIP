#include <iostream>
#include <stdio.h>
#include <string.h>


using namespace std;

int main (int argc, char* argv[]) {
	string user;
	string passwd;
	string database;	
	std::cout<<"Enter user name: ";
	getline(cin, user);
	std::cout<<"Enter password: ";
	getline(cin, passwd);
	std::cout<<"Enter database name: ";
	getline(cin, database);
	string login_string;
	login_string = "dbname=%s user=%s password=%s hostaddr=localhost port=5432", database, user, passwd;
	cout << login_string;
	return 0;
}


