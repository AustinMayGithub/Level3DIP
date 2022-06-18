#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;
string login_string;
const char *convertchar;
char *lchr;

int main (int    argc,
      char **argv){
  ifstream in("data.dmf");
  in >> login_string;
  convertchar = login_string.c_str();
  lchr = strdup(convertchar);
  // replace all commas with spaces
  for (int i = 0; i < strlen(lchr); i++) {
	if (lchr[i] == ',') {
	  lchr[i] = ' ';
	}
  }
  printf("%s\n", lchr);
  
  return 0;
}