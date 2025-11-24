#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100
#define FILENAME "fees.txt"

typedef struct {
    int serial;
    char name[50];
    int grade;
    float total_fee;
    float fee_paid;
    float balance;
} Student;

Student students[MAX];
int count = 0;

// Function declarations
void clearInputBuffer();
float getTotalFeeByGrade(int grade);
void saveToFile();
void loadFromFile();
void displayStudentDetails(int index);
void addStudent();
void displayAllStudents();
void searchStudent();
void updateFee();
void deleteStudent();
void adminLogin();
void studentLogin();

// Function definitions

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

float getTotalFeeByGrade(int grade) {
    if (grade >= 1 && grade <= 4) {
        return 40000.0;
    } else if (grade >= 5 && grade <= 8) {
        return 50000.0;
    } else if (grade >= 9 && grade <= 12) {
        return 60000.0;
    }
    return 0.0;
}

void saveToFile() {
    FILE *file;
    file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    int i;
    for (i = 0; i < count; i++) {
        fprintf(file, "%d,%s,%d,%.2f,%.2f,%.2f\n",
                students[i].serial,
                students[i].name,
                students[i].grade,
                students[i].total_fee,
                students[i].fee_paid,
                students[i].balance);
    }

    fclose(file);
}

void loadFromFile() {
    FILE *file;
    file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("No existing file found. Starting fresh.\n");
        return;
    }

    count = 0;
    char line[256];

    while (fgets(line, sizeof(line), file) != NULL && count < MAX) {
        int result = sscanf(line, "%d,%49[^,],%d,%f,%f,%f",
                           &students[count].serial,
                           students[count].name,
                           &students[count].grade,
                           &students[count].total_fee,
                           &students[count].fee_paid,
                           &students[count].balance);

        if (result == 6) {
            count++;
        }
    }

    fclose(file);
    printf("Loaded %d students.\n", count);
}

void displayStudentDetails(int index) {
    printf("\n==============================================\n");
    printf("           STUDENT DETAILS                    \n");
    printf("==============================================\n");
    printf("Name       : %s\n", students[index].name);
    printf("Serial No  : %d\n", students[index].serial);
    printf("Class      : %d\n", students[index].grade);
    printf("Total Fee  : Rs. %.2f\n", students[index].total_fee);
    printf("Fee Paid   : Rs. %.2f\n", students[index].fee_paid);
    printf("Balance    : Rs. %.2f\n", students[index].balance);
    printf("==============================================\n");
}

void addStudent() {
    if (count >= MAX) {
        printf("Cannot add more students. Database full!\n");
        return;
    }

    printf("\n--- Add New Student ---\n");

    Student newStudent;
    int i;

    printf("Enter Serial Number: ");
    scanf("%d", &newStudent.serial);
    clearInputBuffer();

    // check duplicate
    for (i = 0; i < count; i++) {
        if (students[i].serial == newStudent.serial) {
            printf("Serial number already exists!\n");
            return;
        }
    }

    printf("Enter Name: ");
    fgets(newStudent.name, sizeof(newStudent.name), stdin);
    newStudent.name[strcspn(newStudent.na
