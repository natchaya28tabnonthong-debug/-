#include "resource_manager.h"

ResourceUsage records[MAX_RECORDS];
int recordCount = 0;
ResourceUsage trashRecords[MAX_RECORDS];
int trashCount = 0;
const char* FILENAME = "data.csv";
const char* TRASH_FILENAME = "trash.csv";
char* toLower(char* s) {
    for (char* p = s; *p; ++p) {
        *p = tolower((unsigned char)*p);
    }
    return s;
}

int is_valid_name(const char* name) {
    if (strlen(name) == 0) return 0;
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && !isspace(name[i])) {
            return 0;
        }
    }
    return 1;
}

int is_valid_amount(const char* amount) {
    if (strlen(amount) == 0) return 0;
    for (int i = 0; amount[i] != '\0'; i++) {
        if (!isdigit(amount[i])) {
            return 0;
        }
    }
    return 1;
}

int is_valid_date(const char* date) {
    int year, month, day;
    if (sscanf(date, "%d-%d-%d", &year, &month, &day) != 3) return 0;
    if (strlen(date) != 10) return 0;
    if (date[4] != '-' || date[7] != '-') return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1) return 0;
    if (month == 2) {
        int is_leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if ((is_leap && day > 29) || (!is_leap && day > 28)) return 0;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) return 0;
    } else {
        if (day > 31) return 0;
    }
    return 1;
}

void format_name_capitalization(char* name) {
    int capitalize_next = 1;
    for (int i = 0; name[i] != '\0'; i++) {
        if (isspace(name[i])) {
            capitalize_next = 1;
        } else if (capitalize_next && isalpha(name[i])) {
            name[i] = toupper(name[i]);
            capitalize_next = 0;
        } else {
            name[i] = tolower(name[i]);
        }
    }
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int loadFromFile() {
    FILE* file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("No data file found. Starting a new session.\n");
        return 0;
    }
    char line[MAX_LINE_LENGTH];
    recordCount = 0;
    fgets(line, sizeof(line), file); // ข้าม Header

    while (fgets(line, sizeof(line), file) && recordCount < MAX_RECORDS) {
        int items_matched = sscanf(line, "%99[^,],%49[^,],%d,%19[^\n]",
                 records[recordCount].employeeName,
                 records[recordCount].resourceType,
                 &records[recordCount].resourceAmount,
                 records[recordCount].usageDate);
        if (items_matched == 4) {
            recordCount++;
        }
    }
    fclose(file);
    printf("Successfully loaded %d record(s) from %s.\n", recordCount, FILENAME);
    return 0;
}

int loadTrashFromFile() {
    FILE* file = fopen(TRASH_FILENAME, "r");
    if (file == NULL) {
        return 0;
    }
    char line[MAX_LINE_LENGTH];
    trashCount = 0;
    fgets(line, sizeof(line), file); 

    while (fgets(line, sizeof(line), file) && trashCount < MAX_RECORDS) {
        int items_matched = sscanf(line, "%99[^,],%49[^,],%d,%19[^\n]",
                 trashRecords[trashCount].employeeName,
                 trashRecords[trashCount].resourceType,
                 &trashRecords[trashCount].resourceAmount,
                 trashRecords[trashCount].usageDate);
        if (items_matched == 4) {
            trashCount++;
        }
    }
    fclose(file);
    return 0;
}
int saveToFile() {
    FILE* file = fopen(FILENAME, "w");
    if (file == NULL) {
        perror("Error: Could not open file for writing");
        return -1;
    }
    fprintf(file, "EmployeeName,ResourceType,ResourceAmount,UsageDate\n");
    for (int i = 0; i < recordCount; i++) {
        fprintf(file, "%s,%s,%d,%s\n",
                records[i].employeeName,
                records[i].resourceType,
                records[i].resourceAmount,
                records[i].usageDate);
    }
    fclose(file);
    printf("Successfully saved %d record(s) to %s.\n", recordCount, FILENAME);
    return 0;
}
int saveTrashToFile() {
    FILE* file = fopen(TRASH_FILENAME, "w");
    if (file == NULL) {
        perror("Error: Could not open trash file for writing");
        return -1;
    }
    fprintf(file, "EmployeeName,ResourceType,ResourceAmount,UsageDate\n");
    for (int i = 0; i < trashCount; i++) {
        fprintf(file, "%s,%s,%d,%s\n",
                trashRecords[i].employeeName,
                trashRecords[i].resourceType,
                trashRecords[i].resourceAmount,
                trashRecords[i].usageDate);
    }
    fclose(file);
    return 0;
}

