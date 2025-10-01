#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const int MAX_RECORDS    = 100;
const int MAX_NAME_LEN   = 50;
const int MAX_TYPE_LEN   = 50;
const int MAX_AMOUNT_LEN = 50;
const int MAX_DATE_LEN   = 11;
const int MAX_LINE_LEN   = 256;

typedef struct {
    char EmployeeName[100];
    char ResourceType[100];
    int ResourceAmount;
    char UsageDate[20];
} EmployeeResource;

int main() {
    ResourceRecord data[MAX_RECORDS];
    int record_count = 0;
    int choice;
    void display_menu() {
    printf("\n=========================\n");
    printf("     Resource Manager    \n");
    printf("=========================\n");
    printf("1. Add record\n");
    printf("---------------------------\n");
    printf("2. Search record\n");
    printf("---------------------------\n");
    printf("3. Update record\n");
    printf("---------------------------\n");
    printf("4. Delete record\n");
    printf("---------------------------\n");
    printf("5. Show all records\n");
    printf("---------------------------\n");
    printf("6. Exit\n");
    printf("=============================\n");
    printf("Enter your choice: ");
 }
