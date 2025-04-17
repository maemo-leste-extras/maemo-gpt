#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <jansson.h>
#include <glib.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#include "on_click.h"
#include "parsing.h"
#include "file_handling.h"
#include "gtk2ui.h"
#include <sys/stat.h>


const gchar *gpt_model = "gpt-4o-mini";


typedef struct {
       GtkEntry *entry;
       GtkTextView *text_view;
 } CallbackData;


void on_play_curTTS() {
system("mpg123 /home/user/.maemo-gpt/output.pcm");
g_print("Playing TTS file...");
return;
}

void on_play_curChat() {
system("mpg123 /home/user/.maemo-gpt/chatoutput.pcm");
g_print("Playing last registred chat"); 
return;
}

void on_play_newTTS(GtkWidget *widget, gpointer data) {
g_print("New TTS loading...");

    GtkWidget *dialog;

 dialog = gtk_dialog_new_with_buttons("ChatGPT TTS: New Text:",
                                         NULL,
                                         GTK_DIALOG_MODAL,
                                         GTK_STOCK_OK,
                                         GTK_RESPONSE_OK,
                                         GTK_STOCK_CANCEL,
                                         GTK_RESPONSE_CANCEL,
                                         NULL);

   // Create an entry widget
    GtkWidget *entry = gtk_entry_new();
    gtk_widget_set_size_request(entry, 550, 70);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), entry, TRUE, TRUE, 0);

   // Show the dialog and entry
    gtk_widget_show(entry);
    gtk_widget_show(dialog);

    // Run the dialog and wait for user response
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));

    // Handle the user's response
    if(result == GTK_RESPONSE_OK) {
        const gchar *input_TTS = gtk_entry_get_text(GTK_ENTRY(entry));
        g_print("User entered: %s\n", input_TTS);

 CURL *curl;
 CURLcode res;

  const gchar *output_filename = "output.pcm";

curl_global_init(CURL_GLOBAL_DEFAULT);
 curl = curl_easy_init();

     // Open the file for writing
      FILE *fp; 
      fp = fopen(output_filename, "wb");
        if(!fp) {
            perror("File opening failed");
            return;
        }

 // Set the API endpoint URL
 curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/audio/speech");

gchar *json_data_tts = g_malloc(524288 * sizeof(gchar));
      if (json_data_tts == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
       }

snprintf(json_data_tts, 524288, "{\"model\":\"tts-1\",\"input\":\"%s\",\"voice\":\"alloy\"}", input_TTS);

// Set up cURL options
 struct curl_slist *headers = NULL;
 headers = curl_slist_append(headers, "Content-Type: application/json");


const gchar *api_filename = "/home/user/.maemo-gpt/gpt-config.cfg";
    const gchar *key_to_find = "apikey";
    gchar API_key[256];

    read_config_value(api_filename, key_to_find, API_key, sizeof(API_key));


gchar *key_data = g_malloc(1000 * sizeof(gchar));
      if (key_data == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
       }

snprintf(key_data, 1000, "Authorization: Bearer %s", API_key);
headers = curl_slist_append(headers, key_data);


 curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
 curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data_tts);


// Perform the POST reques
curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback_data_stream);
 curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

curl_easy_escape(curl, json_data_tts, 0);

 curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data_tts);


res = curl_easy_perform(curl);
 if (res != CURLE_OK) {
 GtkTextBuffer *text_buffer = gtk_text_buffer_new(NULL);
 gtk_text_buffer_insert_at_cursor(text_buffer, curl_easy_strerror(res), -1);
fclose(fp);
 }

system("mpg123 /home/user/.maemo-gpt/output.pcm");
fclose(fp);
g_free(json_data_tts);
g_free(key_data);

curl_slist_free_all(headers);
curl_easy_cleanup(curl);


 curl_global_cleanup();
}
   gtk_widget_destroy(dialog);
return;
}


void on_play_newChat() {
g_print("Converting last chat message into speech...");

gchar *audiochat = read_file("/home/user/.maemo-gpt/chataudio.log");
removeCharacters(audiochat);
removeNewLines(audiochat);


CURL *curl;
 CURLcode res;

  const gchar *output_filename = "chatoutput.pcm";


curl_global_init(CURL_GLOBAL_DEFAULT);
 curl = curl_easy_init();

     // Open the file for writing
      FILE *fp; 
      fp = fopen(output_filename, "wb");
        if(!fp) {
            perror("File opening failed");

         return;
        }

// Set the API endpoint URL
 curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/audio/speech");


gchar *json_data_tts = g_malloc(524288 * sizeof(gchar));
      if (json_data_tts == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
       }

snprintf(json_data_tts, 524288, "{\"model\":\"tts-1\",\"input\":\"%s\",\"voice\":\"alloy\"}", audiochat);

// Set up cURL options
 struct curl_slist *headers = NULL;
 headers = curl_slist_append(headers, "Content-Type: application/json");


const gchar *api_filename = "/home/user/.maemo-gpt/gpt-config.cfg";
    const gchar *key_to_find = "apikey";
    gchar API_key[256];

    read_config_value(api_filename, key_to_find, API_key, sizeof(API_key));


gchar *key_data = g_malloc(1000 * sizeof(gchar));
      if (key_data == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
       }

snprintf(key_data, 1000, "Authorization: Bearer %s", API_key);
headers = curl_slist_append(headers, key_data);


 curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
 curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data_tts);


// Perform the POST request
 curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback_data_stream);
 curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

 curl_easy_escape(curl, json_data_tts, 0);

 curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data_tts);

res = curl_easy_perform(curl);
 if (res != CURLE_OK) {
 GtkTextBuffer *text_buffer = gtk_text_buffer_new(NULL);
 gtk_text_buffer_insert_at_cursor(text_buffer, curl_easy_strerror(res), -1);
fclose(fp);
 }

