#include <gtk/gtk.h>
#include <pqxx/pqxx> 
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdbool.h>
#include <string.h>
#include <math.h>
// connect to the database
using namespace std;

using namespace pqxx;
string login_string;

int i; 
int login (){
  ifstream in("data.dmf");
  in >> login_string;
  const char *lchr = login_string.c_str();
  printf("%s", lchr);
  connection C(lchr);
  
  return 0;
}
enum
{
  COL_ID = 0,
  COL_NAME,
  COL_DESCRIPTION,
  COL_PRICE,
  COL_QUANTITY,
  NUM_COLS
} ;


static GtkTreeModel *
create_and_fill_model (void)
{
  ifstream in("data.dmf");
  in >> login_string;
  const char *lchr = login_string.c_str();
  printf("%s", lchr);
  lchr = "dbname=testdb user=postgres password=Austin12! host=127.0.0.1 port=5432";
  connection C(lchr);
  

  GtkListStore *store = gtk_list_store_new (NUM_COLS,
                      G_TYPE_STRING,
											G_TYPE_STRING,
											G_TYPE_STRING,
											G_TYPE_STRING,
											G_TYPE_STRING);

  /* Append a row and fill in some data */
  cout<<"it gets here";
  GtkTreeIter iter;
  gtk_list_store_append (store, &iter);
  // fill the row with data from the database using the sql query select command
  work W(C); 
  string query = "select * from items";
  cout<<"it gets here";
  result R( W.exec(query));
  // iterate through the items and add them to the database 
  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
    
    gtk_list_store_set (store, &iter,
              COL_ID, c[0].c_str(),
              COL_NAME, c[1].c_str(),
              COL_DESCRIPTION, c[2].c_str(),
              COL_PRICE, c[3].c_str(),
              COL_QUANTITY, c[4].c_str(),
              -1);
    gtk_list_store_append (store, &iter);
    }
    
  return GTK_TREE_MODEL (store);
  
}
static void
exit (void)
{
  gtk_main_quit ();
}


static GtkWidget *
create_view_and_model (void)
{
  GtkWidget *view = gtk_tree_view_new ();
  // create the model and add it to the view
  // populate the cells with the column data
  GtkCellRenderer *renderer;
  renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view), -1, "ID",  renderer, "text", COL_ID,NULL);
  renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view), -1, "Name",  renderer, "text", COL_NAME,NULL);
  renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view), -1, "Description",  renderer, "text", COL_DESCRIPTION,NULL);
  renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view), -1, "Price",  renderer, "text", COL_PRICE,NULL);
  renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view), -1, "Quantity",  renderer, "text", COL_QUANTITY,NULL);

  // load the database data into the tree view
  GtkTreeModel *model = create_and_fill_model ();

   
  gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);

  g_object_unref (model);

  return view;
}


int main(int argc, char* argv[]) {
  ifstream in("data.dmf");
  in >> login_string;
  const char *lchr = login_string.c_str();
  printf("%s", lchr);
  connection C(lchr);
   // connect to the database, otherwise throw an error
   if (C.is_open()) {
      cout << "Connection Successful, connected to: " << C.dbname() << endl;
   } else {
      cout << "Connection Failed" << endl;
      return 1;
   }
  gtk_init (&argc, &argv);

  GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect (window, "destroy", gtk_main_quit, NULL);

  GtkWidget *view = create_view_and_model ();
  // make 4 buttons on the left side of the window
  GtkWidget *button1 = gtk_button_new_with_label ("Add Item");
  GtkWidget *button2 = gtk_button_new_with_label ("Remove Item");
  GtkWidget *button3 = gtk_button_new_with_label ("Update Item");
  GtkWidget *button4 = gtk_button_new_with_label ("Exit");
  // make a grid to hold the buttons
  GtkWidget *grid;
  grid = gtk_grid_new();
  gtk_grid_attach(GTK_GRID(grid), button1, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button2, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button3, 0, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button4, 0, 3, 1, 1);
  g_signal_connect(button4, "clicked", G_CALLBACK(exit), NULL);
  // add the tree view to the grid and make it fill the entire window except for the buttons
  

  // add scroll bar to the window
  GtkWidget *scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  // make the window scrollable
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_add (GTK_CONTAINER (scrolled_window), view);
  gtk_grid_attach(GTK_GRID(grid), scrolled_window, 1, 0, 9, 4);
  gtk_container_add (GTK_CONTAINER (window), grid);
  // set smallest window size
  gtk_window_set_default_size (GTK_WINDOW (window), 300, 300);

  gtk_widget_show_all (window);

  gtk_main ();

  return 0;
}
