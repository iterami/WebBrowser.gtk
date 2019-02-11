#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include "main.h"
#include "../../common/c/core.c"
#include "../../common/c/gtk.c"

void activate(GtkApplication* app, gpointer data){
    GtkAccelGroup *accelgroup;
    GtkWidget *box;
    GtkWidget *menubar;
    GtkWidget *menuitem_menu;
    GtkWidget *menuitem_menu_closetab;
    GtkWidget *menuitem_menu_copy;
    GtkWidget *menuitem_menu_cut;
    GtkWidget *menuitem_menu_delete;
    GtkWidget *menuitem_menu_insert;
    GtkWidget *menuitem_menu_movetableft;
    GtkWidget *menuitem_menu_movetabright;
    GtkWidget *menuitem_menu_newtab;
    GtkWidget *menuitem_menu_nexttab;
    GtkWidget *menuitem_menu_paste;
    GtkWidget *menuitem_menu_previoustab;
    GtkWidget *menuitem_menu_quit;
    GtkWidget *menuitem_menu_selectall;
    GtkWidget *menumenu_menu;
    GtkWidget *toolbar;
    WebKitWebView *view;

    gtk_init_gtk(
      app,
      "WebBrowser.gtk"
    );

    // Setup menu items.
    menubar = gtk_menu_bar_new();
    accelgroup = gtk_accel_group_new();
    gtk_window_add_accel_group(
      GTK_WINDOW(window),
      accelgroup
    );
    // Menu menu.
    menumenu_menu = gtk_menu_new();
    menuitem_menu = gtk_menu_item_new_with_mnemonic("☰");
    gtk_menu_item_set_submenu(
      GTK_MENU_ITEM(menuitem_menu),
      menumenu_menu
    );
    menuitem_menu_newtab = gtk_add_menuitem(
      menumenu_menu,
      "_New Tab",
      accelgroup,
      KEY_NEWTAB,
      GDK_CONTROL_MASK
    );
    gtk_menu_shell_append(
      GTK_MENU_SHELL(menumenu_menu),
      gtk_separator_menu_item_new()
    );
    menuitem_menu_copy = gtk_add_menuitem(
      menumenu_menu,
      "_Copy",
      accelgroup,
      KEY_COPY,
      GDK_CONTROL_MASK
    );
    menuitem_menu_cut = gtk_add_menuitem(
      menumenu_menu,
      "Cu_t",
      accelgroup,
      KEY_CUT,
      GDK_CONTROL_MASK
    );
    menuitem_menu_paste = gtk_add_menuitem(
      menumenu_menu,
      "_Paste",
      accelgroup,
      KEY_PASTE,
      GDK_CONTROL_MASK
    );
    menuitem_menu_delete = gtk_add_menuitem(
      menumenu_menu,
      "_Delete",
      accelgroup,
      KEY_DELETE,
      0
    );
    menuitem_menu_insert = gtk_add_menuitem(
      menumenu_menu,
      "Toggle _Overwrite",
      accelgroup,
      KEY_INSERT,
      0
    );
    menuitem_menu_selectall = gtk_add_menuitem(
      menumenu_menu,
      "_Select All",
      accelgroup,
      KEY_SELECTALL,
      GDK_CONTROL_MASK
    );
    gtk_menu_shell_append(
      GTK_MENU_SHELL(menumenu_menu),
      gtk_separator_menu_item_new()
    );
    menuitem_menu_nexttab = gtk_add_menuitem(
      menumenu_menu,
      "_Next Tab",
      accelgroup,
      KEY_NEXTTAB,
      GDK_CONTROL_MASK
    );
    menuitem_menu_previoustab = gtk_add_menuitem(
      menumenu_menu,
      "_Previous Tab",
      accelgroup,
      KEY_PREVIOUSTAB,
      GDK_CONTROL_MASK
    );
    menuitem_menu_movetableft = gtk_add_menuitem(
      menumenu_menu,
      "Move Tab _Left",
      accelgroup,
      KEY_MOVETABLEFT,
      GDK_CONTROL_MASK | GDK_SHIFT_MASK
    );
    menuitem_menu_movetabright = gtk_add_menuitem(
      menumenu_menu,
      "Move Tab _Right",
      accelgroup,
      KEY_MOVETABRIGHT,
      GDK_CONTROL_MASK | GDK_SHIFT_MASK
    );
    gtk_menu_shell_append(
      GTK_MENU_SHELL(menumenu_menu),
      gtk_separator_menu_item_new()
    );
    menuitem_menu_closetab = gtk_add_menuitem(
      menumenu_menu,
      "_Close Tab",
      accelgroup,
      KEY_CLOSETAB,
      GDK_CONTROL_MASK
    );
    menuitem_menu_quit = gtk_add_menuitem(
      menumenu_menu,
      "_Quit",
      accelgroup,
      KEY_QUIT,
      GDK_CONTROL_MASK
    );
    gtk_menu_shell_append(
      GTK_MENU_SHELL(menubar),
      menuitem_menu
    );

    // Setup toolbar.
    toolbar = gtk_box_new(
      GTK_ORIENTATION_HORIZONTAL,
      0
    );
    gtk_box_pack_start(
      GTK_BOX(toolbar),
      GTK_WIDGET(menubar),
      FALSE,
      FALSE,
      0
    );
    button_toolbar_back = gtk_button_new_with_label("←");
    gtk_box_pack_start(
      GTK_BOX(toolbar),
      button_toolbar_back,
      FALSE,
      FALSE,
      0
    );
    button_toolbar_forward = gtk_button_new_with_label("→");
    gtk_box_pack_start(
      GTK_BOX(toolbar),
      button_toolbar_forward,
      FALSE,
      FALSE,
      0
    );
    button_toolbar_reload = gtk_button_new_with_label("⟳");
    gtk_box_pack_start(
      GTK_BOX(toolbar),
      button_toolbar_reload,
      FALSE,
      FALSE,
      0
    );
    button_toolbar_stop = gtk_button_new_with_label("X");
    gtk_box_pack_start(
      GTK_BOX(toolbar),
      button_toolbar_stop,
      FALSE,
      FALSE,
      0
    );
    entry_toolbar_address = gtk_entry_new();
    g_signal_connect(
      entry_toolbar_address,
      "activate",
      G_CALLBACK(toolbar_address_activate),
      NULL
    );
    gtk_box_pack_start(
      GTK_BOX(toolbar),
      entry_toolbar_address,
      TRUE,
      TRUE,
      0
    );
    g_signal_connect(
      button_toolbar_back,
      "clicked",
      G_CALLBACK(toolbar_back),
      NULL
    );
    g_signal_connect(
      button_toolbar_forward,
      "clicked",
      G_CALLBACK(toolbar_forward),
      NULL
    );
    g_signal_connect(
      button_toolbar_reload,
      "clicked",
      G_CALLBACK(toolbar_reload),
      NULL
    );
    g_signal_connect(
      button_toolbar_stop,
      "clicked",
      G_CALLBACK(toolbar_stop),
      NULL
    );

    // Setup scrollable notebook.
    notebook = GTK_NOTEBOOK(gtk_notebook_new());
    gtk_notebook_popup_enable(notebook);
    gtk_notebook_set_scrollable(
      notebook,
      TRUE
    );
    g_signal_connect_after(
      notebook,
      "switch-page",
      G_CALLBACK(tab_switch),
      NULL
    );

    // Setup menu item callbacks.
    g_signal_connect_swapped(
      menuitem_menu_closetab,
      "activate",
      G_CALLBACK(menu_closetab),
      NULL
    );
    g_signal_connect_swapped(
      menuitem_menu_movetableft,
      "activate",
      G_CALLBACK(menu_movetableft),
      NULL
    );
    g_signal_connect_swapped(
      menuitem_menu_movetabright,
      "activate",
      G_CALLBACK(menu_movetabright),
      NULL
    );
    g_signal_connect_swapped(
      menuitem_menu_newtab,
      "activate",
      G_CALLBACK(menu_newtab),
      NULL
    );
    g_signal_connect_swapped(
      menuitem_menu_nexttab,
      "activate",
      G_CALLBACK(gtk_notebook_next_page),
      notebook
    );
    g_signal_connect_swapped(
      menuitem_menu_previoustab,
      "activate",
      G_CALLBACK(gtk_notebook_prev_page),
      notebook
    );
    g_signal_connect_swapped(
      menuitem_menu_quit,
      "activate",
      G_CALLBACK(gtk_widget_destroy),
      window
    );

    // Disable nonfunctional menu items.
    gtk_widget_set_sensitive(
      menuitem_menu_copy,
      FALSE
    );
    gtk_widget_set_sensitive(
      menuitem_menu_cut,
      FALSE
    );
    gtk_widget_set_sensitive(
      menuitem_menu_delete,
      FALSE
    );
    gtk_widget_set_sensitive(
      menuitem_menu_insert,
      FALSE
    );
    gtk_widget_set_sensitive(
      menuitem_menu_paste,
      FALSE
    );
    gtk_widget_set_sensitive(
      menuitem_menu_selectall,
      FALSE
    );

    // Add everything to a box.
    box = gtk_box_new(
      GTK_ORIENTATION_VERTICAL,
      0
    );
    gtk_box_pack_start(
      GTK_BOX(box),
      toolbar,
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

    // Setup home tab.
    menu_newtab();
    gtk_notebook_set_tab_label_text(
      notebook,
      gtk_notebook_get_nth_page(
        notebook,
        0
      ),
      "⌂"
    );

    gtk_widget_show_all(window);
}

WebKitWebView* get_tab_view(void){
    return WEBKIT_WEB_VIEW(gtk_notebook_get_nth_page(
      notebook,
      gtk_notebook_get_current_page(notebook)
    ));
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
    int page = gtk_notebook_get_current_page(notebook);

    if(page <= 0){
        return;
    }

    gtk_notebook_remove_page(
      notebook,
      page
    );
}

void menu_movetableft(void){
    int page = gtk_notebook_get_current_page(notebook);
    int position = page - 1;

    if(position < 0){
        return;

    }else if(position == 0){
        position = gtk_notebook_get_n_pages(notebook) - 1;
    }

    gtk_notebook_reorder_child(
      notebook,
      gtk_notebook_get_nth_page(
        notebook,
        page
      ),
      position
    );
}

void menu_movetabright(void){
    int page = gtk_notebook_get_current_page(notebook);
    int position = page + 1;

    if(position <= 1){
        return;

    }else if(position >= gtk_notebook_get_n_pages(notebook)){
        position = 1;
    }

    gtk_notebook_reorder_child(
      notebook,
      gtk_notebook_get_nth_page(
        notebook,
        page
      ),
      position
    );
}

void menu_newtab(void){
    WebKitWebView *view;

    // Setup tab view.
    view = WEBKIT_WEB_VIEW(webkit_web_view_new());

    // Append and show.
    gtk_notebook_append_page(
      notebook,
      GTK_WIDGET(view),
      gtk_label_new("NEW TAB")
    );
    gtk_widget_show_all(window);
    gtk_notebook_set_current_page(
      notebook,
      gtk_notebook_get_n_pages(notebook) - 1
    );
    gtk_widget_grab_focus(GTK_WIDGET(view));

    // Setup signals.
    g_signal_connect(
      view,
      "load-changed",
      G_CALLBACK(view_load_changed),
      NULL
    );

    webkit_web_view_load_uri(
      view,
      "https://iterami.com"
    );
}

void tab_switch(GtkNotebook *notebook, GtkWidget *page_content, guint page, gpointer data){
    tab_update_labels();
}

void tab_update_labels(void){
    WebKitWebView *view;

    view = get_tab_view();

    gtk_entry_set_text(
      GTK_ENTRY(entry_toolbar_address),
      webkit_web_view_get_uri(view)
    );
    gtk_window_set_title(
      GTK_WINDOW(window),
      webkit_web_view_get_title(view)
    );

    int page = gtk_notebook_get_current_page(notebook);
    if(page > 0){
        gtk_notebook_set_tab_label_text(
          notebook,
          gtk_notebook_get_nth_page(
            notebook,
            page
          ),
          webkit_web_view_get_title(view)
        );
    }
}

void toolbar_address_activate(void){
    WebKitWebView *view;

    view = get_tab_view();

    webkit_web_view_load_uri(
      view,
      gtk_entry_get_text(GTK_ENTRY(entry_toolbar_address))
    );
}

void toolbar_back(void){
    WebKitWebView *view;

    view = get_tab_view();

    if(webkit_web_view_can_go_back(view)){
        webkit_web_view_go_back(view);
    }
}

void toolbar_forward(void){
    WebKitWebView *view;

    view = get_tab_view();

    if(webkit_web_view_can_go_forward(view)){
        webkit_web_view_go_forward(view);
    }
}

void toolbar_reload(void){
    webkit_web_view_reload(get_tab_view());
}

void toolbar_stop(void){
    webkit_web_view_stop_loading(get_tab_view());
}

void view_load_changed(WebKitWebView *view, WebKitLoadEvent load_event, gpointer data){
    switch(load_event){
      case WEBKIT_LOAD_STARTED:
          break;

      case WEBKIT_LOAD_REDIRECTED:
          break;

      case WEBKIT_LOAD_COMMITTED:
          break;

      case WEBKIT_LOAD_FINISHED:
          tab_update_labels();

          break;
    }
}
