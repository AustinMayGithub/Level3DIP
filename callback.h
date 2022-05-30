#include <gtk/gtk.h>

void cb_create_entry(GtkWidget *p_widget, gpointer user_data){
    gtk_button_released(p_widget);
    GtkWidget *p_window;
    GtkWidget *p_v_box;
    GtkWidget *p_entry;

    p_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(p_window), "Create DB");
    gtk_window_set_default_size(GTK_WINDOW(p_window), 320, 200);

    p_v_box = gtk_vbox_new(TRUE, 0);
    gtk_container_add(GTK_CONTAINER(p_window), p_v_box);

    p_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(p_v_box), p_entry, TRUE, FALSE, 0);
    gtk_widget_show_all(p_window);
}