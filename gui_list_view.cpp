#include <gtk/gtk.h>
#include <pqxx/pqxx> 
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>

gchar *id;
GtkWidget *search_box;
GtkWidget *id_entry;
GtkWidget *name_entry;
GtkWidget *cat_entry;
GtkWidget *description_entry;
GtkWidget *price_entry;
GtkWidget *quantity_entry;
GtkWidget *update_item;
gchar *name;
gchar *cat;
gchar *description;
gchar *price;
gchar *quantity;

// connect to the database
using namespace std;

using namespace pqxx;
string login_string;
const char* convertchar;
char *lchr;

int i; 
int login (){
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
    // add a filter to the list view
    
  return GTK_TREE_MODEL (store);
  
}
static void
exit (void)
{
  gtk_main_quit ();
}
// commit data to the database
static void
commit_data (void)
{
 
  connection C(lchr);
  work W(C); 
  string query = "insert into inventory values ("+ string(gtk_entry_get_text(GTK_ENTRY(id_entry))) + ", '" + string(gtk_entry_get_text(GTK_ENTRY(name_entry))) + "', '" + string(gtk_entry_get_text(GTK_ENTRY(cat_entry))) + "', '" + string(gtk_entry_get_text(GTK_ENTRY(description_entry))) + "', " + string(gtk_entry_get_text(GTK_ENTRY(price_entry))) + ", " + string(gtk_entry_get_text(GTK_ENTRY(quantity_entry))) +")";
  W.exec(query);
  W.commit();
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
static void 
delete_data (void) {
  // remove the selected item from the database
  printf("%s", lchr);
  connection C(lchr);
  work W(C);
  string query = "delete from inventory where id = ";
  query += id;
  W.exec(query);
  W.commit();
}
// update the item in the database
static void
update_data (void) {
  // commit the data to the database
  connection C(lchr);
  work W(C);
  // check if id is in the database
  string query = "select * from inventory where id = ";
  query += id;
  result R( W.exec(query));
  try {
    string query = "update inventory set id = " + string(gtk_entry_get_text(GTK_ENTRY(id_entry))) + ", name = '" + string(gtk_entry_get_text(GTK_ENTRY(name_entry))) + "', category = '" + string(gtk_entry_get_text(GTK_ENTRY(cat_entry))) + "', description = '" + string(gtk_entry_get_text(GTK_ENTRY(description_entry))) + "', price = " + string(gtk_entry_get_text(GTK_ENTRY(price_entry))) + ", quantity = " + string(gtk_entry_get_text(GTK_ENTRY(quantity_entry))) + " where id = '" + id + "'";
    W.exec(query);
    W.commit();
  } catch (pqxx::sql_error const &e) {
    GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW (update_item),
                                              GTK_DIALOG_DESTROY_WITH_PARENT,
                                              GTK_MESSAGE_ERROR,
                                              GTK_BUTTONS_CLOSE,
                                              "Duplicated IDs, Make sure that you do not use an ID more than once.\n\n More Info: %s", e.what());
    gtk_widget_show (dialog);
    
    }
    
  }

static GtkWidget * 
add_item (void) {
  
  // add the item to the database
  GtkWidget *add_item = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (add_item), "Add Item");
  gtk_window_set_default_size (GTK_WINDOW (add_item), 200, 200);
  gtk_container_set_border_width (GTK_CONTAINER (add_item), 10);
  gtk_window_set_position (GTK_WINDOW (add_item), GTK_WIN_POS_CENTER);

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
  gtk_container_add (GTK_CONTAINER (add_item), grid);
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
  g_signal_connect (save_button, "clicked", G_CALLBACK (commit_data), NULL);
  // add a button to delete the item
  // connect the signals to the buttons
  gtk_widget_show_all (add_item);
}

