#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H
#define MAX_LINE_LENGTH 256

void read_config_value();

gboolean is_txt_file();

gchar* ascii_to_utf8();

size_t write_callback_data_stream();

void remove_doublequotes();

void remove_newlines();

gchar* read_file();

#endif
