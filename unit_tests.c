#include "resource_manager.h"

int runUnitTests() {
    int passed = 0;
    int failed = 0;
    char testStr[100];

    printf("\n\n--- Running Unit Tests ---\n");

    printf("\n[TESTING] is_valid_date():\n");
    if (is_valid_date("2025-10-09")) { printf("  PASS: Valid date\n"); passed++; } else { printf("  FAIL: Valid date\n"); failed++; }
    if (!is_valid_date("2025-13-01")) { printf("  PASS: Invalid month\n"); passed++; } else { printf("  FAIL: Invalid month\n"); failed++; }
    if (!is_valid_date("2025-02-29")) { printf("  PASS: Invalid leap day\n"); passed++; } else { printf("  FAIL: Invalid leap day\n"); failed++; }
    if (is_valid_date("2024-02-29")) { printf("  PASS: Valid leap day\n"); passed++; } else { printf("  FAIL: Valid leap day\n"); failed++; }
    if (!is_valid_date("2025-10-9")) { printf("  PASS: Invalid format (short day)\n"); passed++; } else { printf("  FAIL: Invalid format (short day)\n"); failed++; }
    if (!is_valid_date("2025/10/09")) { printf("  PASS: Invalid format (slashes)\n"); passed++; } else { printf("  FAIL: Invalid format (slashes)\n"); failed++; }

    
    printf("\n[TESTING] is_valid_name():\n");
    if (is_valid_name("John Doe")) { printf("  PASS: Valid name\n"); passed++; } else { printf("  FAIL: Valid name\n"); failed++; }
    if (!is_valid_name("John123")) { printf("  PASS: Invalid name with numbers\n"); passed++; } else { printf("  FAIL: Invalid name with numbers\n"); failed++; }
    if (!is_valid_name("")) { printf("  PASS: Invalid empty name\n"); passed++; } else { printf("  FAIL: Invalid empty name\n"); failed++; }

   
    printf("\n[TESTING] format_name_capitalization():\n");
    strcpy(testStr, "john doe");
    format_name_capitalization(testStr);
    if (strcmp(testStr, "John Doe") == 0) { printf("  PASS: 'john doe' -> 'John Doe'\n"); passed++; } else { printf("  FAIL: 'john doe' -> 'John Doe'\n"); failed++; }
    
    strcpy(testStr, "MARY ANN");
    format_name_capitalization(testStr);
    if (strcmp(testStr, "Mary Ann") == 0) { printf("  PASS: 'MARY ANN' -> 'Mary Ann'\n"); passed++; } else { printf("  FAIL: 'MARY ANN' -> 'Mary Ann'\n"); failed++; }

    printf("\n--- Unit Test Summary ---\n");
    printf("Total Tests: %d\n", passed + failed);
    printf("Passed: %d\n", passed);
    printf("Failed: %d\n", failed);
    printf("-------------------------\n");

    return failed;
}
