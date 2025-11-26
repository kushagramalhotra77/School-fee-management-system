#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define FILENAME "fees.txt"
#define ADMINDATA "datastored.txt"

void clearInputBuffer();
void addStudent();
void displayAllStudents();
void searchStudent();
void updateFee();
void deleteStudent();
void studentLogin();
void adminLogin();
int admindata(char username[], char password[]);
void adminMenu();
int getFixedFeeForClass(int class);


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

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int admindata(char username[], char password[]) {
    FILE *fp = fopen(ADMINDATA, "r");
    if (fp == NULL) {
        printf("Admin data file not found!\n");
        return 0;
    }

    char user[50], pass[50];
    char line[100];

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (sscanf(line, "%[^,],%s", user, pass) == 2) {
            if (strcmp(username, user) == 0 && strcmp(password, pass) == 0) {
                fclose(fp);
                return 1;
            }
        }
    }

    fclose(fp);
    return 0;
}

float gradeFee(int grade) {
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
    char line[200];

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

void studentInfo(int index) {
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
    for (i = 0; i < count; i++) {
        if (students[i].serial == newStudent.serial) {
            printf("Serial number already exists!\n");
            return;
        }
    }

    printf("Enter Name: ");
    fgets(newStudent.name, sizeof(newStudent.name), stdin);
    newStudent.name[strcspn(newStudent.name, "\n")] = '\0';

    if (strlen(newStudent.name) == 0) {
        printf("Name cannot be empty!\n");
        return;
    }

    printf("Enter Grade (1-12): ");
    scanf("%d", &newStudent.grade);
    clearInputBuffer();

    newStudent.total_fee = gradeFee(newStudent.grade);

    printf("Enter Fee Paid: Rs. ");
    scanf("%f", &newStudent.fee_paid);
    clearInputBuffer();

    newStudent.balance = newStudent.total_fee - newStudent.fee_paid;

    students[count] = newStudent;
    count++;

    saveToFile();

    printf("\nStudent added!\n");
    printf("Total Fee: Rs. %.2f\n", newStudent.total_fee);
    printf("Balance: Rs. %.2f\n", newStudent.balance);
}

void displayAllStudents() {
    if (count == 0) {
        printf("No students found.\n");
        return;
    }

    printf("\n========================================================================\n");
    printf("                            ALL STUDENTS\n");
    printf("========================================================================\n");
    printf("Serial   Name                      Grade   Total Fee   Paid      Balance\n");
    printf("------------------------------------------------------------------------\n");

    int i;
    for (i = 0; i < count; i++) {
        printf("%8d %25s %7d %11.2f %9.2f %.2f\n",
               students[i].serial,
               students[i].name,
               students[i].grade,
               students[i].total_fee,
               students[i].fee_paid,
               students[i].balance);
    }

    printf("========================================================================\n");
    printf("Total: %d students\n", count);
}

void searchStudent() {
    if (count == 0) {
        printf("No students in system.\n");
        return;
    }

    int serial;
    printf("\nEnter Serial Number: ");
    scanf("%d", &serial);
    clearInputBuffer();

    int i;
    for (i = 0; i < count; i++) {
        if (students[i].serial == serial) {
            studentInfo(i);
            return;
        }
    }

    printf("Student not found!\n");
}

void updateFee() {
    if (count == 0) {
        printf("No students in system.\n");
        return;
    }

    int serial;
    printf("\nEnter Serial Number: ");
    scanf("%d", &serial);
    clearInputBuffer();

    int i;
    for (i = 0; i < count; i++) {
        if (students[i].serial == serial) {
            printf("\nStudent: %s\n", students[i].name);
            printf("Current Balance: Rs. %.2f\n", students[i].balance);

            if (students[i].balance <= 0) {
                printf("No pending fees!\n");
                return;
            }

            float payment;
            printf("Enter Payment: Rs. ");
            scanf("%f", &payment);
            clearInputBuffer();

            students[i].fee_paid = students[i].fee_paid + payment;
            students[i].balance = students[i].balance - payment;

            saveToFile();

            printf("\nPayment updated!\n");
            printf("Fee Paid: Rs. %.2f\n", students[i].fee_paid);
            printf("Balance: Rs. %.2f\n", students[i].balance);
            return;
        }
    }

    printf("Student not found!\n");
}

void deleteStudent() {
    if (count == 0) {
        printf("No students in system.\n");
        return;
    }

    int serial;
    printf("\nEnter Serial Number: ");
    scanf("%d", &serial);
    clearInputBuffer();

    int i, j;
    for (i = 0; i < count; i++) {
        if (students[i].serial == serial) {
            printf("\nStudent: %s\n", students[i].name);
            printf("Class: %d\n", students[i].grade);
            printf("Balance: Rs. %.2f\n", students[i].balance);

            printf("\nDelete this record? (Y/N): ");
            char confirm;
            scanf(" %c", &confirm);
            clearInputBuffer();

            if (confirm == 'Y' || confirm == 'y') {
                for (j = i; j < count - 1; j++) {
                    students[j] = students[j + 1];
                }
                count--;

                saveToFile();
                printf("Record deleted!\n");
            } else {
                printf("Cancelled.\n");
            }
            return;
        }
    }

    printf("Student not found!\n");
}

void adminLogin() {
    char username[50];
    char password[50];

    printf("\n--- Admin Login ---\n");

    printf("Username: ");
    scanf("%49s", username);
    clearInputBuffer();

    printf("Password: ");
    scanf("%49s", password);
    clearInputBuffer();

    if (!admindata(username, password)) {
        printf("Wrong username or password!\n");
        return;
    }

    printf("Login successful!\n");

    int choice;
    while (1) {
        printf("\n========================================\n");
        printf("            ADMIN MENU\n");
        printf("========================================\n");
        printf("1. Add Student\n");
        printf("2. View All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Fee\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");
        printf("========================================\n");

        printf("Choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) { 
        case 1: addStudent(); break; 
        case 2: displayAllStudents(); break; 
        case 3: searchStudent(); break; 
        case 4: updateFee(); break; 
        case 5: deleteStudent(); break; 
        case 6: printf("Logged out.\n"); return; 
        default: printf("Invalid choice!\n"); 
        } 
    }
}

void studentLogin() {
    if (count == 0) {
        printf("No students in system.\n");
        return;
    }

    printf("\n--- Student Login ---\n");

    int serial;
    printf("Serial Number: ");
    scanf("%d", &serial);
    clearInputBuffer();

    int i;
    for (i = 0; i < count; i++) {
        if (students[i].serial == serial) {
            printf("Login successful!\n");

            float currentTotalFee = gradeFee(students[i].grade);
            if (students[i].total_fee != currentTotalFee) {
                printf("Fee updated!\n");
                students[i].total_fee = currentTotalFee;
                students[i].balance = students[i].total_fee - students[i].fee_paid;

                if (students[i].balance < 0) {
                    students[i].balance = 0;
                }
                saveToFile();
            }
            studentInfo(i);
            return;
        }
    }

    printf("Student not found!\n");
}

int main() {
    loadFromFile();

    int choice;

    while (1) {
        printf("\n========================================\n");
        printf("   SCHOOL FEE MANAGEMENT SYSTEM\n");
        printf("========================================\n");
        printf("1. Admin Login\n");
        printf("2. Student Login\n");
        printf("3. Exit\n");
        printf("========================================\n");

        printf("Enter choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        if (choice == 1) {
            adminLogin();
        } else if (choice == 2) {
            studentLogin();
        } else if (choice == 3) {
            printf("\nExiting...\n");
        }else
            printf("Invalid \n");
                    return 0;`

        
    }

    return 0;
}
