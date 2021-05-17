#pragma once

#define HOME_TAB_TITLE "⌂"
#define KEY_CLOSETAB GDK_KEY_w
#define KEY_DELETE GDK_KEY_Delete
#define KEY_DEVTOOLS GDK_KEY_i
#define KEY_INSERT GDK_KEY_Insert
#define KEY_MOVETABLEFT GDK_KEY_underscore
#define KEY_MOVETABRIGHT GDK_KEY_plus
#define KEY_NEWTAB GDK_KEY_t
#define KEY_NEXTTAB GDK_KEY_equal
#define KEY_OPEN GDK_KEY_o
#define KEY_PREVIOUSTAB GDK_KEY_minus
#define KEY_RELOAD GDK_KEY_r
#define KEY_SELECTALL GDK_KEY_a

GtkNotebook *notebook;
GtkWidget *button_toolbar_back;
GtkWidget *button_toolbar_forward;
GtkWidget *button_toolbar_reload;
GtkWidget *button_toolbar_stop;
GtkWidget *entry_toolbar_address;

WebKitWebView* get_tab_view(void);
int main(int argc, char **argv);
void menu_closetab(void);
void menu_devtools(void);
void menu_movetab(const gint movement);
void menu_newtab(const gchar *title);
void menu_openfile(void);
void menu_reload(const int bypass);
void startup(GtkApplication* app, gpointer data);
void tab_switch(GtkNotebook *notebook, GtkWidget *page_content, guint page, gpointer data);
void tab_update_labels(void);
void toolbar_address_activate(void);
void toolbar_back(void);
void toolbar_forward(void);
void toolbar_stop(void);
void view_load_changed(WebKitWebView *view, WebKitLoadEvent load_event, gpointer data);
