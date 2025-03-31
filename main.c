#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <jansson.h>
#include <glib.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#include "gtk2ui.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    create_main_window();
    gtk_main();
    return 0;
}
