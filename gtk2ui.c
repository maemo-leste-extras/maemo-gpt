#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
//#include <curl/curl.h>
#include <string.h>
//#include <jansson.h>
#include <glib.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#include "gtk2ui.h"
#include "on_click.h"


void create_main_window() {

typedef struct {
       GtkEntry *entry;
       GtkTextView *text_view;
 } CallbackData;


 GtkWidget *window;
 GtkWidget *scrolled_window;
 GtkTextBuffer *buffer;
 GtkWidget *vbox;
 GtkWidget *hbox;
 GtkWidget *button1;
 GtkWidget *menubar;
 GtkWidget *file_menu;
 GtkWidget *images_menu;
 GtkWidget *audio_menu;
 GtkWidget *option_menu;
 GtkWidget *assistant_menu;
 GtkWidget *about_menu;
 GtkWidget *file_item;
 GtkWidget *file_item2;
 GtkWidget *file_item3;
 GtkWidget *file_item4;
 GtkWidget *file_item5;
 GtkWidget *file_item6;
// GtkWidget *file_nickname_item;
// GtkWidget *file_assistant_item;
// GtkWidget *file_thread_item;
 GtkWidget *file_newmsg_item;
 GtkWidget *file_run_item;
 GtkWidget *file_listmsgs_item;
 GtkWidget *file_list_item;
 GtkWidget *file_upload_item;
 GtkWidget *file_vision_item;
 GtkWidget *file_audio_item;
 GtkWidget *file_audio_stt_item;
 GtkWidget *file_genimage_item;
 GtkWidget *file_save_item;
 GtkWidget *file_load_item;
 GtkWidget *file_reset_item;
// GtkWidget *file_apikey_item;
 GtkWidget *file_models_item;
 GtkWidget *file_temperature_item;
 GtkWidget *file_abouttxt_item;
 GtkWidget *file_exit_item;
//GtkWidget *submenu, *submenu_item1, *submenu_item2;
GtkWidget *submenu2, *submenu_item2, *submenu_item3, *submenu_item4, *submenu_item5;
GtkWidget *submenu3, *submenu_item6, *submenu_item7, *submenu_item8, *submenu_item9;
//GtkWidget *submenu4, *submenu_item10, *submenu_item11;


window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
 gtk_window_set_title(GTK_WINDOW(window), "Maemo-GPT");
 gtk_container_set_border_width(GTK_CONTAINER(window), 10);
 gtk_widget_set_size_request(window, 800, 480);
 g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);


  // Create the menu bar
    menubar = gtk_menu_bar_new();

    // Create file menu
    file_menu = gtk_menu_new();
    option_menu = gtk_menu_new();
    images_menu = gtk_menu_new();
    audio_menu = gtk_menu_new();
    assistant_menu = gtk_menu_new();
    about_menu = gtk_menu_new();

    // Create menu items

//    file_nickname_item = gtk_menu_item_new_with_label("Nickname");
//    file_assistant_item = gtk_menu_item_new_with_label("Assistant id");
//    file_thread_item = gtk_menu_item_new_with_label("Thread id");
    file_newmsg_item = gtk_menu_item_new_with_label("New Request");
    file_run_item = gtk_menu_item_new_with_label("Run");
    file_listmsgs_item = gtk_menu_item_new_with_label("Show Messages");
    file_list_item = gtk_menu_item_new_with_label("List Files");
    file_upload_item = gtk_menu_item_new_with_label("Upload Files");
    file_vision_item = gtk_menu_item_new_with_label("Vision");
    file_audio_item = gtk_menu_item_new_with_label("Text to Speech");
    file_audio_stt_item = gtk_menu_item_new_with_label("Speech to Text");
    file_genimage_item = gtk_menu_item_new_with_label("Generate Image");
    file_save_item = gtk_menu_item_new_with_label("Save Chat");
    file_load_item = gtk_menu_item_new_with_label("Load Chat");
    file_reset_item = gtk_menu_item_new_with_label("Reset Chat");
    file_models_item = gtk_menu_item_new_with_label("Models");
//    file_apikey_item = gtk_menu_item_new_with_label("API Key");
    file_temperature_item = gtk_menu_item_new_with_label("Temperature");
    file_abouttxt_item = gtk_menu_item_new_with_label("Maemo-GPT by arno11");
    file_exit_item = gtk_menu_item_new_with_label("Exit");

 // Create a submenu for the "File" menu item