system("mpg123 /home/user/.maemo-gpt/chatoutput.pcm");
fclose(fp);
g_free(json_data_tts);
g_free(key_data);

curl_slist_free_all(headers);
 curl_easy_cleanup(curl);


 curl_global_cleanup();
return;
}


void on_gpt_mini() {
g_print("gpt-4o-mini selected\n");
g_print("Current model: %s\n", gpt_model);

const gchar *gpt_model = "gpt-4o-mini";
g_print("Current model: %s\n", gpt_model);
return;
}

void on_gpt_4() {
g_print("gpt-4o selected\n");
g_print("Current model: %s\n", gpt_model);

const gchar *gpt_model = "gpt-4o";
g_print("Current model: %s\n", gpt_model);
return;
}

void on_gpt_35() {
g_print("gpt-3.5-turbo selected\n");
g_print("Current model: %s\n", gpt_model);

const gchar *gpt_model = "gpt-3.5-turbo";
g_print("Current model: %s\n", gpt_model);
return;
}


void on_file_models() {
g_print("Choose Model...");
return;
}


//about menu
void on_file_about() {
g_print("Maemo-GPT by arno11");
return;
}


//reset chat
void on_file_reset() {
FILE *file_history = fopen("/home/user/.maemo-gpt/chat-history.log", "wb"); // Create/overwrite an empty file
 if (file_history == NULL) {
 perror("Error creating file");
 return;
 }
 fclose(file_history);

FILE *file_temp = fopen("/home/user/.maemo-gpt/response_back.log", "wb"); //>
 if (file_temp == NULL) {
 perror("Error creating file");
 return;
 }
 fclose(file_temp);

FILE *file_quotes = fopen("/home/user/.maemo-gpt/response_display.log", "wb");
 if (file_quotes == NULL) {
 perror("Error creating file");
 return;
 }
 fclose(file_quotes);

g_print("Reset Chat...\n");
return;
}

//load chat
void on_file_load(GtkWidget *widget, gpointer window) {
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new("Open File",
                                         GTK_WINDOW(window),
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         ("_Cancel"),
                                         GTK_RESPONSE_CANCEL,
                                         ("_Open"),
                                         GTK_RESPONSE_OK,
                                         NULL);

 // Set the default directory
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), "/home/user/.maemo-gpt/history");

  // Run the dialog and get the response
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        gchar *file_histo;

        file_histo = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        g_print("Selected file: %s\n", file_histo);

        if (is_txt_file(file_histo)) {
gchar *chat_save_histo = read_file(file_histo);

  FILE *fileload = fopen("/home/user/.maemo-gpt/chat-history.log", "wb");
 if (fileload == NULL) {
        perror("Error opening file");
        return;
    }

  // Write history to the file
    fprintf(fileload, "%s", chat_save_histo);

    // Close the file
    fclose(fileload);


   printf("Conversation transfered !\n");


gchar *display_file_histo = g_malloc(100 * sizeof(gchar));
      if (display_file_histo == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
       }

snprintf(display_file_histo, 100, "%s_display.log", file_histo);
g_print(display_file_histo);
gchar *chat_save_display_histo = read_file(display_file_histo);
g_free(display_file_histo);


 FILE *fileloaddisplay = fopen("/home/user/.maemo-gpt/response_display.log", "wb");
if (fileloaddisplay == NULL) {
        perror("Error opening file");
        return;
    }

  // Write history to the file
    fprintf(fileloaddisplay, "%s", chat_save_display_histo);

    // Close the file
    fclose(fileloaddisplay);
    g_free(file_histo);

   printf("Conversation display transfered !\n");
} else {
        g_print("Error opening file");
    }


}
    gtk_widget_destroy(dialog);  // Destroy the dialog
return;
}


