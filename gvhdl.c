#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON/cJSON.h"
#include "utilities.h"

/*******************************************
    Some definitions
*******************************************/
#define TEMPLATE_NAME "template.vhd"        // Template file
#define BUFFER_SIZE 256                     // Buffer size to read lines


/*******************************************
    main
*******************************************/
int main(int argc, char *argv[]) {

    char *entity, *architecture;    // Entity and architecture names    
    //char buffer[BUFFER_SIZE];       // Buffer to read lines
    //char *lines[24];
    //unsigned int line = 0;
    unsigned char option;
    int index = 0;
    

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

    // Display FPGA kits from JSON data
    cJSON *kit = NULL;    
    cJSON_ArrayForEach(kit, fpga_kits) {
        cJSON *kit_name = cJSON_GetObjectItem(kit, "name");
        printf("%d. %s\n", index + 1, kit_name->valuestring);
        index++;
    }
    
        
    cJSON_Delete(fpga_kits);

    option = print_main_menu(entity);
    return 0;
}