int addResourceUsage() {
    if (recordCount >= MAX_RECORDS) {
        printf("Error: Cannot add new record. The database is full.\n");
        return -1;
    }

    ResourceUsage newRecord;
    char buffer[MAX_LINE_LENGTH];
    int is_valid;

    
    is_valid = 0;
    do {
        printf("Enter employee name (or type 'exit' to cancel): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        if (strcmp(buffer, "exit") == 0) {
            printf("\nAdding record cancelled.\n");
            return -1;
        }
        if (is_valid_name(buffer)) {
            is_valid = 1;
        } else {
            printf("Invalid input. Please use only letters (a-z, A-Z) and spaces.\n");
        }
    } while (!is_valid);
    format_name_capitalization(buffer);
    strcpy(newRecord.employeeName, buffer);

  
    printf("Enter resource type (or type 'exit' to cancel): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strcmp(buffer, "exit") == 0) {
        printf("\nAdding record cancelled.\n");
        return -1;
    }
    strcpy(newRecord.resourceType, buffer);

  
    is_valid = 0;
    do {
        printf("Enter amount used (or type 'exit' to cancel): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        if (strcmp(buffer, "exit") == 0) {
             printf("\nAdding record cancelled.\n");
             return -1;
        }
        if (is_valid_amount(buffer)) {
             is_valid = 1;
        } else {
             printf("Invalid input. Please use only numbers (0-9).\n");
        }
    } while (!is_valid);
    newRecord.resourceAmount = atoi(buffer);
     is_valid = 0;
    do {
    printf("Enter usage date (YYYY-MM-DD or type 'exit' to cancel): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strcmp(buffer, "exit") == 0) {
        printf("\nAdding record cancelled.\n");
        return -1;
    }
    if (is_valid_date(buffer)) {
        is_valid = 1;
    } else {
        printf("Invalid date format. Please use YYYY-MM-DD.\n");
    }
    } while (!is_valid);
    strcpy(newRecord.usageDate, buffer);
 
    int confirmed = 0;
    do {
        printf("\n===== Please confirm new record =====\n");
        printf("1. Name:     %s\n", newRecord.employeeName);
        printf("2. Resource: %s\n", newRecord.resourceType);
        printf("3. Amount:   %d\n", newRecord.resourceAmount);
        printf("4. Date:     %s\n", newRecord.usageDate);
        printf("-----------------------------------\n");
        printf("Is this information correct? (y/n): ");

        char confirm_choice = 'n';
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, " %c", &confirm_choice);

        if (confirm_choice == 'y' || confirm_choice == 'Y') {
            confirmed = 1;
        } else {
            printf("Adding record cancelled. Please start over.\n");
            return -1; 
        }
    } while (!confirmed);

    records[recordCount++] = newRecord;
    printf("\nRecord added successfully!\n");
    saveToFile();
    
    printAllRecords();
    printf("\nPress Enter to return to the main menu...");
    getchar();
    
    return 0;
}

int is_substring_match(const char* base_string, const char* search_term) {
    char temp_base[100];
    strcpy(temp_base, base_string);
    return strstr(toLower(temp_base), search_term) != NULL;
}

int searchResourceUsage() {
    int choice = -1;
    char buffer[MAX_LINE_LENGTH];

    do {
        printf("\nSearch by:\n");
        printf("1. Employee Name\n");
        printf("2. Resource Type\n");
        printf("3. Usage Date\n");
        printf("4. ID\n");
        printf("------------------\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");
        
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, "%d", &choice);

        if (choice >= 1 && choice <= 4) {
            char searchTerm[100];
            printf("Enter search term: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            strcpy(searchTerm, buffer);

            char lowerSearchTerm[100];
            strcpy(lowerSearchTerm, searchTerm);
            toLower(lowerSearchTerm);

            int foundCount = 0;
            printf("\n===== Search Results =====\n");
            printf("--------------------------------------------------------------------------\n");
            printf("%-5s | %-22s | %-20s | %-8s | %s\n", "ID", "Employee Name", "Resource Type", "Amount", "Usage Date");
            printf("------|------------------------|----------------------|----------|------------\n");
            
            for (int i = 0; i < recordCount; i++) {
                int match = 0;
                switch (choice) {
                    case 1:
                        if (is_substring_match(records[i].employeeName, lowerSearchTerm)) match = 1;
                        break;
                    case 2:
                        if (is_substring_match(records[i].resourceType, lowerSearchTerm)) match = 1;
                        break;
                    case 3:
                        if (is_substring_match(records[i].usageDate, lowerSearchTerm)) match = 1;
                        break;
                    case 4: { 
                        int searchId = atoi(searchTerm);
                        if (searchId == i) { 
                            match = 1;
                        }
                        break;
                    }
                }

                if (match) {
                    printf("%-5d | %-22s | %-20s | %-8d | %s\n",
                           i,
                           records[i].employeeName, 
                           records[i].resourceType, 
                           records[i].resourceAmount, 
                           records[i].usageDate);
                    foundCount++;
                }
            }

            if (foundCount == 0) {
                printf("No records found matching your query.\n");
            }

            printf("\nPress Enter to continue...");
            getchar();

        } else if (choice != 0) {
            printf("Invalid choice. Please try again.\n");
            printf("Press Enter to continue...");
            getchar();
        }

    } while (choice != 0);

    printf("Returning to main menu...\n");
    return 0;
}