//save chat
void on_file_save() {

    // Get current time
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Format the time into a string suitable for a filename
    gchar chatlog_name[100];
    strftime(chatlog_name, sizeof(chatlog_name), "/home/user/.maemo-gpt/history/chat_%Y%m%d_%H%M%S.txt", timeinfo);

    gchar chatlog_display_name[100];
    strftime(chatlog_display_name, sizeof(chatlog_display_name), "/home/user/.maemo-gpt/history/chat_%Y%m%d_%H%M%S.txt_display.log", timeinfo);

//read chat log file
gchar *chat_temp = read_file("/home/user/.maemo-gpt/chat-history.log");

    // Open the file for writing, creating it if it doesn't exist
    FILE *filesave = fopen(chatlog_name, "wb");
 if (filesave == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(filesave, "%s", chat_temp);

    fclose(filesave);

   printf("File created: %s\n", chatlog_name);


gchar *chat_display_temp = read_file("/home/user/.maemo-gpt/response_display.log");

   FILE *filesavedisplay = fopen(chatlog_display_name, "wb");
 if (filesavedisplay == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(filesavedisplay, "%s", chat_display_temp);

    fclose(filesavedisplay);

   printf("File created: %s\n", chatlog_display_name);


    return;
}


// Function to generate image
void on_file_genimage(GtkWidget *widget, gpointer user_data) {

//dialog box

    GtkWidget *dialog;

 dialog = gtk_dialog_new_with_buttons("Describe your image:",
                                         NULL,
                                         GTK_DIALOG_MODAL,
                                         GTK_STOCK_OK,
                                         GTK_RESPONSE_OK,
                                         GTK_STOCK_CANCEL,
                                         GTK_RESPONSE_CANCEL,
                                         NULL);

    // Create an entry widget
    GtkWidget *entry = gtk_entry_new();
    gtk_widget_set_size_request(entry, 550, 70);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), entry, TRUE, TRUE, 0);

   // Show the dialog and entry
    gtk_widget_show(entry);
    gtk_widget_show(dialog);

    // Run the dialog and wait for user response
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));


    if(result != GTK_RESPONSE_OK) {

     gtk_widget_destroy(dialog);
      return;
    }
    // Handle the user's response
    if(result == GTK_RESPONSE_OK) {
        const gchar *gen_request = gtk_entry_get_text(GTK_ENTRY(entry));
        g_print("User entered: %s\n", gen_request);  // Do something with the input


//curl and display

 CURL *curl;
 CURLcode res;

const gchar *call_back = "/home/user/.maemo-gpt/response_back.log";


curl_global_init(CURL_GLOBAL_DEFAULT);
 curl = curl_easy_init();

   FILE *fpcall;
      fpcall = fopen(call_back, "wb");
        if(!fpcall) {
            perror("File opening failed");
            return;
        }

if(curl) {
 curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/images/generations");


gchar *json_data = g_malloc(1000000 * sizeof(gchar));
      if (json_data == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
       }

snprintf(json_data, 1000000, "{\"model\":\"dall-e-3\",\"prompt\":\"%s\",\"n\":1,\"size\":\"1024x1024\"}", gen_request);

 struct curl_slist *headers = NULL;
 headers = curl_slist_append(headers, "Content-Type: application/json");


const gchar *api_filename = "/home/user/.maemo-gpt/gpt-config.cfg";
    const gchar *key_to_find = "apikey";
    gchar API_key[256];

    read_config_value(api_filename, key_to_find, API_key, sizeof(API_key));


gchar *key_data = g_malloc(1000 * sizeof(gchar));
      if (key_data == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
       }

snprintf(key_data, 1000, "Authorization: Bearer %s", API_key);
headers = curl_slist_append(headers, key_data);


 curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
 curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
 curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback_data_stream);
 curl_easy_setopt(curl, CURLOPT_WRITEDATA, fpcall);


curl_easy_escape(curl, json_data, 0);
curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);

 res = curl_easy_perform(curl);
 if (res != CURLE_OK) {
 GtkTextBuffer *text_buffer = gtk_text_buffer_new(NULL);
 gtk_text_buffer_insert_at_cursor(text_buffer, curl_easy_strerror(res), -1);
 }

curl_slist_free_all(headers);
 curl_easy_cleanup(curl);
fclose(fpcall);
g_free(json_data);
g_free(key_data);

 } else {
        fprintf(stderr, "Failed to initialize CURL.\n");
    }

 curl_global_cleanup();

gtk_widget_destroy(dialog);

}

//extract json
gchar *gen_answer = read_file("/home/user/.maemo-gpt/response_back.log");

//g_print(text_answer);
const gchar* json_str = extract_json(gen_answer);

// Parse the JSON text
 json_error_t error;
json_t *root = json_loads(json_str, 0, &error);

if (!root) {
 g_print("Error parsing JSON: on line %d: %s\n", error.line, error.text);
 } else {

// Create a new string buffer to hold the output

gchar *image_link = g_malloc(1000 * sizeof(gchar));
      if (image_link == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
       }

 json_t *datas = json_object_get(root, "data");

 if (!json_is_array(datas)) {
 fprintf(stderr, "Error: choices is not an array\n");

json_decref(root);
 return;
 }

 json_t *first_choice = json_array_get(datas, 0);

if (!first_choice) {
 fprintf(stderr, "Error: no choices available\n");
 json_decref(root);
 return;
 }

 json_t *url = json_object_get(first_choice, "url");

if (json_is_string(url)) {
 const gchar *response_content = json_string_value(url);
//g_print(response_content);
snprintf(image_link, 1000, "xdg-open \"%s\"", response_content);
g_print(image_link);

gchar* utf8_link = ascii_to_utf8(image_link);

 if (utf8_link != NULL) {
   system(utf8_link);
 g_free(image_link);
 g_free(utf8_link);
 }

 } else {
 g_print("Name field is not a string or does not exist\n");
 }

 json_decref(root);

  }
return;
}


// Function to handle quitting the application
void on_file_exit() {
    gtk_main_quit();
return;
}



