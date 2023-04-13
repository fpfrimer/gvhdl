#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON/cJSON.h"

/*******************************************
    Some definitions
*******************************************/
#define TEMPLATE_NAME "template.vhd"        // Template file
#define BUFFER_SIZE 256                     // Buffer size to read lines

/*******************************************
    Prototypes
*******************************************/
char *replace_substring(const char *str, const char *old_substring, const char *new_substring);
unsigned char print_main_menu(char *file_name, cJSON *fpga_kits);
cJSON *parse_json_file(const char *filename);

/*******************************************
    main
*******************************************/
int main(int argc, char *argv[]) {

    char *entity, *architecture;    // Entity and architecture names    
    char buffer[BUFFER_SIZE];       // Buffer to read lines
    char *lines[24];
    unsigned int line = 0;
    unsigned char option;
    

    // Check if the correct number of arguments has been provided
    if (argc != 3) {
        printf("Error. Use: %s <entity name> <architecture name>\n", argv[0]);
        return 1;
    }

    // Entity and architecture names
    entity = argv[1];
    architecture = argv[2];

    // Parse JSON file and extract FPGA kit information
    cJSON *fpga_kits = parse_json_file("fpga_kits.json");

    // Menu
    option = print_main_menu(entity, fpga_kits);
    while (1){
        switch (option){
        case 1: // Create file
            // Open file for reading
            FILE *file = fopen(TEMPLATE_NAME, "r");
            if (file == NULL) {
                printf("Error opening the file '%s'.\n", TEMPLATE_NAME);
                exit(EXIT_FAILURE);
            }

            while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
                line = 0;
                lines[line] = replace_substring(buffer, "<entity_name>", entity);
                lines[line] = replace_substring(lines[line], "<architecture_name>", architecture);
                printf("%s", lines[line]);
                line++;        
            }

            // Close the file
            fclose(file);
            return 0;
            break;

        case 2: // Kit DE10 Lite interfaces
            printf("Still under development.\n");
            break;

        case 3: // Show template
            printf("Still under development.\n");
            break;

        case 4: // Quit
            printf("Good by!.\n");
            return 0;
            break;
        
        default:
            return 1;
            break;
        }
    }
    
        
    cJSON_Delete(fpga_kits);
    return 0;
}

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
unsigned char print_main_menu(char *file_name, cJSON *fpga_kits) {
    unsigned char choice;

    printf("\nMenu:\n");
    printf("1. Done! create file %s.vhd.\n", file_name);
    
    // Display FPGA kits from JSON data
    cJSON *kit = NULL;
    cJSON_ArrayForEach(kit, fpga_kits) {
        printf("%d. %s\n", kit->valueint, kit->string);
    }

    printf("%d. Show template.\n", cJSON_GetArraySize(fpga_kits) + 1);
    printf("%d. Cancel and quit.\n", cJSON_GetArraySize(fpga_kits) + 2);
    printf("Enter the number of the chosen option: ");
    scanf("%c", &choice);

    while (choice < 1 || choice > cJSON_GetArraySize(fpga_kits) + 2) {
        printf("Invalid option. Please, enter again: ");
        scanf("%c", &choice);
    }

    return choice;
}

/*  *parse_json_file
    function to parse the JSON file and extract FPGA kits information
*/
cJSON *parse_json_file(const char *filename) {
    cJSON *json_data = NULL;
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening the file '%s'.\n", filename);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = (char *)malloc(length + 1);
    if (data == NULL) {
        printf("Error allocating memory.\n");
        exit(EXIT_FAILURE);
    }

    fread(data, 1, length, file);
    fclose(file);
    data[length] = '\0';

    json_data = cJSON_Parse(data);
    if (json_data == NULL) {
        printf("Error parsing JSON data.\n");
        exit(EXIT_FAILURE);
    }

    free(data);
    return json_data;
}
