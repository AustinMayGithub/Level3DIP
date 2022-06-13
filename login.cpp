#include <gtk/gtk.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

using namespace std;
// set up the varibles used to store the users login information
const char *username;
const char *password;
const char *database;
const char *ip;
const char *port;
// set up the gtk widgets 
GtkWidget *password_entry;
GtkWidget *username_entry;
GtkWidget *database_entry;
GtkWidget *ip_entry;
GtkWidget *port_entry;

static void
save_info (GtkWidget *widget,
             gpointer   data)
{
  password = gtk_entry_get_text(GTK_ENTRY(password_entry));
  username = gtk_entry_get_text(GTK_ENTRY(username_entry));
  database = gtk_entry_get_text(GTK_ENTRY(database_entry));
  ip = gtk_entry_get_text(GTK_ENTRY(ip_entry));
  port = gtk_entry_get_text(GTK_ENTRY(port_entry));
  ofstream out("data.dmf");
  out << "dbname=" << database << " user=" << username << " password=" << password << " host=" << ip << " port=" << port;
  out.close();
  gtk_main_quit();
  system("./main");
  
}

static void
login (GtkWidget *widget,
             gpointer   data)
{
  
  gtk_main_quit();
  system("./main");
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *button; 

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Login - DataManage");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
  gtk_container_set_border_width (GTK_CONTAINER (window), 20);

  //add entry boxes for username and password
  GtkWidget *grid;
  grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), grid);
  gtk_widget_show(grid);
  GtkWidget *label;
  label = gtk_label_new(NULL);
  gtk_label_set_markup(GTK_LABEL(label), "<span font_desc=\"18.0\"><i><b>Welcome to DataManage!</b></i></span>");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 2, 1);
  label = gtk_label_new(NULL);
  

  GtkWidget *username_label;
  username_label = gtk_label_new("Username:");
  gtk_grid_attach(GTK_GRID(grid), username_label, 0, 2, 1, 1);
  gtk_widget_show(username_label);
  //create a text entry box to hold the item name

  username_entry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), username_entry, 1, 2, 1, 1);
  // password
  GtkWidget *password_label;
  password_label = gtk_label_new("Password:");
  gtk_grid_attach(GTK_GRID(grid), password_label, 0, 3, 1, 1);
  //create a text entry box to hold the item name
  password_entry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), password_entry, 1, 3, 1, 1);

  GtkWidget *database_label;
  database_label = gtk_label_new("Database:");
  gtk_grid_attach(GTK_GRID(grid), database_label, 0, 4, 1, 1);

  database_entry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), database_entry, 1, 4, 1, 1);

  GtkWidget *ip_label;
  ip_label = gtk_label_new("IP Address:");
  gtk_grid_attach(GTK_GRID(grid), ip_label, 0, 5, 1, 1);

  ip_entry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), ip_entry, 1, 5, 1, 1);

  GtkWidget *port_label;
  port_label = gtk_label_new("Port:");
  gtk_grid_attach(GTK_GRID(grid), port_label, 0, 6, 1, 1);

  port_entry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), port_entry, 1, 6, 1, 1);

  //create a button to submit the username and password
  button = gtk_button_new_with_label("Connect");
  g_signal_connect (button, "clicked", G_CALLBACK (save_info), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 0, 7, 2, 1);

  button = gtk_button_new_with_label ("Previous Session Login");
  g_signal_connect (button, "clicked", G_CALLBACK (login), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 0, 8, 2, 1);
  gtk_container_add (GTK_CONTAINER (window), grid);

  gtk_widget_show_all (window);
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.datamanage", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}