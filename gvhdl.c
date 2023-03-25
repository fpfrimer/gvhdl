#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*******************************************
    Some definitions
*******************************************/
#define TEMPLATE_NAME "template.vhd"        // Template file
#define BUFFER_SIZE 256                     // Buffer size to read lines

/*******************************************
    Prototypes
*******************************************/
char *replace_substring(const char *str, const char *old_substring, const char *new_substring);
char print_main_menu(char *file_name);

/*******************************************
    main
*******************************************/
int main(int argc, char *argv[]) {

    char *entity, *architecture;    // Entity and architecture names    
    char buffer[BUFFER_SIZE];       // Buffer to read lines
    char *lines[24];
    unsigned int line = 0;
    char option;
    

    // Check if the correct number of arguments has been provided
    if (argc != 3) {
        printf("Error. Use: %s <entity name> <architecture name>\n", argv[0]);
        return 1;
    }

    // Entity and architecture names
    entity = argv[1];
    architecture = argv[2];

    // Menu
    option = print_main_menu(entity);
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
char print_main_menu(char *file_name){
    int choice;

    printf("\nMenu:\n");
    printf("1. Done! create file %s.vhd.\n", file_name);
    printf("2. Kit DE10 Lite interfaces.\n");
    printf("3. Show template.\n");
    printf("4. Cancel and quit.\n");
    printf("Enter the number of the chosen option: ");
    scanf("%d", &choice);

    while (choice < 1 || choice > 4) {
        printf("Invalid option. Please, enter again: ");
        scanf("%d", &choice);
    }

    return (char)choice;
}