// get the currently selected row
static void
on_row_activated (GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer data)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  model = gtk_tree_view_get_model (treeview);
  gtk_tree_model_get_iter (model, &iter, path);
  gtk_tree_model_get (model, &iter,COL_ID, &id, COL_NAME, &name, COL_CAT, &cat, COL_DESCRIPTION, &description, COL_PRICE, &price, COL_QUANTITY, &quantity, -1);
  printf ("Selected: %s, %s, %s, %s, %s\n", id, name, cat, description, price, quantity);
  // make a new window that shows details about the item and allow the user to edit it or delete it
  GtkWidget *update_item = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (update_item), "Item Details");
  gtk_window_set_default_size (GTK_WINDOW (update_item), 200, 200);
  gtk_container_set_border_width (GTK_CONTAINER (update_item), 10);
  gtk_window_set_position (GTK_WINDOW (update_item), GTK_WIN_POS_CENTER);

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
  gtk_container_add (GTK_CONTAINER (update_item), grid);
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
  g_signal_connect (save_button, "clicked", G_CALLBACK (update_data), NULL);
  // add a button to delete the item
  GtkWidget *delete_button = gtk_button_new_with_label ("Delete");
  // make the delete button colored red using the style provider
  GtkCssProvider *provider = gtk_css_provider_new ();
  gtk_css_provider_load_from_data (provider, "* { color: red; }", -1, NULL);
  gtk_style_context_add_provider (gtk_widget_get_style_context (delete_button), GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
  gtk_grid_attach (GTK_GRID (grid), delete_button, 1, 7, 1, 1);
  g_signal_connect (delete_button, "clicked", G_CALLBACK (delete_data), NULL);
  // unreference the provider
  g_object_unref (provider);
  // connect the signals to the buttons
  gtk_widget_show_all (update_item);

}
// search for an item by id
// static void
// search_item (GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer data, GtkEntry *entry) {
//   // get the text from the entry 
//   const gchar *text = gtk_entry_get_text (search_box);
//   printf("HI");
//   // find the item in the list and highlight it
//   GtkTreeModel *model;
//   GtkTreeIter iter;
//   model = gtk_tree_view_get_model (GTK_TREE_VIEW (tree_view));
//   if (gtk_tree_model_get_iter_first (model, &iter)) {
//     do {
//       gchar *id;
//       gtk_tree_model_get (model, &iter, COL_ID, &id, -1);
//       if (strcmp (id, text) == 0) {
//         GtkTreePath *path = gtk_tree_model_get_path (model, &iter);
//         gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW (tree_view), path, NULL, FALSE, 0, 0);
//         gtk_tree_selection_select_iter (gtk_tree_view_get_selection (GTK_TREE_VIEW (tree_view)), &iter);
//         gtk_tree_path_free (path);
//       }
//     } while (gtk_tree_model_iter_next (model, &iter));
//   }

// }




int main(int argc, char* argv[]) {
  try {
    login();
    connection C(lchr);
    // connect to the database, otherwise throw an error
    if (C.is_open()) {
        cout << "Connection Successful, connected to: " << C.dbname() << endl;
    } else {
        cout << "Connection Failed" << endl;
        return 1;
    }
  } catch (pqxx::sql_error const &e) {
    // open a window to show the error
    GtkWidget *error_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (error_window), "Error - DataManage");
    gtk_window_set_default_size (GTK_WINDOW (error_window), 300, 100);
    gtk_window_set_position (GTK_WINDOW (error_window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width (GTK_CONTAINER (error_window), 10);
    GtkWidget *error_label = gtk_label_new ("Error connecting to the database");
    gtk_container_add (GTK_CONTAINER (error_window), error_label);
    gtk_widget_show_all (error_window);

  }
  gtk_init (&argc, &argv);

  GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect (window, "destroy", gtk_main_quit, NULL);

  GtkWidget *view = create_view_and_model ();
  // make four buttons along the top edge of the screen 
  GtkWidget *button1 = gtk_button_new_with_label ("Add Item");
  GtkWidget *button2 = gtk_button_new_with_label ("Sort Items");
  GtkWidget *button3 = gtk_button_new_with_label ("Update Item");
  GtkWidget *button4 = gtk_button_new_with_label ("Search Item");
  GtkWidget *button5 = gtk_button_new_with_label ("Exit");
  // add a search box below the buttons
  GtkWidget *search_box = gtk_entry_new ();
  // make the search box say "Type to search"
  GtkWidget *search_label = gtk_label_new ("Type or scan to search ID");
  // make a grid to hold the buttons
  GtkWidget *grid;
  grid = gtk_grid_new();
  // add the buttons to the grid
  gtk_grid_attach (GTK_GRID (grid), button1, 0, 0, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), button2, 1, 0, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), button3, 2, 0, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), button4, 3, 0, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), button5, 4, 0, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), search_label, 0, 1, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), search_box, 1, 1, 5, 1);

  g_signal_connect(button1, "clicked", G_CALLBACK(add_item), NULL);
  g_signal_connect(button5, "clicked", G_CALLBACK(exit), NULL);
  // make the search bar highlighted by default
  gtk_widget_grab_focus (search_box);
  // when enter is pressed, search for the item
  // g_signal_connect (search_box, "activate", G_CALLBACK (search_item), &C);
  // add scroll bar to the window
  GtkWidget *scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  // make the window scrollable
  
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_add (GTK_CONTAINER (scrolled_window), view);
  // make sure that the tree view is the size of the window
  gtk_widget_set_size_request (scrolled_window, 550, 600);
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