//    submenu = gtk_menu_new();
//    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_load_item), submenu);

    submenu2 = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_models_item), submenu2);

    submenu3 = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_audio_item), submenu3);

 //   submenu4 = gtk_menu_new();
 //   gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_vision_item), submenu4);

    // Create submenu items
//    submenu_item1 = gtk_menu_item_new_with_label("Save 1");
//    g_signal_connect(submenu_item1, "activate", G_CALLBACK(on_menu_item_activate), "Open");
//    gtk_menu_shell_append(GTK_MENU_SHELL(submenu), submenu_item1);

    submenu_item2 = gtk_menu_item_new_with_label("gpt-4o-mini (web search)");
    g_signal_connect(submenu_item2, "activate", G_CALLBACK(on_gpt_mini_web), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu2), submenu_item2);

    submenu_item3 = gtk_menu_item_new_with_label("chatgpt-3.5-turbo");
    g_signal_connect(submenu_item3, "activate", G_CALLBACK(on_gpt_35), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu2), submenu_item3);

    submenu_item4 = gtk_menu_item_new_with_label("chatgpt-4o");
    g_signal_connect(submenu_item4, "activate", G_CALLBACK(on_gpt_4), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu2), submenu_item4);

    submenu_item5 = gtk_menu_item_new_with_label("chatgpt-4o-mini");
    g_signal_connect(submenu_item5, "activate", G_CALLBACK(on_gpt_mini), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu2), submenu_item5);

    submenu_item6 = gtk_menu_item_new_with_label("Play Last TTS");
    g_signal_connect(submenu_item6, "activate", G_CALLBACK(on_play_curTTS), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu3), submenu_item6);

    submenu_item7 = gtk_menu_item_new_with_label("Play Last TTS from Chat");
    g_signal_connect(submenu_item7, "activate", G_CALLBACK(on_play_curChat), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu3), submenu_item7);

    submenu_item8 = gtk_menu_item_new_with_label("New TTS");
    g_signal_connect(submenu_item8, "activate", G_CALLBACK(on_play_newTTS), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu3), submenu_item8);

    submenu_item9 = gtk_menu_item_new_with_label("New TTS from Chat");
    g_signal_connect(submenu_item9, "activate", G_CALLBACK(on_play_newChat), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu3), submenu_item9);

   // Connect signals to menu items
//   g_signal_connect(file_assistant_item, "activate", G_CALLBACK(on_file_assistant), NULL);

//   g_signal_connect(file_thread_item, "activate", G_CALLBACK(on_file_thread), NULL);

   g_signal_connect(file_run_item, "activate", G_CALLBACK(on_file_run), NULL);

   g_signal_connect(file_upload_item, "activate", G_CALLBACK(on_file_upload), NULL);

   g_signal_connect(file_save_item, "activate", G_CALLBACK(on_file_save), NULL);

   g_signal_connect(file_load_item, "activate", G_CALLBACK(on_file_load), NULL);

   g_signal_connect(file_reset_item, "activate", G_CALLBACK(on_file_reset), NULL); 

//   g_signal_connect(file_apikey_item, "activate", G_CALLBACK(on_file_apikey), NULL); 

   g_signal_connect(file_abouttxt_item, "activate", G_CALLBACK(on_file_about), NULL);

   g_signal_connect(file_exit_item, "activate", G_CALLBACK(on_file_exit), NULL);


   // Append items to menus
//    gtk_menu_append(GTK_MENU(assistant_menu), file_nickname_item);
//    gtk_menu_append(GTK_MENU(assistant_menu), file_assistant_item);
//    gtk_menu_append(GTK_MENU(assistant_menu), file_thread_item);
    gtk_menu_append(GTK_MENU(assistant_menu), file_newmsg_item);
    gtk_menu_append(GTK_MENU(assistant_menu), file_run_item);
    gtk_menu_append(GTK_MENU(assistant_menu), file_listmsgs_item);

    gtk_menu_append(GTK_MENU(file_menu), file_list_item);
    gtk_menu_append(GTK_MENU(file_menu), file_upload_item);
    gtk_menu_append(GTK_MENU(images_menu), file_vision_item);
    gtk_menu_append(GTK_MENU(audio_menu), file_audio_item);
    gtk_menu_append(GTK_MENU(audio_menu), file_audio_stt_item);
    gtk_menu_append(GTK_MENU(images_menu), file_genimage_item);
    gtk_menu_append(GTK_MENU(file_menu), file_save_item);
    gtk_menu_append(GTK_MENU(file_menu), file_load_item);
    gtk_menu_append(GTK_MENU(file_menu), file_reset_item);
    gtk_menu_append(GTK_MENU(file_menu), file_exit_item);
    gtk_menu_append(GTK_MENU(option_menu), file_models_item);
