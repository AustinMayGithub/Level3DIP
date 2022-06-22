#include <gtk/gtk.h>
#include <pqxx/pqxx> 
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>


GtkCellRenderer *renderer;
GtkWidget *search_box;
GtkWidget *id_entry;
GtkWidget *name_entry;
GtkWidget *cat_entry;
GtkWidget *filter_entry;
GtkWidget *description_entry;
GtkWidget *price_entry;
GtkWidget *quantity_entry;
GtkWidget *update_item;
GtkWidget *view;
GtkWidget *add_window;
GtkWidget *main_window;
GtkWidget *filter_window;

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
string category_filter;
string id;


int i; 
int login (){
  ifstream in("data.dmf");
  in >> login_string;
  convertchar = login_string.c_str();
  lchr = strdup(convertchar);
  category_filter = "*";
  // replace all commas with spaces
  for (int i = 0; i < strlen(lchr); i++) {
	if (lchr[i] == ',') {
	  lchr[i] = ' ';
	}
  }

  connection C(lchr);
  
  return 0;
}
string query_main = "select * from inventory";
enum
{
  COL_ID,
  COL_NAME,
  COL_CAT,
  COL_DESCRIPTION,
  COL_PRICE,
  COL_QUANTITY,
  NUM_COLS
} ;

GtkListStore *store = gtk_list_store_new (NUM_COLS,
                    G_TYPE_STRING,
                    G_TYPE_STRING,
										G_TYPE_STRING,
										G_TYPE_STRING,
										G_TYPE_STRING,
										G_TYPE_STRING);

static void
exit (void)
{
  gtk_main_quit ();
}

// create a table
static void 
create (void) {
  connection C(lchr);
  work W(C);

  string query = "create table inventory (id text primary key, name text, category text, description text, price decimal, quantity int);";
  W.exec(query);
  W.commit();

}


static GtkTreeModel *
create_and_fill_model (void)
{
  connection C(lchr);


  GtkTreeIter iter;
  gtk_list_store_append (store, &iter);
  // fill the row with data from the database using the sql query select command
  work W(C); 
  try {
    
    result R(W.exec(query_main));
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
  } catch (pqxx::undefined_table) {
    printf("NO TABLE DETECTED");
    // show error message if the table does not exist and ask the user to create it
    GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "Error");
    gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
    gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    GtkWidget *label = gtk_label_new ("Table does not exist. It must be created to use this program");
    GtkWidget *label2 = gtk_label_new ("Would you like DataManage to create this table for you?");
    // make a button labeled create and exit
    GtkWidget *create_button = gtk_button_new_with_label ("Create Table");
    g_signal_connect (create_button, "clicked", G_CALLBACK (create), NULL);

    GtkWidget *exit_button = gtk_button_new_with_label ("Exit");
    g_signal_connect (create_button, "clicked", G_CALLBACK (exit), NULL);

    GtkWidget *grid = gtk_grid_new ();
    gtk_grid_set_column_spacing (GTK_GRID (grid), 10);
    gtk_grid_set_row_spacing (GTK_GRID (grid), 10);
    gtk_container_add (GTK_CONTAINER (window), grid);

    gtk_grid_attach (GTK_GRID (grid), label, 0, 1, 1, 1);
    gtk_grid_attach (GTK_GRID (grid), label2, 0, 2, 1, 1);
    gtk_grid_attach (GTK_GRID (grid), create_button, 0, 1, 1, 1);
    gtk_grid_attach (GTK_GRID (grid), exit_button, 0, 1, 1, 1);


  }
  return 0;
}

// commit data to the database



static GtkWidget *
create_view_and_model (void)
{
  view = gtk_tree_view_new ();
  // create the model and add it to the view
  // populate the cells with the column data
  
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
  // clear the tree view first
  gtk_list_store_clear (store);
  GtkTreeModel *model = create_and_fill_model ();

   
  gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);

  g_object_unref (model);

  return view;


}

