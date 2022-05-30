#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>

GtkWidget *window;
GtkWidget *grid;
GtkWidget *button;
GtkWidget *entry1;
GtkWidget *entry2;
float x;
float y;
bool first;
float ans;
int function;
function = 0; //0 = nothing, 1 = add, 2 = subtract, 3 = multiply, 4 = divide


static void
add (GtkWidget *widget,
             gpointer   data)
{
  if (first == true) {

    ans = x + y;
    g_print("%f\n", ans);
    //set the text in the entry to the answer
    gtk_entry_set_text(GTK_ENTRY(entry1), g_strdup_printf("%f", ans));
  }
  if (first == false) {
    function = 1;
    y = x;
    x = 0;
    first = true;
    gtk_entry_set_text(GTK_ENTRY(entry1), g_strdup_printf(""));
  }


}
static void
sub (GtkWidget *widget,
             gpointer   data)
{
  if (first == true) {
    ans = x - y;
    g_print("%f\n", ans);
    //set the text in the entry to the answer
    gtk_entry_set_text(GTK_ENTRY(entry1), g_strdup_printf("%f", ans));
  }
  if (first == false) {
    function = 2;
    y = x;
    x = 0;
    first = true;
    gtk_entry_set_text(GTK_ENTRY(entry1), g_strdup_printf(""));
  }


}
static void
divide (GtkWidget *widget,
             gpointer   data)
{
  if (first == true) {
    if (x == 0 || y == 0){
      g_print("Error\n");
      gtk_entry_set_text(GTK_ENTRY(entry1), g_strdup_printf("Error"));
    }
    else {
      ans = x / y;
      g_print("%f\n", ans);
    //set the text in the entry to the answer
      gtk_entry_set_text(GTK_ENTRY(entry1), g_strdup_printf("%f", ans));
    }
  }
  if (first == false) {
    function = 4;
    y = x;
    x = 0;
    first = true;
    gtk_entry_set_text(GTK_ENTRY(entry1), g_strdup_printf(""));
  }


}
static void
mul (GtkWidget *widget,
             gpointer   data)
{
  if (first == true) {
    ans = x * y;
    g_print("%f\n", ans);
    //set the text in the entry to the answer
    gtk_entry_set_text(GTK_ENTRY(entry1), g_strdup_printf("%f", ans));
  }
  if (first == false) {
    function = 3;
    y = x;
    first = true;
    x = 0;
    gtk_entry_set_text(GTK_ENTRY(entry1), g_strdup_printf(""));
  }


}
static void clear (GtkWidget *widget,
             gpointer   data)
{
  first = false;
  function = 0;
  x = 0;
  y = 0;
  gtk_entry_set_text(GTK_ENTRY(entry1), g_strdup_printf(""));

}

static void callback( GtkWidget *widget, gpointer data)
{
    const gchar *entry_text1;
    const gchar *entry_text2;
    if (data == "0") {
      x = x*10;
      x = x + 0;
    }
    if (data == "1") {
      x = x*10;
      x = x + 1;
    }
    if (data == "2") {
      x = x*10;
      x = x + 2;
    }
    if (data == "3") {
      x = x*10;
      x = x + 3;
    }
    if (data == "4") {
      x = x*10;
      x = x + 4;
    }
    if (data == "5") {
      x = x*10;
      x = x + 5;
    }
    if (data == "6") {
      x = x*10;
      x = x + 6;
    }
    if (data == "7") {
      x = x*10;
      x = x + 7;
    }
    if (data == "8") {
      x = x*10;
      x = x + 8;
    }
    if (data == "9") {
      x = x*10;
      x = x + 9;
    }
    if (data == ".") {
      x = x/10;
    }
    gtk_entry_set_text(GTK_ENTRY(entry1), g_strdup_printf("%f", x));
    g_print ("%s was pressed\n", (char *) data);
    
    entry_text1 = gtk_entry_get_text (GTK_ENTRY (entry1));
    sscanf(entry_text1, "%f", &x);
    if (first == true) {
      // execute the function
      if (function == 1) {
        ans = x + y;
        g_print("%f\n", ans);
        //set the text in the entry to the answer
        gtk_entry_set_text(GTK_ENTRY(entry1), g_strdup_printf("%f", ans));
      }
      if (function == 2) {
        ans = x - y;
        g_print("%f\n", ans);
        //set the text in the entry to the answer
        gtk_entry_set_text(GTK_ENTRY(entry1), g_strdup_printf("%f", ans));
      }
      if (function == 3) {
        ans = x * y;
        g_print("%f\n", ans);
        //set the text in the entry to the answer
        gtk_entry_set_text(GTK_ENTRY(entry1), g_strdup_printf("%f", ans));
      }
      if (function == 4) {
        if (x == 0 || y == 0){
          g_print("Error\n");
          gtk_entry_set_text(GTK_ENTRY(entry1), g_strdup_printf("Error"));
        }
        else {
          ans = x / y;
          g_print("%f\n", ans);
          //set the text in the entry to the answer
          gtk_entry_set_text(GTK_ENTRY(entry1), g_strdup_printf("%f", ans));
        }
      }

    }

}


