#include <gtk/gtk.h>
#include <pqxx/pqxx> 
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>

GtkWidget *id_entry;
GtkWidget *name_entry;
GtkWidget *cat_entry;
GtkWidget *description_entry;
GtkWidget *price_entry;
GtkWidget *quantity_entry;

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
  COL_CAT,
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
											G_TYPE_STRING,
											G_TYPE_STRING);

  cout<<"it gets here";
  GtkTreeIter iter;
  gtk_list_store_append (store, &iter);
  // fill the row with data from the database using the sql query select command
  work W(C); 
  string query = "select * from inventory";
  cout<<"it gets here";
  result R( W.exec(query));
  // iterate through the items and add them to the database 
  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
    
    gtk_list_store_set (store, &iter,
              COL_ID, c[0].c_str(),
              COL_NAME, c[1].c_str(),
              COL_CAT, c[2].c_str(),
              COL_DESCRIPTION, c[3].c_str(),
              COL_PRICE, c[4].c_str(),
              COL_QUANTITY, c[5].c_str(),
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
// add a new item to the database
static GtkWidget *
add_item (void)
{
  // make a new window
  GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Add Item");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
  gtk_container_set_border_width (GTK_CONTAINER (window), 10);
  gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
  gtk_window_set_resizable (GTK_WINDOW (window), FALSE);
  gtk_window_set_modal (GTK_WINDOW (window), TRUE);

  // add entries for the new item
  return 0;
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
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view), -1, "Category",  renderer, "text", COL_CAT,NULL);
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
// get the currently selected row
static void
on_row_activated (GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer data)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  gchar *id;
  gchar *name;
  gchar *cat;
  gchar *description;
  gchar *price;
  gchar *quantity;
  model = gtk_tree_view_get_model (treeview);
  gtk_tree_model_get_iter (model, &iter, path);
  gtk_tree_model_get (model, &iter,COL_ID, &id, COL_NAME, &name, COL_CAT, &cat, COL_DESCRIPTION, &description, COL_PRICE, &price, COL_QUANTITY, &quantity, -1);
  printf ("Selected: %s, %s, %s, %s, %s\n", id, name, cat, description, price, quantity);
  // make a new window that shows details about the item and allow the user to edit it or delete it
  GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Item Details");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
  gtk_container_set_border_width (GTK_CONTAINER (window), 10);
  gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);

  // add entries for the new item
  id_entry = gtk_entry_new();
  name_entry = gtk_entry_new();
  cat_entry = gtk_entry_new();
  description_entry = gtk_entry_new();
  price_entry = gtk_entry_new();
  quantity_entry = gtk_entry_new();

  gtk_entry_set_text (GTK_ENTRY (id_entry), id);
  gtk_entry_set_text (GTK_ENTRY (name_entry), name);
  gtk_entry_set_text (GTK_ENTRY (cat_entry), cat);
  gtk_entry_set_text (GTK_ENTRY (description_entry), description);
  gtk_entry_set_text (GTK_ENTRY (price_entry), price);
  gtk_entry_set_text (GTK_ENTRY (quantity_entry), quantity);
  // make a grid to hold the entries  
  GtkWidget *grid = gtk_grid_new ();
  gtk_grid_set_column_spacing (GTK_GRID (grid), 10);
  gtk_grid_set_row_spacing (GTK_GRID (grid), 10);
  gtk_container_add (GTK_CONTAINER (window), grid);
  // add the entries to the grid
  gtk_grid_attach (GTK_GRID (grid), gtk_label_new ("ID"), 0, 0, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), id_entry, 1, 0, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), gtk_label_new ("Name"), 0, 1, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), name_entry, 1, 1, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), gtk_label_new ("Category"), 0, 2, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), cat_entry, 1, 2, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), gtk_label_new ("Description"), 0, 3, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), description_entry, 1, 3, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), gtk_label_new ("Price"), 0, 4, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), price_entry, 1, 4, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), gtk_label_new ("Quantity"), 0, 5, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), quantity_entry, 1, 5, 1, 1);
  // add a button to save the changes
  GtkWidget *save_button = gtk_button_new_with_label ("Save");
  gtk_grid_attach (GTK_GRID (grid), save_button, 1, 6, 1, 1);
  // add a button to delete the item
  GtkWidget *delete_button = gtk_button_new_with_label ("Delete");
  gtk_grid_attach (GTK_GRID (grid), delete_button, 1, 7, 1, 1);
  // connect the signals to the buttons
  gtk_widget_show_all (window);




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
  // make four buttons along the top edge of the screen 
  GtkWidget *button1 = gtk_button_new_with_label ("Add Item");
  GtkWidget *button2 = gtk_button_new_with_label ("Remove Item");
  GtkWidget *button3 = gtk_button_new_with_label ("Update Item");
  GtkWidget *button4 = gtk_button_new_with_label ("Search Item");
  GtkWidget *button5 = gtk_button_new_with_label ("Exit");
  // make a grid to hold the buttons
  GtkWidget *grid;
  grid = gtk_grid_new();
  // add the buttons to the grid
  gtk_grid_attach (GTK_GRID (grid), button1, 0, 0, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), button2, 1, 0, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), button3, 2, 0, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), button4, 3, 0, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), button5, 4, 0, 1, 1);

  g_signal_connect(button1, "clicked", G_CALLBACK(add_item), &C);
  g_signal_connect(button5, "clicked", G_CALLBACK(exit), NULL);
  // add the tree view to the grid and make it fill the entire window except for the buttons
  

  // add scroll bar to the window
  GtkWidget *scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  // make the window scrollable
  
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_add (GTK_CONTAINER (scrolled_window), view);
  // make sure that the tree view is the size of the window
  gtk_widget_set_size_request (scrolled_window, 500, 600);
  gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 4, 5, 4);
  // get the value of the item that is selected in the tree view
  g_signal_connect (view, "row-activated", G_CALLBACK (on_row_activated), NULL);

  gtk_container_add (GTK_CONTAINER (window), grid);
  // set smallest window size
  gtk_window_set_default_size (GTK_WINDOW (window), 300, 300);
  gtk_window_set_title(GTK_WINDOW(window), "Online - DataManage");
  gtk_window_set_resizable (GTK_WINDOW (window), FALSE);
  gtk_widget_show_all (window);

  gtk_main ();

  return 0;
}
