// compile flags: gcc conn_test.cpp -o conn_test -lpqxx -lpq
// requires libpqxx version 7.7.3 or later

#include <iostream>
#include <pqxx/pqxx> 
#include <string.h>

//ask for user name, password, and database name
void get_info() {
   
}
char login_string(255);
using namespace std;
using namespace pqxx;

int main(int argc, char* argv[]) {
   string user;
   string passwd;
   string database;
   std::cout<<"Enter user name: ";
   getline(std::cin, user);
   std::cout<<"Enter password: ";
   getline(std::cin, passwd);
   std::cout<<"Enter database name: ";
   getline(std::cin, database);
   get_info();
   connection C("dbname="+database+" user="+user+" password="+passwd+" hostaddr=127.0.0.1 port=5432");
   if (C.is_open()) {
      cout << "Connection Successful, connected to: " << C.dbname() << endl;
   } else {
      cout << "Connection Failed" << endl;
      return 1;
   }
   while (true) {
      try {
         
         work W(C);
         // ask the user what they want to do
         cout<<"DataManage Menu"<<endl;
         cout<<"1. Add a new item"<<endl;
         cout<<"2. Remove an item"<<endl;
         cout<<"3. Update an item"<<endl;
         cout<<"4. Search for an item"<<endl;
         cout<<"5. View all items"<<endl;
         cout<<"6. View all items in a category"<<endl;
         cout<<"7. Setup Tables"<<endl;
         // read input from user
         int choice;
         cin>>choice;
         // do the appropriate thing
         if (choice == 1) {
            string name;
            string category;
            string description;
            string price;
            string quantity;
            cout<<"Enter item name: ";
            cin>>name;
            cout<<"Enter item category: ";
            cin>>category;
            cout<<"Enter item description: ";
            cin>>description;
            cout<<"Enter item price: ";
            cin>>price;
            cout<<"Enter item quantity: ";
            cin>>quantity;
            W.exec("INSERT INTO items (name, category, description, price, quantity) VALUES ('"+name+"', '"+category+"', '"+description+"', '"+price+"', '"+quantity+"');");
         } else if (choice == 2) {
            string name;
            cout<<"Enter item name: ";
            cin>>name;
            W.exec("DELETE FROM items WHERE name = '"+name+"';");
         } else if (choice == 3) {
            string name;
            string category;
            string description;
            string price;
            string quantity;
            cout<<"Enter item name: ";
            cin>>name;
            cout<<"Enter item category: ";
            cin>>category;
            cout<<"Enter item description: ";
            cin>>description;
            cout<<"Enter item price: ";
            cin>>price;
            cout<<"Enter item quantity: ";
            cin>>quantity;
            W.exec("UPDATE items SET category = '"+category+"', description = '"+description+"', price = '"+price+"', quantity = '"+quantity+"' WHERE name = '"+name+"';");
         } else if (choice == 4) {
            string name;
            cout<<"Enter item name: ";
            cin>>name;
            result R(W.exec("SELECT * FROM items WHERE name = '"+name+"';"));
            if (R.size() == 0) {
               cout<<"Item not found"<<endl;
            } else {
               for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
                  cout<<"Name: "<<c[0].as<string>()<<endl;
                  cout<<"Category: "<<c[1].as<string>()<<endl;
                  cout<<"Description: "<<c[2].as<string>()<<endl;
                  cout<<"Price: "<<c[3].as<string>()<<endl;
                  cout<<"Quantity: "<<c[4].as<string>()<<endl;
               }
            }
         } else if (choice == 5) {
            result R(W.exec("SELECT * FROM items;"));
            if (R.size() == 0) {
               cout<<"No items found"<<endl;
            } else {
               for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
                  cout<<"Name: "<<c[0].as<string>()<<endl;
                  cout<<"Category: "<<c[1].as<string>()<<endl;
                  cout<<"Description: "<<c[2].as<string>()<<endl;
                  cout<<"Price: "<<c[3].as<string>()<<endl;
                  cout<<"Quantity: "<<c[4].as<string>()<<endl;
               }
            }
         } else if (choice == 6) {
            string category;
            cout<<"Enter item category: ";
            cin>>category;
            result R(W.exec("SELECT * FROM items WHERE category = '"+category+"';"));
            if (R.size() == 0) {
               cout<<"No items found"<<endl;
            } else {
               for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
                  cout<<"Name: "<<c[0].as<string>()<<endl;
                  cout<<"Category: "<<c[1].as<string>()<<endl;
                  cout<<"Description: "<<c[2].as<string>()<<endl;
                  cout<<"Price: "<<c[3].as<string>()<<endl;
                  cout<<"Quantity: "<<c[4].as<string>()<<endl;
               }
            }
         } else if (choice == 7) {
            W.exec("CREATE TABLE items (name VARCHAR(255), category VARCHAR(255), description VARCHAR(255), price VARCHAR(255), quantity VARCHAR(255));");
         } else {
            cout<<"Invalid choice"<<endl;
         }
      } catch (const std::exception &e) {
         std::cerr << e.what() << std::endl;
      }
   }
}
