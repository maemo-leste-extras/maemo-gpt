#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
//#include <curl/curl.h>
#include <string.h>
//#include <jansson.h>
#include <glib.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#include "parsing.h"

//json extract function
const gchar* extract_json(const gchar* exttext) {
 const gchar *start = strchr(exttext, '{');
 const gchar *end = strrchr(exttext, '}');

 if (start && end && start < end) {
 size_t length = end - start + 1;
 gchar *json_str = (gchar*) g_malloc(length + 1);
 strncpy(json_str, start, length);
 json_str[length] = '\0';
 return json_str;
g_free(json_str);
 }

return NULL;
}


//function to remove problematic characters in json
void removeCharacters(gchar *str) {
if (str != NULL) {
    int i, j = 0;
    int len = strlen(str);
    gchar result[len + 1]; // +1 for the null terminator

    for (i = 0; i < len; i++) {

        if (str[i] != '"' && str[i] != '\b' && str[i] != '\f' && str[i] != '\r' && str[i] != '\t' && str[i] != '%' && str[i] != '$' && str[i] != '\\' && str[i] != '{' && str[i] != '}') {
            result[j++] = str[i]; // Add character to the result
        }
    }
    result[j] = '\0'; // Null terminate the result string
    strcpy(str, result); // Copy the result back to the original string
 } else {
return;
   }

}


void removeNewLines(gchar *linestr) {
if (linestr != NULL) {
    int i, j = 0;
    int len = strlen(linestr);
    gchar lineresult[len + 1]; // +1 for the null terminator

    for (i = 0; i < len; i++) {

        if (linestr[i] != '\n') {
            lineresult[j++] = linestr[i];
        }
    }
    lineresult[j] = '\0'; // Null terminate the result string
    strcpy(linestr, lineresult); // Copy the result back to the original string
} else {
return;
   }

}


//remove % to avoid segfault
void removeQuotes(gchar *linestrx) {
if (linestrx != NULL) {
    int i, j = 0;
    int len = strlen(linestrx);
    gchar lineresultx[len + 1]; // +1 for the null terminator

    for (i = 0; i < len; i++) {

 if (linestrx[i] != '%') {
            lineresultx[j++] = linestrx[i]; // Add character to the result
        }
    }
    lineresultx[j] = '\0'; // Null terminate the result string
    strcpy(linestrx, lineresultx); // Copy the result back to the original string
} else {
return;
   }

}


