#ifndef UTILITIES_H
#define UTILITIES_H

#include "cJSON/cJSON.h"

char *replace_substring(const char *str, const char *old_substring, const char *new_substring);
unsigned char print_main_menu(char *file_name);
cJSON *parse_json_file(const char *filename);

#endif // UTILITIES_H
