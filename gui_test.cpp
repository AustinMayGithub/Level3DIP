#include <gtk/gtk.h>
// include gtk extra
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

GtkWidget *window;
GtkWidget *grid;
GtkWidget *display;
GtkWidget *button;
GtkWidget *entry1;
GtkWidget *out_label;
enum
{
  COLUMN_ID,
  COLUMN_NAME,
  COLUMN_DESCRIPTION,
  COLUMN_PRICE,
  COLUMN_QUANTITY,
  NUM_COLUMNS
};

char *entry_text1;

static void update( GtkWidget *widget, gpointer data)
{
  //make a new window so that an item can be updated
  GtkWidget *update_window;
  update_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(update_window), "Update Item");
  gtk_window_set_default_size(GTK_WINDOW(update_window), 300, 200);
  gtk_window_set_position(GTK_WINDOW(update_window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(update_window), 10);
  gtk_widget_show(update_window);
  //create a grid to hold the widgets
  GtkWidget *update_grid;
  update_grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(update_window), update_grid);
  gtk_widget_show(update_grid);
  //create a label to display the item name
  GtkWidget *update_label;
  update_label = gtk_label_new("Item Name:");
  gtk_grid_attach(GTK_GRID(update_grid), update_label, 0, 0, 1, 1);
  gtk_widget_show(update_label);
  //create a text entry box to hold the item name
  GtkWidget *update_entry1;
  update_entry1 = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(update_grid), update_entry1, 1, 0, 1, 1);
  gtk_widget_show(update_entry1);


  
}

static void add( GtkWidget *widget, gpointer data)
{
  //make a new window
  GtkWidget *add_window;
  add_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(add_window), "Add Item to Database");
  gtk_window_set_default_size(GTK_WINDOW(add_window), 300, 200);
  gtk_window_set_position(GTK_WINDOW(add_window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(add_window), 10);
  gtk_widget_show(add_window);
  //create a grid to hold the widgets
  GtkWidget *add_grid;
  add_grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(add_window), add_grid);
  gtk_widget_show(add_grid);
  //create a label to display the item name
  GtkWidget *add_item_name_label;
  add_item_name_label = gtk_label_new("Item Name:");
  gtk_grid_attach(GTK_GRID(add_grid), add_item_name_label, 0, 0, 1, 1);
  gtk_widget_show(add_item_name_label);
  //create a text entry box to hold the item name
  GtkWidget *add_item_name_entry1;
  add_item_name_entry1 = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(add_grid), add_item_name_entry1, 1, 0, 1, 1);
  gtk_widget_show(add_item_name_entry1);

  GtkWidget *add_item_category_label;
  add_item_category_label = gtk_label_new("Item Category:");
  gtk_grid_attach(GTK_GRID(add_grid), add_item_category_label, 0, 1, 1, 1);
  gtk_widget_show(add_item_category_label);
  //create a text entry box to hold the item name
  GtkWidget *add_item_category_entry1;
  add_item_category_entry1 = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(add_grid), add_item_category_entry1, 1, 1, 2, 1);
  gtk_widget_show(add_item_category_entry1);
  // item description
  GtkWidget *add_item_description_label;
  add_item_description_label = gtk_label_new("Item Description:");
  gtk_grid_attach(GTK_GRID(add_grid), add_item_description_label, 0, 2, 1, 1);
  gtk_widget_show(add_item_description_label);
  //create a text entry box to hold the item name
  GtkWidget *add_item_description_entry1;
  add_item_description_entry1 = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(add_grid), add_item_description_entry1, 1, 2, 1, 1);
  gtk_widget_show(add_item_description_entry1);
  // item price
  GtkWidget *add_item_price_label;
  add_item_price_label = gtk_label_new("Item Price:");
  gtk_grid_attach(GTK_GRID(add_grid), add_item_price_label, 0, 3, 1, 1);
  gtk_widget_show(add_item_price_label);
  //create a text entry box to hold the item name
  GtkWidget *add_item_price_entry1;
  add_item_price_entry1 = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(add_grid), add_item_price_entry1, 1, 3, 1, 1);
  gtk_widget_show(add_item_price_entry1);
  // item quantity
  GtkWidget *add_item_quantity_label;
  add_item_quantity_label = gtk_label_new("Item Quantity:");
  gtk_grid_attach(GTK_GRID(add_grid), add_item_quantity_label, 0, 4, 1, 1);
  gtk_widget_show(add_item_quantity_label);
  //create a text entry box to hold the item name
  GtkWidget *add_item_quantity_entry1;
  add_item_quantity_entry1 = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(add_grid), add_item_quantity_entry1, 1, 4, 1, 1);
  gtk_widget_show(add_item_quantity_entry1);


}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  // set smallest size of window
  
  
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "DataManage");
  gtk_container_set_border_width (GTK_CONTAINER (window), 10);
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

  /* Here we construct the container that is going pack our buttons */
  grid = gtk_grid_new ();

  /* Pack the container in the window */
  gtk_container_add (GTK_CONTAINER (window), grid);
  // add large box that contains all the entries (using tree view)
  GtkWidget *large_box;
  large_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add(GTK_CONTAINER(grid), large_box);
  gtk_widget_show(large_box);
  // add a label to the large box

  GtkWidget *add_button;
  add_button = gtk_button_new_with_label("Add Item");
  gtk_grid_attach(GTK_GRID(grid), add_button, 0, 0, 1, 1);
  gtk_widget_show(add_button);
  // when the add button is clicked, call the add function
  g_signal_connect(add_button, "clicked", G_CALLBACK(add), NULL);
  
  GtkWidget *delete_button;
  delete_button = gtk_button_new_with_label("Delete Item");
  gtk_grid_attach(GTK_GRID(grid), delete_button, 1, 0, 1, 1);
  gtk_widget_show(delete_button);
  GtkWidget *update_button;
  update_button = gtk_button_new_with_label("Update Item");
  gtk_grid_attach(GTK_GRID(grid), update_button, 2, 0, 1, 1);
  gtk_widget_show(update_button);
  GtkWidget *search_button;
  search_button = gtk_button_new_with_label("Search Item");
  gtk_grid_attach(GTK_GRID(grid), search_button, 3, 0, 1, 1);
  gtk_widget_show(search_button);



  gtk_widget_show_all (window);
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.shoppinglist", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}