int updateResourceUsage() {
    int id = -1;
    char buffer[MAX_LINE_LENGTH];
    
    printAllRecords();
    if (recordCount == 0) {
        printf("\nNo records to update. Press Enter to return to menu...");
        getchar();
        return 0;
    }

    printf("\nEnter the ID of the record to update (from the list above): ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &id);

    if (id < 0 || id >= recordCount) {
        printf("Error: Invalid ID.\n");
        return -1;
    }

    int choice = -1;
    do {
        printf("\n===== Editing Record ID: %d =====\n", id);
        printf("1. Name:     %s\n", records[id].employeeName);
        printf("2. Resource: %s\n", records[id].resourceType);
        printf("3. Amount:   %d\n", records[id].resourceAmount);
        printf("4. Date:     %s\n", records[id].usageDate);
        printf("--------------------------------\n");
        printf("0. Save and Finish Editing\n");
        printf("Enter the number of the field you want to edit: ");
        
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, "%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter new employee name: ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                if(is_valid_name(buffer)){
                    format_name_capitalization(buffer);
                    strcpy(records[id].employeeName, buffer);
                } else { printf("Invalid name format.\n"); }
                break;
            case 2:
                printf("Enter new resource type: ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                strcpy(records[id].resourceType, buffer);
                break;
            case 3:
                printf("Enter new amount used: ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                if(is_valid_amount(buffer)){
                    records[id].resourceAmount = atoi(buffer);
                } else { printf("Invalid amount format.\n"); }
                break;
            case 4:
                printf("Enter new usage date (YYYY-MM-DD): ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                if(is_valid_date(buffer)){
                    strcpy(records[id].usageDate, buffer);
                } else { printf("Invalid date format.\n"); }
                break;
            case 0: printf("Finishing update...\n"); break;
            default: printf("Invalid option, please try again.\n"); break;
        }
    } while (choice != 0);

    saveToFile();
    printf("Record updated successfully!\n");
    return 0;
}

void printTrashRecords() {
    printf("\n==== Records in Trash Bin ====\n");
    if (trashCount == 0) {
        printf("Trash bin is empty.\n");
        return;
    }
    printf("------------------------------------------------------------------------------\n");
    printf("%-5s | %-22s | %-20s | %-8s | %s\n", "ID", "Employee Name", "Resource Type", "Amount", "Usage Date");
    printf("------|------------------------|----------------------|----------|------------\n");
    for (int i = 0; i < trashCount; i++) {
        printf("%-5d | %-22s | %-20s | %-8d | %s\n",
               i, trashRecords[i].employeeName, trashRecords[i].resourceType, trashRecords[i].resourceAmount, trashRecords[i].usageDate);
    }
}

