#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define MAX_RECORDS 100
#define MAX_LINE_LENGTH 256

typedef struct {
    char employeeName[100];
    char resourceType[50];
    int resourceAmount;
    char usageDate[20]; // YYYY-MM-DD
} ResourceUsage;

extern ResourceUsage records[MAX_RECORDS];
extern int recordCount;
extern ResourceUsage trashRecords[MAX_RECORDS];
extern int trashCount;
extern const char* FILENAME;

int loadFromFile();
int saveToFile();
int addResourceUsage();
int searchResourceUsage();
int updateResourceUsage();
int deleteResourceUsage();
int restoreFromTrash();
int printAllRecords();
int displayMenu();
void clearInputBuffer();
char* toLower(char* s);
int is_valid_name(const char* name);
int is_valid_amount(const char* amount);
int is_valid_date(const char* date);
void format_name_capitalization(char* name);
int loadTrashFromFile(); 
int saveTrashToFile();   
int runUnitTests();
int runE2ETest();

#endif
