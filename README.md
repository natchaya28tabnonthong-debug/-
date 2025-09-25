#include <stdio.h>
typedef struct {
    char EmployeeName[100];
    char ResourceType[100];
    int ResourceAmount;
    char UsageDate[20];
} EmployeeResource;

void addRecord(const char *filename, EmployeeResource data) {
    FILE *file = fopen(filename, "a");  // "a" = append mode
    if (!file) {
        printf("ไม่สามารถเปิดไฟล์ CSV เพื่อเขียนได้\n");
        return;
    }

    fprintf(file, "%s,%s,%d,%s\n", 
            data.EmployeeName, 
            data.ResourceType, 
            data.ResourceAmount, 
            data.UsageDate);

    fclose(file);
    printf("เพิ่มข้อมูลเรียบร้อยแล้ว!\n");
}
