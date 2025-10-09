#include "resource_manager.h"

int main() {
    loadFromFile();
    loadTrashFromFile();
    int choice = -1;
    char buffer[MAX_LINE_LENGTH];
    do {
        displayMenu();
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d", &choice) != 1) {
                choice = -1; // Invalid input, not a number
            }
        } else {
            choice = 0; // EOF reached, exit
        }

        switch (choice) {
            case 1: addResourceUsage(); break;
            case 2: searchResourceUsage(); break;
            case 3: updateResourceUsage(); break;
            case 4: deleteResourceUsage(); break;
            case 5: 
                printAllRecords();
                printf("\nPress Enter to continue...");
                getchar();
                break;
            case 8:
                runUnitTests();
                printf("\nPress Enter to return to the main menu...");
                getchar();
                break;
            case 9:
                runE2ETest();
                printf("\nPress Enter to return to the main menu...");
                getchar();
                break;
            case 0:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                printf("Press Enter to continue...");
                getchar();
        }
    } while (choice != 0);

    return 0;
}
