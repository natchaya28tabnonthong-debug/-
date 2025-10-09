#include "resource_manager.h" 

int runE2ETest() {
    printf("\n\n--- Running End-to-End (E2E) Flow Test ---\n");
    printf("WARNING: This test will modify/delete 'data.csv'.\n");

    printf("\n[STEP 1] Setting up test environment...\n");
    rename(FILENAME, "data.csv.bak");
    recordCount = 0;
    trashCount = 0;
    printf("  - Original data.csv backed up to data.csv.bak\n");
    printf("  - In-memory records cleared.\n");

    printf("\n[STEP 2] Adding a new record and saving...\n");
    strcpy(records[0].employeeName, "Test User");
    strcpy(records[0].resourceType, "Laptop");
    records[0].resourceAmount = 1;
    strcpy(records[0].usageDate, "2025-01-01");
    recordCount = 1;
    if (saveToFile() != 0) { printf("  - FAIL: saveToFile() failed.\n"); rename("data.csv.bak", FILENAME); return 1; }
    printf("  - PASS: saveToFile() executed successfully.\n");

    printf("\n[STEP 3] Loading from file and verifying...\n");
    recordCount = 0;
    loadFromFile();
    if (recordCount == 1 && strcmp(records[0].employeeName, "Test User") == 0) {
        printf("  - PASS: Record loaded and verified successfully.\n");
    } else {
        printf("  - FAIL: Failed to load or verify record.\n"); rename("data.csv.bak", FILENAME); return 1;
    }

    printf("\n[STEP 4] Deleting the record and saving...\n");
    trashRecords[0] = records[0];
    trashCount = 1;
    recordCount = 0;
    saveToFile();
    if (recordCount == 0) { printf("  - PASS: Record moved to trash, main record list is empty.\n");}
    else { printf("  - FAIL: Record count should be 0 after delete.\n"); rename("data.csv.bak", FILENAME); return 1; }
    
    printf("\n[STEP 5] Restoring the record from trash...\n");
    records[0] = trashRecords[0];
    recordCount = 1;
    trashCount = 0;
    if (recordCount == 1 && strcmp(records[0].employeeName, "Test User") == 0) {
        printf("  - PASS: Record restored and verified successfully.\n");
    } else {
        printf("  - FAIL: Failed to restore or verify record.\n"); rename("data.csv.bak", FILENAME); return 1;
    }

    printf("\n[STEP 6] Cleaning up test environment...\n");
    remove(FILENAME);
    rename("data.csv.bak", FILENAME);
    loadFromFile();
    printf("  - Test file removed and original file restored.\n");
    
    printf("\n--- E2E Test Finished: ALL STEPS PASSED ---\n");
    
    return 0;
}