int restoreFromTrash() {
    if (trashCount == 0) {
        printf("Trash bin is empty. Nothing to restore.\n");
        return 0;
    }
    
    printTrashRecords();

    int id = -1;
    char buffer[MAX_LINE_LENGTH];
    printf("\nEnter the ID of the record to restore (from the list above): ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &id);

    if (id < 0 || id >= trashCount) {
        printf("Error: Invalid ID in trash.\n");
        return -1;
    }
    
    printf("\n==== You are about to restore this record ====\n");
    printf("--------------------------------------------------------------------------\n");
    printf("%-5s | %-22s | %-20s | %-8s | %s\n", "ID", "Employee Name", "Resource Type", "Amount", "Usage Date");
    printf("------|------------------------|----------------------|----------|------------\n");
    printf("%-5d | %-22s | %-20s | %-8d | %s\n",
           id,
           trashRecords[id].employeeName, 
           trashRecords[id].resourceType, 
           trashRecords[id].resourceAmount, 
           trashRecords[id].usageDate);
    printf("--------------------------------------------------------------------------\n");

    char confirm = 'n';
    printf("Are you sure you want to restore this record? (y/n): ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, " %c", &confirm);

    if (confirm == 'y' || confirm == 'Y') {
        if (recordCount >= MAX_RECORDS) {
            printf("Error: Main database is full. Cannot restore.\n");
            return -1;
        }

        records[recordCount++] = trashRecords[id];
        printf("Restoring record...\n");

       
        for (int i = id; i < trashCount - 1; i++) {
            trashRecords[i] = trashRecords[i+1];
        }
        trashCount--;

        printf("Record restored successfully!\n");
        saveToFile();
        saveTrashToFile();
    } else {
        printf("Restoration cancelled.\n");
    }
    return 0;
}

int deleteResourceUsage() {
    int choice = -1;
    char buffer[MAX_LINE_LENGTH];

    do {
        printf("\n==== Delete / Restore Menu ====\n");
        printf("1. Delete a record\n");
        printf("2. Restore a record from trash\n");
        printf("-----------------------------\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");
        
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, "%d", &choice);

        switch(choice) {
            case 1: { 
                printAllRecords();
                if (recordCount == 0) {
                    printf("\nNo records to delete. Press Enter to continue...");
                    getchar();
                    break;
                }

                int id_to_delete = -1;
                printf("\nEnter the ID of the record to delete (from the list above): ");
                fgets(buffer, sizeof(buffer), stdin);
                sscanf(buffer, "%d", &id_to_delete);

                if (id_to_delete < 0 || id_to_delete >= recordCount) {
                    printf("Error: Invalid ID.\n");
                    break;
                }

                printf("\n==== You are about to delete this record ====\n");
                printf("--------------------------------------------------------------------------------\n");
                printf("%-5s | %-22s | %-20s | %-8s | %s\n", "ID", "Employee Name", "Resource Type", "Amount", "Usage Date");
                printf("------|------------------------|----------------------|----------|----------------\n");
                printf("%-5d | %-22s | %-20s | %-8d | %s\n",
                       id_to_delete,
                       records[id_to_delete].employeeName, 
                       records[id_to_delete].resourceType, 
                       records[id_to_delete].resourceAmount, 
                       records[id_to_delete].usageDate);
                printf("--------------------------------------------------------------------------\n");

                char confirm = 'n';
                printf("Are you sure you want to delete this record? (y/n): ");
                fgets(buffer, sizeof(buffer), stdin);
                sscanf(buffer, " %c", &confirm);

                if (confirm == 'y' || confirm == 'Y') {
                    if (trashCount >= MAX_RECORDS) {
                        printf("Error: Trash bin is full. Cannot delete.\n");
                        break;
                    }
                    trashRecords[trashCount++] = records[id_to_delete];

                    printf("Moving record to trash bin...\n");

                    for (int i = id_to_delete; i < recordCount - 1; i++) {
                        records[i] = records[i + 1];
                    }
                    recordCount--;
                    
                    printf("Record moved to trash successfully!\n");
                    saveToFile();
                    saveTrashToFile();
                } else {
                    printf("Deletion cancelled.\n");
                }
                break;
            }
            case 2:
                restoreFromTrash();
                break;
            case 0:
                printf("Returning to main menu...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
        if(choice != 0) {
            printf("\nPress Enter to return to the Delete/Restore menu...");
            getchar();
        }

    } while (choice != 0);

    return 0;
}

int printAllRecords() {
    printf("\n===== All Records =====\n");
    if (recordCount == 0) {
        printf("No data available.\n");
        return 0;
    }
    printf("------------------------------------------------------------------------------\n");
    printf("%-5s | %-22s | %-20s | %-8s | %s\n", "ID", "Employee Name", "Resource Type", "Amount", "Usage Date");
    printf("------|------------------------|----------------------|----------|------------\n");
    for (int i = 0; i < recordCount; i++) {
        printf("%-5d | %-22s | %-20s | %-8d | %s\n",
               i, records[i].employeeName, records[i].resourceType, records[i].resourceAmount, records[i].usageDate);
        printf("------------------------------------------------------------------------------\n");
    }
    return recordCount;
}

int displayMenu() {
    printf("\n============== MENU ==============\n");
    printf("1. Add Usage Record\n");
    printf("2. Search Records\n");
    printf("3. Update a Record\n");
    printf("4. Delete / Restore Records\n");
    printf("5. Display All Records\n");
    printf("-------------------------------------\n");
    printf("8. Run Unit Tests\n");
    printf("9. Run E2E Flow Test\n");
    printf("-------------------------------------\n");
    printf("0. Exit\n");
    printf("=====================================\n");
    printf("Enter your choice: ");
    return 0;
}