static void
commit_data (void)
{
  try {
    connection C(lchr);
    work W(C); 
    string query = "insert into inventory values ('"+ string(gtk_entry_get_text(GTK_ENTRY(id_entry))) + "', '" + string(gtk_entry_get_text(GTK_ENTRY(name_entry))) + "', '" + string(gtk_entry_get_text(GTK_ENTRY(cat_entry))) + "', '" + string(gtk_entry_get_text(GTK_ENTRY(description_entry))) + "', " + string(gtk_entry_get_text(GTK_ENTRY(price_entry))) + ", " + string(gtk_entry_get_text(GTK_ENTRY(quantity_entry))) +")";
    W.exec(query);
    W.commit();
    id = "";
    // destroy the window
    gtk_widget_destroy (add_window);
    // reload the tree view
    GtkWidget *view = create_view_and_model ();

    gtk_widget_grab_focus (search_box);
    // make sure that the tree view is the size of the window
  }
  catch (const std::exception &e) {
    // show the error message
    GtkWidget *dialog = gtk_message_dialog_new (NULL,
                                                GTK_DIALOG_DESTROY_WITH_PARENT,
                                                GTK_MESSAGE_ERROR,
                                                GTK_BUTTONS_CLOSE,
                                                "%s", e.what());
    gtk_dialog_run (GTK_DIALOG (dialog));
  }
}

static void 
delete_data (void) {
  // remove the selected item from the database
  connection C(lchr);
  work W(C);
  string query = "delete from inventory where id = '" + string(gtk_entry_get_text(GTK_ENTRY(id_entry))) + "'";
  W.exec(query);
  W.commit();
  // destroy the window
  gtk_widget_destroy (update_item);
  // reload the model and let the view know we changed the model
  // clear the list store
  GtkWidget *view = create_view_and_model ();

  gtk_widget_grab_focus (search_box);
}
// update the item in the database
static void
update_data (void) {
  // commit the data to the database
  connection C(lchr);
  work W(C);
  // check if id is in the database
  try {
    printf("%s\n", id.c_str());
    string query = "update inventory set id = '" + string(gtk_entry_get_text(GTK_ENTRY(id_entry))) + "', name = '" + string(gtk_entry_get_text(GTK_ENTRY(name_entry))) + "', category = '" + string(gtk_entry_get_text(GTK_ENTRY(cat_entry))) + "', description = '" + string(gtk_entry_get_text(GTK_ENTRY(description_entry))) + "', price = " + string(gtk_entry_get_text(GTK_ENTRY(price_entry))) + ", quantity = " + string(gtk_entry_get_text(GTK_ENTRY(quantity_entry))) + " where id = '" + id.c_str() + "'";
    printf("%s\n", query.c_str());
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
  gtk_widget_destroy (update_item);
  GtkWidget *view = create_view_and_model ();
  gtk_widget_grab_focus (search_box);
    
  }

static GtkWidget * 
add_item (void) {
  
  // add the item to the database
  add_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (add_window), "Add Item");
  gtk_window_set_default_size (GTK_WINDOW (add_window), 200, 200);
  gtk_container_set_border_width (GTK_CONTAINER (add_window), 10);
  gtk_window_set_position (GTK_WINDOW (add_window), GTK_WIN_POS_CENTER);

  // add entries for the new item
  id_entry = gtk_entry_new();
  name_entry = gtk_entry_new();
  cat_entry = gtk_entry_new();
  description_entry = gtk_entry_new();
  price_entry = gtk_entry_new();
  quantity_entry = gtk_entry_new();

  gtk_entry_set_text (GTK_ENTRY (id_entry), id.c_str());
  // make a grid to hold the entries  
  GtkWidget *grid = gtk_grid_new ();
  gtk_grid_set_column_spacing (GTK_GRID (grid), 10);
  gtk_grid_set_row_spacing (GTK_GRID (grid), 10);
  gtk_container_add (GTK_CONTAINER (add_window), grid);
  // add the entries to the grid
  gtk_grid_attach (GTK_GRID (grid), gtk_label_new ("ID"), 0, 0, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), id_entry, 1, 0, 1, 1);
  // prefill the id entry with the next available id
  gtk_entry_set_text (GTK_ENTRY (id_entry), id.c_str());
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
  gtk_widget_show_all (add_window);
  return 0;
}
// search the database for the item
static void
update (string id) {
  // update the item in the database
  update_item = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (update_item), "Item Details");
  gtk_window_set_default_size (GTK_WINDOW (update_item), 200, 200);
  gtk_container_set_border_width (GTK_CONTAINER (update_item), 10);
  gtk_window_set_position (GTK_WINDOW (update_item), GTK_WIN_POS_CENTER);

  // get the item from the database based on the id
  connection C(lchr);
  work W(C);
  result R = W.exec("SELECT * FROM inventory WHERE id = '" + string(id.c_str())+"'");
  // set the entries to the values of the item
  id_entry = gtk_entry_new();
  name_entry = gtk_entry_new();
  cat_entry = gtk_entry_new();
  description_entry = gtk_entry_new();
  price_entry = gtk_entry_new();
  quantity_entry = gtk_entry_new();
  gtk_entry_set_text (GTK_ENTRY (id_entry), id.c_str());
  gtk_entry_set_text (GTK_ENTRY (name_entry), R[0]["name"].c_str());
  gtk_entry_set_text (GTK_ENTRY (cat_entry), R[0]["category"].c_str());
  gtk_entry_set_text (GTK_ENTRY (description_entry), R[0]["description"].c_str());
  gtk_entry_set_text (GTK_ENTRY (price_entry), R[0]["price"].c_str());
  gtk_entry_set_text (GTK_ENTRY (quantity_entry), R[0]["quantity"].c_str());

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

