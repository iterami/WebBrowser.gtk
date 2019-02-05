#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include "main.h"
#include "../../common/c/core.c"
#include "../../common/c/gtk.c"

void activate(GtkApplication* app, gpointer data){
    GtkAccelGroup *accelgroup;
    GtkWidget *box;
    GtkWidget *menubar;
    GtkWidget *menuitem_file;
    GtkWidget *menuitem_file_closetab;
    GtkWidget *menuitem_file_movetableft;
    GtkWidget *menuitem_file_movetabright;
    GtkWidget *menuitem_file_newtab;
    GtkWidget *menuitem_file_nexttab;
    GtkWidget *menuitem_file_previoustab;
    GtkWidget *menuitem_file_quit;
    GtkWidget *menumenu_file;
    WebKitWebView *view;

    gtk_init_gtk(
      app,
      "WebBrowser.gtk"
    );

    // Setup scrollable notebook.
    notebook = GTK_NOTEBOOK(gtk_notebook_new());
    gtk_notebook_popup_enable(notebook);
    gtk_notebook_set_scrollable(
      notebook,
      TRUE
    );

    // Setup menu items.
    menubar = gtk_menu_bar_new();
    accelgroup = gtk_accel_group_new();
    gtk_window_add_accel_group(
      GTK_WINDOW(window),
      accelgroup
    );
    // File menu.
    menumenu_file = gtk_menu_new();
    menuitem_file = gtk_menu_item_new_with_mnemonic("_File");
    gtk_menu_item_set_submenu(
      GTK_MENU_ITEM(menuitem_file),
      menumenu_file
    );
    menuitem_file_newtab = gtk_add_menuitem(
      menumenu_file,
      "_New Tab",
      accelgroup,
      KEY_NEWTAB,
      GDK_CONTROL_MASK
    );
    gtk_menu_shell_append(
      GTK_MENU_SHELL(menumenu_file),
      gtk_separator_menu_item_new()
    );
    menuitem_file_nexttab = gtk_add_menuitem(
      menumenu_file,
      "_Next Tab",
      accelgroup,
      KEY_NEXTTAB,
      GDK_CONTROL_MASK
    );
    menuitem_file_previoustab = gtk_add_menuitem(
      menumenu_file,
      "_Previous Tab",
      accelgroup,
      KEY_PREVIOUSTAB,
      GDK_CONTROL_MASK
    );
    menuitem_file_movetableft = gtk_add_menuitem(
      menumenu_file,
      "Move Tab _Left",
      accelgroup,
      KEY_MOVETABLEFT,
      GDK_CONTROL_MASK | GDK_SHIFT_MASK
    );
    menuitem_file_movetabright = gtk_add_menuitem(
      menumenu_file,
      "Move Tab _Right",
      accelgroup,
      KEY_MOVETABRIGHT,
      GDK_CONTROL_MASK | GDK_SHIFT_MASK
    );
    gtk_menu_shell_append(
      GTK_MENU_SHELL(menumenu_file),
      gtk_separator_menu_item_new()
    );
    menuitem_file_closetab = gtk_add_menuitem(
      menumenu_file,
      "_Close Tab",
      accelgroup,
      KEY_CLOSETAB,
      GDK_CONTROL_MASK
    );
    menuitem_file_quit = gtk_add_menuitem(
      menumenu_file,
      "_Quit",
      accelgroup,
      KEY_QUIT,
      GDK_CONTROL_MASK
    );
    gtk_menu_shell_append(
      GTK_MENU_SHELL(menubar),
      menuitem_file
    );

    // Setup menu item callbacks.
    g_signal_connect_swapped(
      menuitem_file_closetab,
      "activate",
      G_CALLBACK(menu_closetab),
      NULL
    );
    g_signal_connect_swapped(
      menuitem_file_movetableft,
      "activate",
      G_CALLBACK(menu_movetableft),
      NULL
    );
    g_signal_connect_swapped(
      menuitem_file_movetabright,
      "activate",
      G_CALLBACK(menu_movetabright),
      NULL
    );
    g_signal_connect_swapped(
      menuitem_file_newtab,
      "activate",
      G_CALLBACK(menu_newtab),
      NULL
    );
    g_signal_connect_swapped(
      menuitem_file_nexttab,
      "activate",
      G_CALLBACK(gtk_notebook_next_page),
      notebook
    );
    g_signal_connect_swapped(
      menuitem_file_previoustab,
      "activate",
      G_CALLBACK(gtk_notebook_prev_page),
      notebook
    );
    g_signal_connect_swapped(
      menuitem_file_quit,
      "activate",
      G_CALLBACK(gtk_widget_destroy),
      window
    );

    // Add everything to a box.
    box = gtk_box_new(
      GTK_ORIENTATION_VERTICAL,
      0
    );
    gtk_box_pack_start(
      GTK_BOX(box),
      menubar,
      FALSE,
      FALSE,
      0
    );
    gtk_box_pack_start(
      GTK_BOX(box),
      GTK_WIDGET(notebook),
      TRUE,
      TRUE,
      0
    );
    gtk_container_add(
      GTK_CONTAINER(window),
      box
    );

    // Setup main tab.
    view = WEBKIT_WEB_VIEW(webkit_web_view_new());
    gtk_notebook_append_page(
      notebook,
      GTK_WIDGET(view),
      gtk_label_new("H")
    );
    webkit_web_view_load_uri(
      view,
      "https://iterami.com"
    );

    gtk_widget_show_all(window);
}

