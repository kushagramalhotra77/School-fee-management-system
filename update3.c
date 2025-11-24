#include <stdio.h>
#include <string.h>

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

// Function prototypes
void loadFromFile();
void saveToFile();
float getTotalFee(int grade);
void addStudent();
void displayAll();
void searchStudent();
void updateFee();
void deleteStudent();
void studentLogin();
void adminLogin();

// Get fee based on grade
float getTotalFee(int grade) {
    if (grade >= 1 && grade <= 4) return 40000;
    if (grade >= 5 && grade <= 8) return 50000;
    if (grade >= 9 && grade <= 12) return 60000;
    return 0;
}

// Load students from file
void loadFromFile() {
    FILE *fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("No saved records found. Starting fresh.\n");
        return;
    }

    count = 0;
    while (fscanf(fp, "%d,%49[^,],%d,%f,%f,%f\n",
                  &students[count].serial,
                  students[count].name,
                  &students[count].grade,
                  &students[count].total_fee,
                  &students[count].fee_paid,
                  &students[count].balance) == 6) {
        count++;
    }

    fclose(fp);
}

// Save students to file
void saveToFile() {
    FILE *fp = fopen(FILENAME, "w");
    if (fp == NULL) {
        printf("Cannot save file!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d,%s,%d,%.2f,%.2f,%.2f\n",
                students[i].serial,
                students[i].name,
                students[i].grade,
                students[i].total_fee,
                students[i].fee_paid,
                students[i].balance);
    }

    fclose(fp);
}

// Add student
void addStudent() {
    if (count >= MAX) {
        printf("Database full!\n");
        return;
    }

    Student s;

    printf("\nEnter Serial Number: ");
    scanf("%d", &s.serial);
    getchar();

    printf("Enter Name: ");
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = '\0';

    printf("Enter Grade (1-12): ");
    scanf("%d", &s.grade);

    s.total_fee = getTotalFee(s.grade);

    printf("Enter Fee Paid: ");
    scanf("%f", &s.fee_paid);

    s.balance = s.total_fee - s.fee_paid;

    students[count++] = s;

    saveToFile();
    printf("Student added successfully.\n");
}

// Display all students
void displayAll() {
    if (count == 0) {
        printf("No students found.\n");
        return;
    }

    printf("\n----------------------------------------------\n");
    printf("                 ALL STUDENTS\n");
    printf("----------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("%d. %s | Class %d | Total: %.2f | Paid: %.2f | Balance: %.2f\n",
               students[i].serial,
               students[i].name,
               students[i].grade,
               students[i].total_fee,
               students[i].fee_paid,
               students[i].balance);
    }
}

// Search student
void searchStudent() {
    int serial;
    printf("\nEnter Serial Number to search: ");
    scanf("%d", &serial);

    for (int i = 0; i < count; i++) {
        if (students[i].serial == serial) {
            printf("\n--- Student Found ---\n");
            printf("Name: %s\n", students[i].name);
            printf("Class: %d\n", students[i].grade);
            printf("Total Fee: %.2f\n", students[i].total_fee);
            printf("Paid: %.2f\n", students[i].fee_paid);
            printf("Balance: %.2f\n", students[i].balance);
            return;
        }
    }

    printf("Student not found.\n");
}

// Update fee
void updateFee() {
    int serial;
    printf("\nEnter Serial Number: ");
    scanf("%d", &serial);

    for (int i = 0; i < count; i++) {
        if (students[i].serial == serial) {

            printf("Enter Additional Fee Paid: ");
            float extra;
            scanf("%f", &extra);

            students[i].fee_paid += extra;
            students[i].balance = students[i].total_fee - students[i].fee_paid;

            saveToFile();
            printf("Fee updated successfully.\n");
            return;
        }
    }
    printf("Student not found.\n");
}

// Delete student
void deleteStudent() {
    int serial;
    printf("\nEnter Serial Number to delete: ");
    scanf("%d", &serial);

    for (int i = 0; i < count; i++) {
        if (students[i].serial == serial) {

            for (int j = i; j < count - 1; j++)
                students[j] = students[j + 1];

            count--;
            saveToFile();
            printf("Record deleted.\n");
            return;
        }
    }

    printf("Student not found.\n");
}

// Student Login
void studentLogin() {
    int serial, grade;

    printf("\nSerial Number: ");
    scanf("%d", &serial);

    printf("Class: ");
    scanf("%d", &grade);

    for (int i = 0; i < count; i++) {
        if (students[i].serial == serial && students[i].grade == grade) {

            printf("\nLogin Successful.\n");
            printf("Name: %s\n", students[i].name);
            printf("Total Fee: %.2f\n", students[i].total_fee);
            printf("Paid: %.2f\n", students[i].fee_paid);
            printf("Balance: %.2f\n", students[i].balance);
            return;
        }
    }

    printf("Invalid login details.\n");
}

// Admin Menu
void adminLogin() {
    char user[20], pass[20];

    printf("\nUsername: ");
    scanf("%s", user);

    printf("Password: ");
    scanf("%s", pass);

    if (strcmp(user, "admin") != 0 || strcmp(pass, "1234") != 0) {
        printf("Invalid login!\n");
        return;
    }

    int choice;
    while (1) {
        printf("\n--- ADMIN MENU ---\n");
        printf("1. Add Student\n");
        printf("2. View All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Fee\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayAll(); break;
            case 3: searchStudent(); break;
            case 4: updateFee(); break;
            case 5: deleteStudent(); break;
            case 6: return;
        }
    }
}

int main() {
    loadFromFile();

    int choice;

    while (1) {
        printf("\n===== SCHOOL FEE SYSTEM =====\n");
        printf("1. Admin Login\n");
        printf("2. Student Login\n");
        printf("3. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: adminLogin(); break;
            case 2: studentLogin(); break;
            case 3: printf("Exiting...\n"); return 0;
        }
    }

    return 0;
}