// get the currently selected row
static void
on_row_activated (GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer data)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  model = gtk_tree_view_get_model (treeview);
  gtk_tree_model_get_iter (model, &iter, path);
  gtk_tree_model_get (model, &iter,COL_ID, &id, COL_NAME, &name, COL_CAT, &cat, COL_DESCRIPTION, &description, COL_PRICE, &price, COL_QUANTITY, &quantity, -1);
  update(string(id.c_str()));
  // make a new window that shows details about the item and allow the user to edit it or delete it
}

static void
search_item (void) {
  // search the database for the item
  // get the id from the search box

  id = string(gtk_entry_get_text(GTK_ENTRY(search_box)));
  // clear the search box
  gtk_entry_set_text (GTK_ENTRY (search_box), "");

  // check if the id is in the database
  connection C(lchr);
  work W(C);
  string query = "select * from inventory where id = '";
  query += id + "'";
  result R( W.exec(query));
  // if the id is in the database, display the item
  if (R.size() > 0) {
    // open the update item window
    update(id);


  } else {
    add_item();
  }
  // print the results

}

int main(int argc, char* argv[]) {
  try {
    login();
    connection C(lchr);
  } catch (pqxx::sql_error const &e) {
    // open a dialog box to tell the user that the connection failed
    GtkWidget *dialog = gtk_message_dialog_new (NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Connection Failed");
    gtk_dialog_run (GTK_DIALOG (dialog));

  }
  gtk_init (&argc, &argv);

  main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect (main_window, "destroy", gtk_main_quit, NULL);

  
  // make four buttons along the top edge of the screen 
  GtkWidget *button1 = gtk_button_new_with_label ("Add Item");
  GtkWidget *button3 = gtk_button_new_with_label ("Help");
  GtkWidget *button5 = gtk_button_new_with_label ("Exit");
  // add a search box below the buttons
  search_box = gtk_entry_new ();
  // make the search box say "Type to search"
  GtkWidget *search_label = gtk_label_new ("Type or scan to search ID");
  // make a grid to hold the buttons
  GtkWidget *grid;
  grid = gtk_grid_new();
  // add the buttons to the grid
  gtk_grid_attach (GTK_GRID (grid), button1, 0, 0, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), button3, 3, 0, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), button5, 4, 0, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), search_label, 0, 1, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), search_box, 1, 1, 5, 1);

  g_signal_connect(button1, "clicked", G_CALLBACK(add_item), NULL);
  g_signal_connect(button5, "clicked", G_CALLBACK(exit), NULL);
  // make the search bar highlighted by default
  gtk_widget_grab_focus (search_box);
  // when enter is pressed, search for the item
  g_signal_connect (search_box, "activate", G_CALLBACK (search_item), search_box);
  
  // add scroll bar to the window
  GtkWidget *scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  GtkWidget *view = create_view_and_model ();
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_add (GTK_CONTAINER (scrolled_window), view);
  // make sure that the tree view is the size of the window
  gtk_widget_set_size_request (scrolled_window, 600, 600);
  gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 4, 5, 4);
  // get the value of the item that is selected in the tree view
  g_signal_connect (view, "row-activated", G_CALLBACK (on_row_activated), NULL);


  gtk_container_add (GTK_CONTAINER (main_window), grid);
  // set smallest window size
  gtk_window_set_default_size (GTK_WINDOW (main_window), 300, 300);
  gtk_window_set_title(GTK_WINDOW(main_window), "Online - DataManage");
  gtk_window_set_resizable (GTK_WINDOW (main_window), FALSE);
  gtk_widget_show_all (main_window);

  gtk_main ();

  return 0;
}