static void
activate (GtkApplication *app,
          gpointer        user_data)
{

  /* create a new window, and set its title */
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Calculator");
  gtk_container_set_border_width (GTK_CONTAINER (window), 10);

  /* Here we construct the container that is going pack our buttons */
  grid = gtk_grid_new ();

  /* Pack the container in the window */
  gtk_container_add (GTK_CONTAINER (window), grid);

  button = gtk_button_new_with_label ("Clear");
  g_signal_connect (button, "clicked", G_CALLBACK (clear), NULL);

  gtk_grid_attach (GTK_GRID (grid), button, 3, 0, 1, 1);

  button = gtk_button_new_with_label ("7");
  g_signal_connect (button, "clicked", G_CALLBACK (callback), "7");
  gtk_grid_attach (GTK_GRID (grid), button, 0, 1, 1, 1);

  button = gtk_button_new_with_label ("8");
  g_signal_connect (button, "clicked", G_CALLBACK (callback), "8");
  gtk_grid_attach (GTK_GRID (grid), button, 1, 1, 1, 1);

  button = gtk_button_new_with_label ("9");
  g_signal_connect (button, "clicked", G_CALLBACK (callback), "9");
  gtk_grid_attach (GTK_GRID (grid), button, 2, 1, 1, 1);

  button = gtk_button_new_with_label ("6");
  g_signal_connect (button, "clicked", G_CALLBACK (callback), "6");
  gtk_grid_attach (GTK_GRID (grid), button, 2, 2, 1, 1);

  button = gtk_button_new_with_label ("5");
  g_signal_connect (button, "clicked", G_CALLBACK (callback), "5");
  gtk_grid_attach (GTK_GRID (grid), button, 1, 2, 1, 1);

  button = gtk_button_new_with_label ("4");
  g_signal_connect (button, "clicked", G_CALLBACK (callback), "4");
  gtk_grid_attach (GTK_GRID (grid), button, 0, 2, 1, 1);

  button = gtk_button_new_with_label ("3");
  g_signal_connect (button, "clicked", G_CALLBACK (callback), "3");
  gtk_grid_attach (GTK_GRID (grid), button, 2, 3, 1, 1);

  button = gtk_button_new_with_label ("2");
  g_signal_connect (button, "clicked", G_CALLBACK (callback), "2");
  gtk_grid_attach (GTK_GRID (grid), button, 1, 3, 1, 1);

  button = gtk_button_new_with_label ("1");
  g_signal_connect (button, "clicked", G_CALLBACK (callback), "1");
  gtk_grid_attach (GTK_GRID (grid), button, 0, 3, 1, 1);

  button = gtk_button_new_with_label ("0");
  g_signal_connect (button, "clicked", G_CALLBACK (callback), "0");
  gtk_grid_attach (GTK_GRID (grid), button, 0, 4, 2, 1);

  button = gtk_button_new_with_label (".");
  g_signal_connect (button, "clicked", G_CALLBACK (callback), ".");
  gtk_grid_attach (GTK_GRID (grid), button, 2, 4, 1, 1);

  button = gtk_button_new_with_label ("Add");
  g_signal_connect (button, "clicked", G_CALLBACK (add), NULL);

  gtk_grid_attach (GTK_GRID (grid), button, 0, 5, 1, 1);

  button = gtk_button_new_with_label ("Sub");
  g_signal_connect (button, "clicked", G_CALLBACK (sub), NULL);

  gtk_grid_attach (GTK_GRID (grid), button, 1, 5, 1, 1);

  button = gtk_button_new_with_label ("Mul");
  g_signal_connect (button, "clicked", G_CALLBACK (mul), NULL);

  gtk_grid_attach (GTK_GRID (grid), button, 2, 5, 1, 1);
  
  button = gtk_button_new_with_label ("Div");
  g_signal_connect (button, "clicked", G_CALLBACK (divide), NULL);

  gtk_grid_attach (GTK_GRID (grid), button, 1, 6, 1, 1);

  button = gtk_button_new_with_label ("=");
  g_signal_connect (button, "clicked", G_CALLBACK (callback), "=");
  gtk_grid_attach (GTK_GRID (grid), button, 2, 6, 1, 1);

  //space between the buttons and the entry
  gtk_grid_set_row_spacing (GTK_GRID (grid), 5);


  entry1 = gtk_entry_new ();
  gtk_grid_attach (GTK_GRID (grid), entry1, 0, 0, 3, 1);
  g_signal_connect (entry1, "activate", G_CALLBACK (callback), "entry1");

  gtk_widget_show_all (window);

}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.calculator", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}