#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

GtkWidget *window;
GtkWidget *grid;
GtkWidget *button;
GtkWidget *entry1;
GtkWidget *label;
float x;
float y;
bool first;
float ans;
int function;
int xi;
int yi;
float prev1;
float prev2;
float prev3;
function = 0; //0 = nothing, 1 = add, 2 = subtract, 3 = multiply, 4 = divide

static void clear (GtkWidget *widget,
             gpointer   data)
{
  first = false;
  x = 0;
  y = 0;
  gtk_entry_set_text(GTK_ENTRY(entry1), g_strdup_printf(""));

}
static void callback( GtkWidget *widget, gpointer data)
{
    const gchar *entry_text1;
	entry_text1 = gtk_entry_get_text (GTK_ENTRY (entry1));
	//split the string into two floats and store them in x and y split the number on the operator that is used to determine the function 
	char *pch;
	char *str = strdup(entry_text1);
	pch = strtok (str,"+-*/&^|!%");
	x = atof(pch);
	xi = x;
	pch = strtok (NULL, "+-*/&^|!%");
	y = atof(pch);
	yi = y; 
	
	if (strchr(entry_text1, '+') != NULL) {
		printf("%f\n", x);
		printf("%f\n", y);
		ans = x + y;
		gtk_entry_set_text(GTK_ENTRY(entry1), g_strdup_printf("%f", ans));

	}
	if (strchr(entry_text1, '-') != NULL) {
		printf("%f\n", x);
		printf("%f\n", y);
		ans = x - y;
		gtk_entry_set_text(GTK_ENTRY(entry1), g_strdup_printf("%f", ans));

	}

	if (strchr(entry_text1, '*') != NULL) {
		printf("%f\n", x);
		printf("%f\n", y);
		ans = x * y;
		gtk_entry_set_text(GTK_ENTRY(entry1), g_strdup_printf("%f", ans));

	}
	if (strchr(entry_text1, '/') != NULL) {
		printf("%f\n", x);
		printf("%f\n", y);
		ans = x / y;
		gtk_entry_set_text(GTK_ENTRY(entry1), g_strdup_printf("%f", ans));

	}
	if (strchr(entry_text1, '&') != NULL) {
		printf("%f\n", x);
		printf("%f\n", y);
		ans = xi & yi;
		printf("%d\n", ans);
		gtk_entry_set_text(GTK_ENTRY(entry1), g_strdup_printf("%d", ans));

	}
	if (strchr(entry_text1, '|') != NULL) {
		printf("%f\n", x);
		printf("%f\n", y);
		ans = xi | yi;
		printf("%d\n", ans);
		gtk_entry_set_text(GTK_ENTRY(entry1), g_strdup_printf("%d", ans));

	}
	if (strchr(entry_text1, '!') != NULL) {
		printf("%f\n", x);
		printf("%f\n", y);
		ans = !xi;
		printf("%d\n", ans);
		gtk_entry_set_text(GTK_ENTRY(entry1), g_strdup_printf("%d", ans));

	}
	if (strchr(entry_text1, '%') != NULL) {
		printf("%f\n", x);
		printf("%f\n", y);
		ans = xi % yi;
		printf("%d\n", ans); 
		gtk_entry_set_text(GTK_ENTRY(entry1), g_strdup_printf("%d", ans));

	}

	// update the history
	gtk_label_set_text(GTK_LABEL(label), g_strdup_printf("%f", ans));




    g_print ("%s was pressed\n", (char *) data);


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
  gtk_grid_set_row_spacing (GTK_GRID (grid), 5);


  entry1 = gtk_entry_new ();
  gtk_grid_attach (GTK_GRID (grid), entry1, 0, 0, 3, 1);
  g_signal_connect (entry1, "activate", G_CALLBACK (callback), "entry1");

  //show history up to 5 numbers
  label = gtk_label_new ("");
  gtk_grid_attach (GTK_GRID (grid), label, 0, 1, 1, 1);


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