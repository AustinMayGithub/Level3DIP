#include <gtk/gtk.h>
#include "callback.h"

void cb_create_entry(GtkWidget *, gpointer);

int create_window(int argc, char *argv[]){
   GtkWidget *p_window = NULL;
   GtkWidget *p_main_box = NULL;

   GtkWidget *p_button[5];

   gtk_init (&argc, &argv);

   //Create window
   p_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

   gtk_window_set_title(GTK_WINDOW(p_window), "Hello");
   gtk_window_set_default_size(GTK_WINDOW(p_window), 320, 200);

   p_main_box = gtk_vbox_new(TRUE, 0);
   gtk_container_add(GTK_CONTAINER(p_window), p_main_box);

   p_button[0] = gtk_button_new_with_label("Create entry");
   g_signal_connect(G_OBJECT(p_button[0]), "clicked", 
   G_CALLBACK(cb_create_entry), NULL);
   gtk_box_pack_start(GTK_BOX(p_main_box), p_button[0], FALSE, FALSE, 0);
   // get data from entry
   gtk_entry_get_text(GTK_ENTRY(p_entry));

   gtk_widget_show_all(p_window);
}

int main (int argc, char *argv[]) {
   create_window(argc, argv);
   gtk_main ();
   return 0;
}