void on_file_vision(GtkWidget *widget, gpointer user_data) {

  CallbackData *data = (CallbackData *)user_data;
       GtkTextView *text_view = data->text_view;

    GtkWidget *dialog;

 dialog = gtk_dialog_new_with_buttons("Vision URL and Request:",
                                         NULL,
                                         GTK_DIALOG_MODAL,
                                         GTK_STOCK_OK,
                                         GTK_RESPONSE_OK,
                                         GTK_STOCK_CANCEL,
                                         GTK_RESPONSE_CANCEL,
                                         NULL);

    // Create an entry widget
    GtkWidget *entry = gtk_entry_new();
    gtk_widget_set_size_request(entry, 500, 60);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), entry, TRUE, TRUE, 0);
    gtk_entry_set_text(GTK_ENTRY(entry), "Request");

    GtkWidget *entry2 = gtk_entry_new();
    gtk_widget_set_size_request(entry2, 500, 60);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), entry2, TRUE, TRUE, 0);
    gtk_entry_set_text(GTK_ENTRY(entry2), "URL");


    // Show the dialog and entry
    gtk_widget_show(entry);
    gtk_widget_show(entry2);
    gtk_widget_show(dialog);

    // Run the dialog and wait for user response
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));

    // Handle the user's response
    if(result == GTK_RESPONSE_OK) {
        const gchar *url_value = gtk_entry_get_text(GTK_ENTRY(entry2));
        g_print("User entered: %s\n", url_value);

       const gchar *req_value = gtk_entry_get_text(GTK_ENTRY(entry));
        g_print("User entered: %s\n", req_value);

//curl

CURL *curl;
 CURLcode res;

const gchar *call_back = "/home/user/.maemo-gpt/response_back.log";

 curl_global_init(CURL_GLOBAL_DEFAULT);
 curl = curl_easy_init();

//opening file to write callback and keep it in history log
    FILE *fpcall;
      fpcall = fopen(call_back, "wb");
        if(!fpcall) {
            perror("File opening failed");
            return;
        }

if(curl) {
 // Set the API endpoint URL
 curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");

 // Prepare the JSON data

 gchar *json_data = g_malloc(2000000 * sizeof(gchar));
      if (json_data == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
       }

snprintf(json_data, 2000000, "{\"model\":\"gpt-4o-mini\",\"messages\":[{\"role\":\"user\",\"content\":[{\"type\":\"text\",\"text\":\"%s\"},{\"type\":\"image_url\",\"image_url\":{\"url\":\"%s\"}}]}],\"max_tokens\":300}", req_value, url_value);

g_print(json_data);
// Set up cURL options
 struct curl_slist *headers = NULL;
 headers = curl_slist_append(headers, "Content-Type: application/json; charset=UTF-8");


const gchar *api_filename = "/home/user/.maemo-gpt/gpt-config.cfg";
    const gchar *key_to_find = "apikey";
    gchar API_key[256];

    read_config_value(api_filename, key_to_find, API_key, sizeof(API_key));


gchar *key_data = g_malloc(1000 * sizeof(gchar));
      if (key_data == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
       }

snprintf(key_data, 1000, "Authorization: Bearer %s", API_key);

headers = curl_slist_append(headers, key_data);


 curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
 curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
// curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback_data_stream);
 curl_easy_setopt(curl, CURLOPT_WRITEDATA, fpcall);

 res = curl_easy_perform(curl);

 if (res != CURLE_OK) {
 GtkTextBuffer *text_buffer = gtk_text_buffer_new(NULL);
 gtk_text_buffer_insert_at_cursor(text_buffer, curl_easy_strerror(res), -1);
 }

g_free(json_data);
g_free(key_data);

curl_slist_free_all(headers);
 curl_easy_cleanup(curl);

fclose(fpcall);

 } else {
        fprintf(stderr, "Failed to initialize CURL.\n");
    }

 curl_global_cleanup();


gchar *text_answer = read_file("/home/user/.maemo-gpt/response_back.log");

//very IMPORTANT: removing % char atm to avoid utf8 errors with response
removeQuotes(text_answer);

//g_print(text_answer);
const gchar* json_str = extract_json(text_answer);

json_error_t error;

json_t *root = json_loads(json_str, 0, &error);

 if (!root) {
 g_print("Error parsing JSON: on line %d: %s\n", error.line, error.text);
 } else {


// Access a string field if it exists
 json_t *choices = json_object_get(root, "choices");

 if (!json_is_array(choices)) {
 fprintf(stderr, "Error: choices is not an array\n");

json_decref(root);
 return;
 }


 // Get the first item in the choices array
 json_t *first_choice = json_array_get(choices, 0);
 if (!first_choice) {
 fprintf(stderr, "Error: no choices available\n");

json_decref(root);
 return;
 }

 // Get the message object

json_t *message = json_object_get(first_choice, "message");
 if (!message) {
 fprintf(stderr, "Error: message is not present\n");
 json_decref(root);
return;
 }

 // Get the content field
 json_t *content = json_object_get(message, "content");

if (json_is_string(content)) {
 const gchar *response_content = json_string_value(content);

//save last response for audio TTS
FILE *fptrAudio;
fptrAudio = fopen("/home/user/.maemo-gpt/chataudio.log", "wb");
fprintf(fptrAudio, response_content);
fclose(fptrAudio);

g_print(response_content);

GtkTextBuffer *buffer = gtk_text_buffer_new(NULL);
gtk_text_buffer_set_text(buffer, response_content, -1);
gtk_text_view_set_buffer(text_view, buffer);

 } else {
 g_print("Name field is not a string or does not exist\n");
 }
 // Decrease reference count
 json_decref(root);

}

//end curl code
   }

    // Destroy the dialog after use
    gtk_widget_destroy(dialog);
return;
}



void on_file_list(GtkWidget *file_list_item, gpointer user_data) {

  CallbackData *data = (CallbackData *)user_data;

       GtkTextView *text_view = data->text_view;


 CURL *curl;
 CURLcode res;
curl_global_init(CURL_GLOBAL_DEFAULT);
 curl = curl_easy_init();

const gchar *list_back = "/home/user/.maemo-gpt/response_back.log";

    FILE *fpcall; 
      fpcall = fopen(list_back, "wb");
        if(!fpcall) {
            perror("File opening failed");
            return;
        }

 if(curl) {

// Set the API endpoint URL
 curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/files");
// Set up cURL options
 struct curl_slist *headers = NULL;
 //headers = curl_slist_append(headers, "Content-Type: application/json");


const gchar *api_filename = "/home/user/.maemo-gpt/gpt-config.cfg";
    const gchar *key_to_find = "apikey";
    gchar API_key[256];

    read_config_value(api_filename, key_to_find, API_key, sizeof(API_key));


gchar *key_data = g_malloc(1000 * sizeof(gchar));
      if (key_data == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
       }

snprintf(key_data, 1000, "Authorization: Bearer %s", API_key);
headers = curl_slist_append(headers, key_data);


 curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
// curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);

// Perform the POST request
 curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback_data_stream);
 curl_easy_setopt(curl, CURLOPT_WRITEDATA, fpcall);

res = curl_easy_perform(curl);
 if (res != CURLE_OK) {
 GtkTextBuffer *text_buffer = gtk_text_buffer_new(NULL);
 gtk_text_buffer_insert_at_cursor(text_buffer, curl_easy_strerror(res), -1);
 }


curl_slist_free_all(headers);
 curl_easy_cleanup(curl);

fclose(fpcall);
g_free(key_data);

  } else {
        fprintf(stderr, "Failed to initialize CURL.\n");
    }

