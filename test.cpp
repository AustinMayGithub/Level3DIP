#include <iostream>
#include <fstream>
#include<map>

 
const char *username;
const char *password;
const char *database;
const char *ip;
const char *port;
int i; 
using namespace std;
map<int, string>  data;
ifstream in("data.dmf");
string line;
while(getline(in,line)){
	data[i] = line;
	i++;
}
username = data[0].c_str();
password = data[1].c_str();
database = data[2].c_str();
ip = data[3].c_str();
port = data[4].c_str();
