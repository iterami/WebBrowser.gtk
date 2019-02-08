#pragma once

#define KEY_CLOSETAB GDK_KEY_w
#define KEY_MOVETABLEFT GDK_KEY_underscore
#define KEY_MOVETABRIGHT GDK_KEY_plus
#define KEY_NEWTAB GDK_KEY_t
#define KEY_NEXTTAB GDK_KEY_equal
#define KEY_PREVIOUSTAB GDK_KEY_minus

GtkNotebook *notebook;
GtkWidget *button_toolbar_back;
GtkWidget *button_toolbar_forward;
GtkWidget *button_toolbar_reload;
GtkWidget *button_toolbar_stop;
GtkWidget *entry_toolbar_address;

void activate(GtkApplication* app, gpointer data);
int main(int argc, char **argv);
void menu_closetab(void);
void menu_movetableft(void);
void menu_movetabright(void);
void menu_newtab(void);
void toolbar_back(void);
void toolbar_forward(void);
void toolbar_reload(void);
void toolbar_stop(void);