curl_global_cleanup();


gchar *list_answer = read_file("/home/user/.maemo-gpt/response_back.log");
//g_print(list_answer);

 GtkTextBuffer *buffer = gtk_text_buffer_new(NULL);
 if (list_answer != NULL) {
 gtk_text_buffer_set_text(buffer, list_answer, -1);
  } else {
        perror("File reading failed");
    }


gtk_text_view_set_buffer(text_view, buffer);
return;
}


void on_file_newmsg(GtkWidget *widget, gpointer user_data) {

  CallbackData *data = (CallbackData *)user_data;
//  GtkEntry *entry = data->entry;
       GtkTextView *text_view = data->text_view;

    GtkWidget *dialog;

 dialog = gtk_dialog_new_with_buttons("New message for Maemo-Leste Assistant:",
                                         NULL,
                                         GTK_DIALOG_MODAL,
                                         GTK_STOCK_OK,
                                         GTK_RESPONSE_OK,
                                         GTK_STOCK_CANCEL,
                                         GTK_RESPONSE_CANCEL,
                                         NULL);

   // Create an entry widget
    GtkWidget *entry = gtk_entry_new();
    gtk_widget_set_size_request(entry, 500, 60);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), entry, TRUE, TRUE, 0);
//    gtk_entry_set_text(GTK_ENTRY(entry), "Request");

    // Show the dialog and entry
    gtk_widget_show(entry);
    gtk_widget_show(dialog);

    gint result = gtk_dialog_run(GTK_DIALOG(dialog));

   // Handle the user's response
    if(result == GTK_RESPONSE_OK) {
        const gchar *request_value = gtk_entry_get_text(GTK_ENTRY(entry));
//        g_print("User entered: %s\n", request_value);

CURL *curl;
 CURLcode res;

const gchar *call_back = "/home/user/.maemo-gpt/response_back.log";

 curl_global_init(CURL_GLOBAL_DEFAULT);
 curl = curl_easy_init();

   FILE *fpmsg;
      fpmsg = fopen(call_back, "wb");
        if(!fpmsg) {
            perror("File opening failed");
            return;
        }


if(curl) {


//read proper line from config file
    const gchar *config_thread = "/home/user/.maemo-gpt/gpt-config.cfg";
    const gchar *key_to_find = "thread";
    gchar thread_value[100];

    read_config_value(config_thread, key_to_find, thread_value, sizeof(thread_value));

                g_print(thread_value);

gchar *thread_id = g_malloc(1000 * sizeof(gchar));
      if (thread_id == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
       }

snprintf(thread_id, 1000, "https://api.openai.com/v1/threads/%s/messages", thread_value);

curl_easy_setopt(curl, CURLOPT_URL, thread_id);

gchar *json_data = g_malloc(2000000 * sizeof(gchar));
      if (json_data == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
       }

snprintf(json_data, 2000000, "{\"role\":\"user\",\"content\":\"%s\"}", request_value);
//g_print(json_data);

struct curl_slist *headers = NULL;
 headers = curl_slist_append(headers, "Content-Type: application/json; charset=UTF-8");
 headers = curl_slist_append(headers, "OpenAI-Beta: assistants=v2");


const gchar *api_filename = "/home/user/.maemo-gpt/gpt-config.cfg";
    const gchar *key_to_find_api = "apikey";
    gchar API_key[256];

    read_config_value(api_filename, key_to_find_api, API_key, sizeof(API_key));


gchar *key_data = g_malloc(1000 * sizeof(gchar));
      if (key_data == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
       }

snprintf(key_data, 1000, "Authorization: Bearer %s", API_key);

headers = curl_slist_append(headers, key_data);

 curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
 curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
// curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback_data_stream);
curl_easy_setopt(curl, CURLOPT_WRITEDATA, fpmsg);

res = curl_easy_perform(curl);

if (res != CURLE_OK) {
 GtkTextBuffer *text_buffer = gtk_text_buffer_new(NULL);
 gtk_text_buffer_insert_at_cursor(text_buffer, curl_easy_strerror(res), -1);
 }


g_free(json_data);
g_free(key_data);

curl_slist_free_all(headers);
 curl_easy_cleanup(curl);
fclose(fpmsg);
g_free(thread_id);

} else {
        fprintf(stderr, "Failed to initialize CURL.\n");
    }

 curl_global_cleanup();

gchar *text_answer = read_file("/home/user/.maemo-gpt/response_back.log");

 GtkTextBuffer *buffer = gtk_text_buffer_new(NULL);
 if (text_answer != NULL) {
 gtk_text_buffer_set_text(buffer, " Your message has been sent !\n\n Now click on 'Run' to proceed.\n\n At any time you can click on 'Show messages' to check messages in the thread.", -1);
  } else {
        perror("Sending message failed");
    }

 gtk_text_view_set_buffer(text_view, buffer);


}
   gtk_widget_destroy(dialog);
return;

}