int main(int argc, char **argv){
    GtkApplication *app;

    app = gtk_application_new(
      "com.iterami.webbrowsergtk",
      0
    );
    g_signal_connect(
      app,
      "activate",
      G_CALLBACK(activate),
      NULL
    );
    int status = g_application_run(
      G_APPLICATION(app),
      argc,
      argv
    );
    g_object_unref(app);

    return status;
}

void menu_closetab(void){
    if(gtk_notebook_get_current_page(notebook) <= 0){
        return;
    }

    gtk_notebook_remove_page(
      notebook,
      gtk_notebook_get_current_page(notebook)
    );
}

void menu_movetableft(void){
    int position = gtk_notebook_get_current_page(notebook) - 1;
    if(position < 1){
        position = gtk_notebook_get_n_pages(notebook) - 1;
    }

    gtk_notebook_reorder_child(
      notebook,
      gtk_notebook_get_nth_page(
        notebook,
        gtk_notebook_get_current_page(notebook)
      ),
      position
    );
}

void menu_movetabright(void){
    int position = gtk_notebook_get_current_page(notebook) + 1;
    if(position >= gtk_notebook_get_n_pages(notebook)){
        position = 1;
    }

    gtk_notebook_reorder_child(
      notebook,
      gtk_notebook_get_nth_page(
        notebook,
        gtk_notebook_get_current_page(notebook)
      ),
      position
    );
}

void menu_newtab(void){
    GtkWidget *box;
    GtkWidget *innerbox;
    WebKitWebView *view;

    box = gtk_box_new(
      GTK_ORIENTATION_VERTICAL,
      0
    );
    innerbox = gtk_box_new(
      GTK_ORIENTATION_HORIZONTAL,
      0
    );

    // Setup tab toolbar.
    gtk_box_pack_start(
      GTK_BOX(innerbox),
      gtk_button_new_with_label("Back"),
      FALSE,
      FALSE,
      0
    );
    gtk_box_pack_start(
      GTK_BOX(innerbox),
      gtk_button_new_with_label("Forward"),
      FALSE,
      FALSE,
      0
    );
    gtk_box_pack_start(
      GTK_BOX(innerbox),
      gtk_button_new_with_label("Reload"),
      FALSE,
      FALSE,
      0
    );
    gtk_box_pack_start(
      GTK_BOX(innerbox),
      gtk_button_new_with_label("Stop"),
      FALSE,
      FALSE,
      0
    );
    gtk_box_pack_start(
      GTK_BOX(innerbox),
      gtk_entry_new(),
      TRUE,
      TRUE,
      0
    );
    gtk_box_pack_start(
      GTK_BOX(box),
      innerbox,
      FALSE,
      FALSE,
      0
    );

    // Setup tab view.
    view = WEBKIT_WEB_VIEW(webkit_web_view_new());
    gtk_box_pack_start(
      GTK_BOX(box),
      GTK_WIDGET(view),
      TRUE,
      TRUE,
      0
    );

    // Append and show.
    gtk_notebook_append_page(
      notebook,
      GTK_WIDGET(box),
      gtk_label_new("UNSAVED")
    );
    gtk_widget_show_all(window);
    gtk_notebook_set_current_page(
      notebook,
      gtk_notebook_get_n_pages(notebook) - 1
    );
    gtk_widget_grab_focus(GTK_WIDGET(view));

    webkit_web_view_load_uri(
      view,
      "https://iterami.com"
    );
}
