#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#include "file_handling.h"



// Function to read a specific line from a configuration file
void read_config_value(const gchar *filename, const gchar *key, gchar *value, size_t value_size) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Could not open file");
        return;
    }

    gchar line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        // Strip newline character from the line
        line[strcspn(line, "\n")] = 0;

        // Check if the line starts with the key followed by '='
        if (strncmp(line, key, strlen(key)) == 0 && line[strlen(key)] == '=') {
            // Copy the value part after '=' into value
            strncpy(value, line + strlen(key) + 1, value_size - 1);
            value[value_size - 1] = '\0'; // Ensure null-termination
            fclose(file);
            return;
        }
    }

    printf("Key not found: %s\n", key);
    fclose(file);
}




//function to check text files
gboolean is_txt_file(const gchar *filetext) {
    const gchar *extension = strrchr(filetext, '.');
    return extension && strcmp(extension, ".txt") == 0;
}


//function to convert ascii to utf8
gchar* ascii_to_utf8(const gchar* ascii_text) {

    size_t length = strlen(ascii_text);

    gchar* utf8_text = (gchar*)g_malloc(length + 1);
    if (utf8_text == NULL) {
        perror("Unable to allocate memory");
        return NULL;
    }

    strcpy(utf8_text, ascii_text);

    return utf8_text;
}



// Write callback for custom data handling
size_t write_callback_data_stream(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}


//function to remove problematic characters from file
void remove_doublequotes(const gchar *input_file, const gchar *output_file) {
 FILE *in = fopen(input_file, "r");
 if (!in) {
 perror("Failed to open input file");
 return;
 }

FILE *out = fopen(output_file, "w");
 if (!out) {
 perror("Failed to open output file");
 fclose(in);
 return;
 }

int ch;
 while ((ch = fgetc(in)) != EOF) {
 if (ch == '\"') {
continue;
}
 if (ch == '{') {
continue;
}
if (ch == '}') {
continue;
}
if (ch == '\b') {
continue;
}
if (ch == '\f') {
continue;
}
if (ch == '\r') {
continue;
}
if (ch == '\t') {
continue;
}
if (ch == '\\') {
continue;
}

fputc(ch, out); // Write the character
 }

 fclose(in);
 fclose(out);
}


//remove new lines for json and escape backslash
void remove_newlines(const gchar *input_file, const gchar *output_file) {
 FILE *in = fopen(input_file, "r");
 if (!in) {
 perror("Failed to open input file");
 return;
 }

 FILE *out = fopen(output_file, "w");
 if (!out) {
 perror("Failed to open output file");
 fclose(in);
 return;
 }

int ch;
 while ((ch = fgetc(in)) != EOF) {
 if (ch == '\n') {
continue;
}
else if (ch == '\\') {
 fputc('\\', out); // Write an escaped backslash
}

 fputc(ch, out); // Write the character
 }

 fclose(in);
 fclose(out);
}


// Function to read the content of history file into a string
gchar* read_file(const gchar *filenamex) {
 FILE *fileread = fopen(filenamex, "r");
 if (!fileread) {
 perror("Could not open file");
 return NULL;
 }

 fseek(fileread, 0, SEEK_END);
 long length = ftell(fileread);
 fseek(fileread, 0, SEEK_SET);

 gchar *contenth = g_malloc(length + 1);
 if (contenth) {
 fread(contenth, 1, length, fileread);
 contenth[length] = '\0'; // Null-terminate the string
 }

 fclose(fileread);
return contenth;
g_free(contenth);
}