void on_file_run() {


CURL *curl;
 CURLcode res;

curl_global_init(CURL_GLOBAL_DEFAULT);
 curl = curl_easy_init();

if(curl) {


//read proper line from config file
    const gchar *config_thread = "/home/user/.maemo-gpt/gpt-config.cfg";
    const gchar *key_to_find = "thread";
    gchar thread_value[100];

    read_config_value(config_thread, key_to_find, thread_value, sizeof(thread_value));

                g_print(thread_value);

gchar *thread_id = g_malloc(1000 * sizeof(gchar));
      if (thread_id == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
       }

snprintf(thread_id, 1000, "https://api.openai.com/v1/threads/%s/runs", thread_value);

 // Set the API endpoint URL
curl_easy_setopt(curl, CURLOPT_URL, thread_id);

struct curl_slist *headers = NULL;
 headers = curl_slist_append(headers, "Content-Type: application/json; charset=UTF-8");
 headers = curl_slist_append(headers, "OpenAI-Beta: assistants=v2");


const gchar *api_filename = "/home/user/.maemo-gpt/gpt-config.cfg";
    const gchar *key_to_find_api = "apikey";
    gchar API_key[256];

    read_config_value(api_filename, key_to_find_api, API_key, sizeof(API_key));


gchar *key_data = g_malloc(1000 * sizeof(gchar));
      if (key_data == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
       }

snprintf(key_data, 1000, "Authorization: Bearer %s", API_key);


//read proper line from config file
    const gchar *config_assist = "/home/user/.maemo-gpt/gpt-config.cfg";
    const gchar *key_to_find_assist = "assistant";
    gchar assist_value[100];

    read_config_value(config_assist, key_to_find_assist, assist_value, sizeof(assist_value));

                g_print(assist_value);

gchar *assist_data = g_malloc(1000 * sizeof(gchar));
      if (assist_data == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
       }

snprintf(assist_data, 1000, "{\"assistant_id\":\"%s\",\"instructions\":\"Please address the user as Leste user.\"}", assist_value);



headers = curl_slist_append(headers, key_data);

 curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
// curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"assistant_id\":\"asst_rRhSRwIvvvlzdfwkK9o0WPGl\",\"instructions\":\"Please address the user as Arno.\"}");
 curl_easy_setopt(curl, CURLOPT_POSTFIELDS, assist_data);

// curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

 curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback_data_stream);

 res = curl_easy_perform(curl);

 if (res != CURLE_OK) {
g_print("Curl error");
 }

g_free(key_data);
g_free(thread_id);
g_free(assist_data);

curl_slist_free_all(headers);
 curl_easy_cleanup(curl);

 } else {
        fprintf(stderr, "Failed to initialize CURL.\n");
    }

 curl_global_cleanup();

g_print("Runs OK\n");
return;
}

void on_file_listmsgs(GtkWidget *on_file_listmsgs, gpointer user_data) {


  CallbackData *data = (CallbackData *)user_data;

       GtkTextView *text_view = data->text_view;

 CURL *curl;
 CURLcode res;
 curl_global_init(CURL_GLOBAL_DEFAULT);
 curl = curl_easy_init();

const gchar *list_assistant_back = "/home/user/.maemo-gpt/response_back.log";

   FILE *fplistmsgs;
     fplistmsgs = fopen(list_assistant_back, "wb");
        if(!fplistmsgs) {
            perror("File opening failed");
            return;
        }

 if(curl) {


//read proper line from config file
    const gchar *config_filename = "/home/user/.maemo-gpt/gpt-config.cfg";
    const gchar *key_to_find = "thread";
    gchar thread_value[100];

    read_config_value(config_filename, key_to_find, thread_value, sizeof(thread_value));

                g_print(thread_value);



gchar *thread_id = g_malloc(1000 * sizeof(gchar));
      if (thread_id == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
       }

snprintf(thread_id, 1000, "https://api.openai.com/v1/threads/%s/messages?order=asc", thread_value);


 // Set the API endpoint URL
curl_easy_setopt(curl, CURLOPT_URL, thread_id);

struct curl_slist *headers = NULL;
 headers = curl_slist_append(headers, "Content-Type: application/json; charset=UTF-8");
 headers = curl_slist_append(headers, "OpenAI-Beta: assistants=v2");


const gchar *api_filename = "/home/user/.maemo-gpt/gpt-config.cfg";
    const gchar *key_to_find_api = "apikey";
    gchar API_key[256];

    read_config_value(api_filename, key_to_find_api, API_key, sizeof(API_key));


gchar *key_data = g_malloc(1000 * sizeof(gchar));
      if (key_data == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
       }

snprintf(key_data, 1000, "Authorization: Bearer %s", API_key);

headers = curl_slist_append(headers, key_data);

 curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback_data_stream);
 curl_easy_setopt(curl, CURLOPT_WRITEDATA, fplistmsgs);


res = curl_easy_perform(curl);
 if (res != CURLE_OK) {
 GtkTextBuffer *buffer = gtk_text_buffer_new(NULL);
 gtk_text_buffer_insert_at_cursor(buffer, curl_easy_strerror(res), -1);
 }

curl_slist_free_all(headers);
 curl_easy_cleanup(curl);

fclose(fplistmsgs);
g_free(key_data);
g_free(thread_id);

 } else {
        fprintf(stderr, "Failed to initialize CURL.\n");
    }

 curl_global_cleanup();


gchar *list_answer = read_file("/home/user/.maemo-gpt/response_back.log");
removeQuotes(list_answer);
//g_print(list_answer);


//answer parsing

const gchar* json_string = extract_json(list_answer);
json_error_t error;

json_t *root = json_loads(json_string, 0, &error);

if (!root) {
        fprintf(stderr, "Error loading JSON: %s\n", error.text);

       return;
    }

    // Check if the root is an object
    if (!json_is_object(root)) {
        fprintf(stderr, "Root is not an object\n");
        json_decref(root);
        return;
    }

    // Get the "data" array from the object
    json_t *data_array = json_object_get(root, "data");
    if (!json_is_array(data_array)) {
        fprintf(stderr, "Data is not an array\n");
        json_decref(root);
        return;
    }
    size_t index;
    json_t *data_item;
    // Iterate over each item in the "data" array
    json_array_foreach(data_array, index, data_item) {
        // Get the "groups" array for each data item

       json_t *groups_array = json_object_get(data_item, "content");
        if (!json_is_array(groups_array)) {
            fprintf(stderr, "Groups is not an array\n");
            continue; // Skip to the next data item
        }

            size_t msg_index;
            json_t *message_item;

            // Iterate over each message in the "messages" array
            json_array_foreach(groups_array, msg_index, message_item) {
                // Get the "content" object from each message
                json_t *content_object = json_object_get(message_item, "text");
                if (json_is_object(content_object)) {
                    // Get the "text" field from the content object
                    json_t *text_value = json_object_get(content_object, "value");
                    if (json_is_string(text_value)) {
                        const gchar *assist_list = json_string_value(text_value);


        FILE *fptrassist;
               fptrassist = fopen("/home/user/.maemo-gpt/assistant_list.log", "ab");
                fprintf(fptrassist, "[MESSAGE:]  %s\n\n\n", assist_list);
        fclose(fptrassist);

                    }

            }
        }
    }
    json_decref(root);

