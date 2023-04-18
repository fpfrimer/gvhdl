#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON/cJSON.h"
#include "utilities.h"

/*******************************************
    Functions
*******************************************/

/*  *replace_substring
    The replace_substring function takes three arguments: the original string, the substring to be replaced, and the new string that will replace the substring. The function returns a new string with the replaced substring.*/
char *replace_substring(const char *str, const char *old_substring, const char *new_substring) {
    char *result;
    int i, count = 0;
    int new_substring_length = strlen(new_substring);
    int old_substring_length = strlen(old_substring);

    // Contar o número de ocorrências do trecho na string
    for (i = 0; str[i] != '\0'; i++) {
        if (strstr(&str[i], old_substring) == &str[i]) {
            count++;
            i += old_substring_length - 1;
        }
    }

    // Alocar memória para a nova string
    result = (char *)malloc(i + count * (new_substring_length - old_substring_length) + 1);

    // Substituir o trecho
    char *ptr;
    i = 0;
    while ((ptr = strstr(str, old_substring)) != NULL) {
        strncpy(result + i, str, ptr - str);
        i += ptr - str;
        strcpy(result + i, new_substring);
        i += new_substring_length;
        str = ptr + old_substring_length;
    }
    strcpy(result + i, str);

    return result;
}

/*  print_main_menu
    The print_main_menu function prints the menu, reads the user input, and validates if the chosen option is between 1 and 4. If the option is invalid, the user will be prompted to enter again until a valid option is selected. The function returns the value of the chosen option as a char.

*/
unsigned char print_main_menu(char *file_name) {
    unsigned char choice;

    printf("\nMenu:\n");
    printf("1. Done! create file %s.vhd.\n", file_name);
    printf("2. Show template.\n");
    printf("3. Add interface.\n");
    printf("4. Cancel and quit.\n\n");
    printf("Enter your option: ");
    scanf("%c", &choice);    

    return choice;
}

/*  *parse_json_file
    function to parse the JSON file and extract FPGA kits information
*/
cJSON *parse_json_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening the file '%s'.\n", filename);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = (char *)calloc(length + 1, sizeof(char));
    if (data == NULL) {
        printf("Error allocating memory for JSON data.\n");
        exit(EXIT_FAILURE);
    }

    size_t bytes_read = fread(data, sizeof(char), length, file);
    fclose(file);

    if (bytes_read != length) {
        printf("Error reading JSON data from file.\n");
        exit(EXIT_FAILURE);
    }

    cJSON *json_data = cJSON_Parse(data);
    if (json_data == NULL) {
        printf("Error parsing JSON data.\n");
        exit(EXIT_FAILURE);
    }

    cJSON *fpga_kits = cJSON_GetObjectItem(json_data, "kits");

    // No need to delete json_data here
    free(data);

    return fpga_kits;
}