//    gtk_menu_append(GTK_MENU(option_menu), file_apikey_item);
    gtk_menu_append(GTK_MENU(option_menu), file_temperature_item);
    gtk_menu_append(GTK_MENU(about_menu), file_abouttxt_item);


  // Create the file menu item on the menu bar
    file_item = gtk_menu_item_new_with_label("Files");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_item), file_menu);
    gtk_menu_bar_append(GTK_MENU_BAR(menubar), file_item);

    file_item2 = gtk_menu_item_new_with_label("Options");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_item2), option_menu);
    gtk_menu_bar_append(GTK_MENU_BAR(menubar), file_item2);

    file_item3 = gtk_menu_item_new_with_label("Images");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_item3), images_menu);
    gtk_menu_bar_append(GTK_MENU_BAR(menubar), file_item3);

    file_item5 = gtk_menu_item_new_with_label("Audio");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_item5), audio_menu);
    gtk_menu_bar_append(GTK_MENU_BAR(menubar), file_item5);

    file_item6 = gtk_menu_item_new_with_label(" Leste-Assistant ");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_item6), assistant_menu);
    gtk_menu_bar_append(GTK_MENU_BAR(menubar), file_item6);


    file_item4 = gtk_menu_item_new_with_label("About");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_item4), about_menu);
    gtk_menu_bar_append(GTK_MENU_BAR(menubar), file_item4);



 // Create a vertical box container
 vbox = gtk_vbox_new(FALSE, 10);
 gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
 gtk_container_add(GTK_CONTAINER(window), vbox);

// Create a horizontal box container
 hbox = gtk_hbox_new(TRUE, 10);
 gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

// Create a text view widget

GtkWidget *text_view = gtk_text_view_new();

 gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);


// Get the buffer associated with the TextView
 buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

 // Set default text in the buffer
 gtk_text_buffer_set_text(buffer, "###########   Welcome to Maemo-GPT !   ###########\n", -1);


// Create a GtkScrolledWindow
 scrolled_window = gtk_scrolled_window_new(NULL, NULL);
 gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
 GTK_POLICY_AUTOMATIC,
 GTK_POLICY_AUTOMATIC);

 gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
 gtk_widget_set_can_focus(GTK_WIDGET(text_view), TRUE);
 gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);

 // Add the text view to the scrolled window
 gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);

 // Add the scrolled window to the main window
 gtk_container_add(GTK_CONTAINER(vbox), scrolled_window);

 g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);


// Create a horizontal box container
 hbox = gtk_hbox_new(FALSE, 10);
 gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);


//  Create an entry widget
GtkWidget *entry = gtk_entry_new();

// entry = gtk_entry_new();
 gtk_widget_set_size_request(entry, 550, 70);
 gtk_box_pack_start(GTK_BOX(hbox), entry, TRUE, TRUE, 0);
// gtk_entry_set_text(GTK_ENTRY(entry), "Request:");

// Create a button next to entry
 button1 = gtk_button_new_with_label("Send");
 gtk_widget_set_size_request(button1, 150, 70);
 gtk_box_pack_start(GTK_BOX(hbox), button1, FALSE, FALSE, 0);


  // Create an instance of the structure and set the widgets
   CallbackData *data = g_malloc(sizeof(CallbackData));
   data->entry = GTK_ENTRY(entry);
   data->text_view = GTK_TEXT_VIEW(text_view);

g_signal_connect(file_newmsg_item, "activate", G_CALLBACK(on_file_newmsg), data);
g_signal_connect(file_listmsgs_item, "activate", G_CALLBACK(on_file_listmsgs), data);
g_signal_connect(file_vision_item, "activate", G_CALLBACK(on_file_vision), data);
g_signal_connect(file_genimage_item, "activate", G_CALLBACK(on_file_genimage), data);
g_signal_connect(file_list_item, "activate", G_CALLBACK(on_file_list), data);
g_signal_connect(button1, "clicked", G_CALLBACK(on_button_clicked1), data);


// Show all widgets
 gtk_widget_show_all(window);

 // place the cursor in text entry on startup
 gtk_widget_grab_focus(entry);


}