//end parsing

gchar *list_def = read_file("/home/user/.maemo-gpt/assistant_list.log");

 GtkTextBuffer *buffer = gtk_text_buffer_new(NULL);
 if (list_def != NULL) {
 gtk_text_buffer_set_text(buffer, list_def, -1);
  } else {
        perror("File reading failed");
    }

   GtkTextIter end_iter;
    gtk_text_buffer_get_end_iter(buffer, &end_iter);
    GtkTextMark *end_mark = gtk_text_buffer_create_mark(buffer, NULL, &end_iter, FALSE);



gtk_text_view_set_buffer(text_view, buffer);


   gtk_text_view_scroll_to_mark(text_view, end_mark, 0.0, TRUE, 0.0, 0.0);



FILE *file_assistant = fopen("/home/user/.maemo-gpt/assistant_list.log", "wb");
 if (file_assistant == NULL) {
 perror("Error cleaning file");
 }
 fclose(file_assistant);
return;
}


//file chooser box, not working yet
void on_file_upload(GtkWidget *widget, gpointer window) {
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new("Open File",
                                         GTK_WINDOW(window),
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         ("_Cancel"),
                                         GTK_RESPONSE_CANCEL,
                                         ("_Open"),
                                         GTK_RESPONSE_OK,
                                         NULL);


    // Run the dialog and get the response
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        gchar *filename;
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        g_print("Selected file: %s\n", filename);
//        g_free(filename);
    }

    gtk_widget_destroy(dialog);  // Destroy the dialog
return;
}



void on_button_clicked1(GtkWidget *button1, gpointer user_data) {

//config file checking, creating one if none exists
const gchar *cfg_dir = "/home/user/.maemo-gpt";

const gchar *cfg_path = "/home/user/.maemo-gpt/gpt-config.cfg";

    // Create the directory
    if (mkdir(cfg_dir, 0777) == -1) {
        perror("Error creating directory");
//        return;
    } else {
        printf("Directory '%s' created successfully.\n", cfg_dir);
    }

    // Try to open the file in read mode to check if it exists
    FILE *file_check_cfg = fopen(cfg_path, "r");
    if (file_check_cfg) {
        // File exists
        fclose(file_check_cfg);
        printf("Configuration file '%s' already exists.\n", cfg_path);
    } else {
        // File does not exist, create it
        file_check_cfg = fopen(cfg_path, "wb"); // Open for writing (creates the file)
        if (file_check_cfg == NULL) {

            perror("Error creating configuration file");
            return;
        }

        // Write default configuration settings to the file
        fprintf(file_check_cfg, "### Maemo-gpt configuration file ###\n\n");
        fprintf(file_check_cfg, "nickname=\n\n");
        fprintf(file_check_cfg, "apikey=sk-proj-\n\n");
	fprintf(file_check_cfg, "assistant=asst_\n\n");
	fprintf(file_check_cfg, "thread=thread_\n\n");
	fprintf(file_check_cfg, "vector_store_id=\n\n");
	fprintf(file_check_cfg, "file_search=\n\n");
	fprintf(file_check_cfg, "code_interpreter=\n\n");

        printf("Configuration file '%s' created successfully.\n", cfg_path);

        // Close the file
        fclose(file_check_cfg);
    }



//important to avoid gtkwidget and textview conflicts
    CallbackData *data = (CallbackData *)user_data;
       GtkEntry *entry = data->entry;
       GtkTextView *text_view = data->text_view;


const gchar *input_temp = gtk_entry_get_text(data->entry);
//very important: convert const char to char
gchar *input_text = g_strdup(input_temp);

removeCharacters(input_text);

// Set up cURL to send a request with the input text to the OpenAI API

 CURL *curl;
 CURLcode res;

const gchar *call_back = "/home/user/.maemo-gpt/response_back.log";

 curl_global_init(CURL_GLOBAL_DEFAULT);
 curl = curl_easy_init();

//opening file to write callback and keep it in history log
    FILE *fpcall;
      fpcall = fopen(call_back, "wb");
        if(!fpcall) {
            perror("File opening failed");
            return;
        }


 if(curl) {

// Set the API endpoint URL
 curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");

 // Prepare the JSON data
 gchar *historydata = read_file("/home/user/.maemo-gpt/chat-history.log");
 removeNewLines(historydata);


 gchar *json_data = g_malloc(2000000 * sizeof(gchar));
      if (json_data == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
       }

snprintf(json_data, 2000000, "{\"model\":\"%s\",\"messages\":[{\"role\":\"system\",\"content\":\"You are a helpful assistant\"},%s{\"role\":\"user\",\"content\":\"%s\"}]}", gpt_model, historydata, input_text);

//debug
//FILE *fptrx;
//fptrx = fopen("/home/user/.maemo-gpt/chat-debug.log", "a");
//fprintf(fptrx, json_data);
//fclose(fptrx);

 // Set up cURL options
 struct curl_slist *headers = NULL;
 headers = curl_slist_append(headers, "Content-Type: application/json; charset=UTF-8");


  const gchar *cfg_filename = "/home/user/.maemo-gpt/gpt-config.cfg";
    const gchar *key_to_find = "apikey";
    gchar API_key[256];

    read_config_value(cfg_filename, key_to_find, API_key, sizeof(API_key));


gchar *key_data = g_malloc(4096 * sizeof(gchar));
      if (key_data == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
       }

snprintf(key_data, 4096, "Authorization: Bearer %s", API_key);

headers = curl_slist_append(headers, key_data);


 curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
 curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
// curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

// Perform the POST request
 curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback_data_stream);
 curl_easy_setopt(curl, CURLOPT_WRITEDATA, fpcall);


//add input request in chat log

gchar *history_log = g_malloc(2000000 * sizeof(gchar));
      if (history_log == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
       }

snprintf(history_log, 2000000, "{\"role\":\"user\",\"content\":\"%s\"},", input_text);

FILE *fptr;
fptr = fopen("/home/user/.maemo-gpt/chat-history.log", "ab+");
fprintf(fptr, history_log);
fclose(fptr);

g_free(history_log);


gchar *quotestemp_log = g_malloc(2000000 * sizeof(gchar));
      if (quotestemp_log == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
       }

snprintf(quotestemp_log, 2000000, "[MY REQUEST]: %s\n\n", input_text);

FILE *fptrxxx;
fptrxxx = fopen("/home/user/.maemo-gpt/response_display.log", "ab+");
fprintf(fptrxxx, quotestemp_log);
fclose(fptrxxx);

g_free(quotestemp_log);
g_free(input_text);
input_text = NULL;

 res = curl_easy_perform(curl);

if (res != CURLE_OK) {
 GtkTextBuffer *text_buffer = gtk_text_buffer_new(NULL);
 gtk_text_buffer_insert_at_cursor(text_buffer, curl_easy_strerror(res), -1);
 }

g_free(json_data);
g_free(key_data);

curl_slist_free_all(headers);
 curl_easy_cleanup(curl);
fclose(fpcall);

 } else {
        fprintf(stderr, "Failed to initialize CURL.\n");
    }

 curl_global_cleanup();

