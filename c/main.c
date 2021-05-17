#include <gtk/gtk.h>
#include <string.h>
#include <webkit2/webkit2.h>
#include "main.h"
#include "../../common/c/core.c"
#include "../../common/c/gtk.c"

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
      G_CALLBACK(gtk_activate),
      NULL
    );
    g_signal_connect(
      app,
      "startup",
      G_CALLBACK(startup),
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

void menu_devtools(void){
    WebKitWebInspector *devtools;
    WebKitWebView *view;

    view = get_tab_view();
    devtools = webkit_web_view_get_inspector(view);

    if(devtools == NULL
      || webkit_web_inspector_get_web_view(devtools) == NULL){
        webkit_web_inspector_show(devtools);

    }else{
        webkit_web_inspector_close(devtools);
    }
}

void menu_movetab(const gint movement){
    int page = gtk_notebook_get_current_page(notebook);

    if(page == 0){
        return;
    }

    int position = page + movement;

    if(position <= 1){
        position = 1;

    }else{
        int pages = gtk_notebook_get_n_pages(notebook);

        if(position >= pages){
            position = pages - 1;
        }
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

void menu_newtab(const gchar *title){
    WebKitSettings *settings;
    WebKitWebView *view;

    // Setup view settings.
    settings = webkit_settings_new();
    webkit_settings_set_enable_developer_extras(
      settings,
      TRUE
    );
    webkit_settings_set_enable_webaudio(
      settings,
      TRUE
    );
    webkit_settings_set_enable_webgl(
      settings,
      TRUE
    );

    // Setup tab view.
    GdkRGBA background_color = {0, 0, 0, 1,};
    view = WEBKIT_WEB_VIEW(webkit_web_view_new_with_settings(settings));
    webkit_web_view_set_background_color(
      view,
      &background_color
    );

    // Append and show.
    gtk_notebook_append_page(
      notebook,
      GTK_WIDGET(view),
      gtk_label_new(title)
    );
    gtk_widget_show_all(window);
    gtk_notebook_set_current_page(
      notebook,
      gtk_notebook_get_n_pages(notebook) - 1
    );
    gtk_widget_grab_focus(entry_toolbar_address);

    // Setup signals.
    g_signal_connect(
      view,
      "load-changed",
      G_CALLBACK(view_load_changed),
      NULL
    );

    tab_update_labels();
}

void menu_openfile(void){
    GtkFileChooser *chooser;
    GtkWidget *dialog_open;

    dialog_open = gtk_file_chooser_dialog_new(
      "Open File...",
      GTK_WINDOW(window),
      GTK_FILE_CHOOSER_ACTION_OPEN,
      "_Cancel",
      GTK_RESPONSE_CANCEL,
      "_Open",
      GTK_RESPONSE_ACCEPT,
      NULL
    );
    chooser = GTK_FILE_CHOOSER(dialog_open);
    gtk_file_chooser_set_show_hidden(
      chooser,
      TRUE
    );

    if(gtk_dialog_run(GTK_DIALOG(dialog_open)) == GTK_RESPONSE_ACCEPT){
        char *filename;
        char path[4096];
        WebKitWebView *view;

        filename = gtk_file_chooser_get_filename(chooser);
        menu_newtab("");
        view = get_tab_view();

        strcpy(
          path,
          "file://"
        );
        strcat(
          path,
          filename
        );

        gtk_entry_set_text(
          GTK_ENTRY(entry_toolbar_address),
          path
        );
        webkit_web_view_load_uri(
          view,
          path
        );

        g_free(filename);
    }

    gtk_widget_destroy(dialog_open);
}

void menu_reload(const int bypass){
    if(bypass == 1){
        webkit_web_view_reload_bypass_cache(get_tab_view());

    }else{
        webkit_web_view_reload(get_tab_view());
    }
}

void startup(GtkApplication* app, gpointer data){
    GtkAccelGroup *accelgroup;
    GtkWidget *box;
    GtkWidget *menu_menu;
    GtkWidget *menubar;
    GtkWidget *menuitem_menu;
    GtkWidget *toolbar;
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
    gtk_notebook_set_show_border(
      notebook,
      FALSE
    );
    g_signal_connect_after(
      notebook,
      "switch-page",
      G_CALLBACK(tab_switch),
      NULL
    );

    // Setup menu items.
    menubar = gtk_menu_bar_new();
    accelgroup = gtk_accel_group_new();
    gtk_window_add_accel_group(
      GTK_WINDOW(window),
      accelgroup
    );
    // Menu menu.
    menu_menu = gtk_menu_new();
    menuitem_menu = gtk_menu_item_new_with_mnemonic("☰");
    gtk_menu_item_set_submenu(
      GTK_MENU_ITEM(menuitem_menu),
      menu_menu
    );
    gtk_add_menuitem(
      menu_menu,
      "_New Tab",
      accelgroup,
      KEY_NEWTAB,
      GDK_CONTROL_MASK,
      G_CALLBACK(menu_newtab),
      "NEW TAB"
    );
    gtk_add_menuitem(
      menu_menu,
      "_Open File...",
      accelgroup,
      KEY_OPEN,
      GDK_CONTROL_MASK,
      G_CALLBACK(menu_openfile),
      NULL
    );
    gtk_menu_shell_append(
      GTK_MENU_SHELL(menu_menu),
      gtk_separator_menu_item_new()
    );
    gtk_add_menuitem(
      menu_menu,
      "Toggle De_v Tools",
      accelgroup,
      KEY_DEVTOOLS,
      GDK_CONTROL_MASK,
      G_CALLBACK(menu_devtools),
      NULL
    );
    gtk_menu_shell_append(
      GTK_MENU_SHELL(menu_menu),
      gtk_separator_menu_item_new()
    );
    gtk_add_menuitem(
      menu_menu,
      "_Copy",
      accelgroup,
      KEY_COPY,
      GDK_CONTROL_MASK,
      NULL,
      NULL
    );
    gtk_add_menuitem(
      menu_menu,
      "Cu_t",
      accelgroup,
      KEY_CUT,
      GDK_CONTROL_MASK,
      NULL,
      NULL
    );
    gtk_add_menuitem(
      menu_menu,
      "_Paste",
      accelgroup,
      KEY_PASTE,
      GDK_CONTROL_MASK,
      NULL,
      NULL
    );
    gtk_add_menuitem(
      menu_menu,
      "_Delete",
      accelgroup,
      KEY_DELETE,
      0,
      NULL,
      NULL
    );
    gtk_add_menuitem(
      menu_menu,
      "Toggle _Overwrite",
      accelgroup,
      KEY_INSERT,
      0,
      NULL,
      NULL
    );
    gtk_add_menuitem(
      menu_menu,
      "_Select All",
      accelgroup,
      KEY_SELECTALL,
      GDK_CONTROL_MASK,
      NULL,
      NULL
    );
    gtk_menu_shell_append(
      GTK_MENU_SHELL(menu_menu),
      gtk_separator_menu_item_new()
    );
    gtk_add_menuitem(
      menu_menu,
      "_Reload",
      accelgroup,
      KEY_RELOAD,
      GDK_CONTROL_MASK,
      G_CALLBACK(menu_reload),
      (gpointer)0
    );
    gtk_add_menuitem(
      menu_menu,
      "Reload, _Bypass Cache",
      accelgroup,
      KEY_RELOAD,
      GDK_CONTROL_MASK | GDK_SHIFT_MASK,
      G_CALLBACK(menu_reload),
      (gpointer)1
    );
    gtk_menu_shell_append(
      GTK_MENU_SHELL(menu_menu),
      gtk_separator_menu_item_new()
    );
    gtk_add_menuitem(
      menu_menu,
      "_Next Tab",
      accelgroup,
      KEY_NEXTTAB,
      GDK_CONTROL_MASK,
      G_CALLBACK(gtk_notebook_next_page),
      GTK_WIDGET(notebook)
    );
    gtk_add_menuitem(
      menu_menu,
      "_Previous Tab",
      accelgroup,
      KEY_PREVIOUSTAB,
      GDK_CONTROL_MASK,
      G_CALLBACK(gtk_notebook_prev_page),
      GTK_WIDGET(notebook)
    );
    gtk_add_menuitem(
      menu_menu,
      "Move Tab _Left",
      accelgroup,
      KEY_MOVETABLEFT,
      GDK_CONTROL_MASK | GDK_SHIFT_MASK,
      G_CALLBACK(menu_movetab),
      (gpointer)-1
    );
    gtk_add_menuitem(
      menu_menu,
      "Move Tab _Right",
      accelgroup,
      KEY_MOVETABRIGHT,
      GDK_CONTROL_MASK | GDK_SHIFT_MASK,
      G_CALLBACK(menu_movetab),
      (gpointer)1
    );
    gtk_menu_shell_append(
      GTK_MENU_SHELL(menu_menu),
      gtk_separator_menu_item_new()
    );
    gtk_add_menuitem(
      menu_menu,
      "_Close Tab",
      accelgroup,
      KEY_CLOSETAB,
      GDK_CONTROL_MASK,
      G_CALLBACK(menu_closetab),
      NULL
    );
    gtk_add_menuitem(
      menu_menu,
      "_Quit",
      accelgroup,
      KEY_QUIT,
      GDK_CONTROL_MASK,
      G_CALLBACK(gtk_widget_destroy),
      window
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
      G_CALLBACK(menu_reload),
      (gpointer)0
    );
    g_signal_connect(
      button_toolbar_stop,
      "clicked",
      G_CALLBACK(toolbar_stop),
      NULL
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
    menu_newtab(HOME_TAB_TITLE);

    gtk_widget_show_all(window);
}

void tab_switch(GtkNotebook *notebook, GtkWidget *page_content, guint page, gpointer data){
    tab_update_labels();
}

void tab_update_labels(void){
    int page = gtk_notebook_get_current_page(notebook);

    if(page == 0){
        gtk_widget_set_sensitive(
          entry_toolbar_address,
          FALSE
        );
        gtk_entry_set_text(
          GTK_ENTRY(entry_toolbar_address),
          "Home Tab"
        );
        gtk_window_set_title(
          GTK_WINDOW(window),
          HOME_TAB_TITLE
        );
        return;

    }else{
        gtk_widget_set_sensitive(
          entry_toolbar_address,
          TRUE
        );
    }

    GtkWidget *page_widget;
    const gchar *title;
    const gchar *uri;
    WebKitWebView *view;

    page_widget = gtk_notebook_get_nth_page(
      notebook,
      page
    );
    view = get_tab_view();
    uri = webkit_web_view_get_uri(view);

    if(uri != NULL){
        title = webkit_web_view_get_title(view);

        gtk_notebook_set_tab_label_text(
          notebook,
          page_widget,
          title
        );
        gtk_notebook_set_menu_label_text(
          notebook,
          page_widget,
          title
        );
        gtk_window_set_title(
          GTK_WINDOW(window),
          title
        );

    }else{
        uri = "";
        gtk_notebook_set_tab_label_text(
          notebook,
          page_widget,
          gtk_notebook_get_tab_label_text(
            notebook,
            page_widget
          )
        );
        gtk_notebook_set_menu_label_text(
          notebook,
          page_widget,
          gtk_notebook_get_tab_label_text(
            notebook,
            page_widget
          )
        );
        gtk_window_set_title(
          GTK_WINDOW(window),
          gtk_notebook_get_tab_label_text(
            notebook,
            page_widget
          )
        );
    }

    gtk_entry_set_text(
      GTK_ENTRY(entry_toolbar_address),
      uri
    );
}

void toolbar_address_activate(void){
    WebKitWebView *view;

    view = get_tab_view();

    webkit_web_view_load_uri(
      view,
      gtk_entry_get_text(GTK_ENTRY(entry_toolbar_address))
    );
    gtk_widget_grab_focus(GTK_WIDGET(view));
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