//end

//clean text zone after button clicked
gtk_entry_set_text(GTK_ENTRY(entry), "");

//extract json

gchar *text_answer = read_file("/home/user/.maemo-gpt/response_back.log");

//very IMPORTANT: removing % char atm to avoid utf8 errors with c code response
removeQuotes(text_answer);


//g_print(text_answer);
const gchar* json_str = extract_json(text_answer);

//ent extract

 // Parse the JSON text
 json_error_t error;

json_t *root = json_loads(json_str, 0, &error);

 if (!root) {
 g_print("Error parsing JSON: on line %d: %s\n", error.line, error.text);
 } else {

// Access a string field if it exists
 json_t *choices = json_object_get(root, "choices");

 if (!json_is_array(choices)) {
 fprintf(stderr, "Error: choices is not an array\n");
 json_decref(root);
 return;
 }


 // Get the first item in the choices array
 json_t *first_choice = json_array_get(choices, 0);
 if (!first_choice) {
 fprintf(stderr, "Error: no choices available\n");
 json_decref(root);
 return;
 }

 // Get the message object
 json_t *message = json_object_get(first_choice, "message");
 if (!message) {
 fprintf(stderr, "Error: message is not present\n");
json_decref(root);
return;
 }

 // Get the content field
 json_t *content = json_object_get(message, "content");

if (json_is_string(content)) {
 const gchar *response_content = json_string_value(content); 

//save last response for audio TTS
FILE *fptrAudio;
fptrAudio = fopen("/home/user/.maemo-gpt/chataudio.log", "wb");
fprintf(fptrAudio, response_content);
fclose(fptrAudio);


gchar *output_response = g_malloc(2000000 * sizeof(gchar));
      if (output_response == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
       }

snprintf(output_response, 2000000, "[CHATGPT]: %s\n\n", response_content);


gchar *output_log = g_malloc(2000000 * sizeof(gchar));
      if (output_log == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
       }

snprintf(output_log, 2000000, "%s", output_response);
g_free(output_response);

FILE *fptrz;
fptrz = fopen("/home/user/.maemo-gpt/response_display.log", "ab");
fprintf(fptrz, output_log);
fclose(fptrz);


gchar *output_extra = g_malloc(2000000 * sizeof(gchar));
      if (output_extra == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
       }

snprintf(output_extra, 2000000, "%s", output_log);
removeCharacters(output_extra);
removeNewLines(output_extra);
g_free(output_log);

FILE *fptr2;
fptr2 = fopen("/home/user/.maemo-gpt/chat-history.log", "ab");
fprintf(fptr2, "{\"role\":\"assistant\",\"content\":\"%s\"},", output_extra);
fclose(fptr2);
g_free(output_extra);

 } else {
 g_print("Name field is not a string or does not exist\n");
 }
 // Decrease reference count
 json_decref(root);

// Set the output text in the GtkTextView

gchar *outclean = read_file("/home/user/.maemo-gpt/response_display.log");
 GtkTextBuffer *buffer = gtk_text_buffer_new(NULL);
 if (outclean != NULL) {
 gtk_text_buffer_set_text(buffer, outclean, -1);
  } else {
        perror("File reading failed");
    }


//create a mark at the end of the text buffer
    GtkTextIter end_iter;
    gtk_text_buffer_get_end_iter(buffer, &end_iter);
    GtkTextMark *end_mark = gtk_text_buffer_create_mark(buffer, NULL, &end_iter, FALSE);

 // Set the new buffer in the text view
 gtk_text_view_set_buffer(text_view, buffer);

  // Scroll to the mark
    gtk_text_view_scroll_to_mark(text_view, end_mark, 0.0, TRUE, 0.0, 0.0);

 }

return;
}


