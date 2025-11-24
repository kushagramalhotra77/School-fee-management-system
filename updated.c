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

// Function prototypes
void adminLogin();
void studentLogin();
void addStudent();
void displayAllStudents();
void searchStudent();
void updateFee();
void deleteStudent();
void saveToFile();
void loadFromFile();
void clearInputBuffer();
int validateInteger(const char *prompt, int min, int max);
float validateFloat(const char *prompt, float min, float max);
float getTotalFeeByGrade(int grade);
void displayStudentDetails(int index);

// Clear input buffer
void clearInputBuffer() {
    while (getchar() != '\n');
}

// Validate integer input
int validateInteger(const char *prompt, int min, int max) {
    int value;
    int valid;

    while (1) {
        printf("%s", prompt);
        valid = scanf("%d", &value);

        if (valid != 1) {
            printf("Invalid input! Please enter a number.\n");
            clearInputBuffer();
            continue;
        }

        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            clearInputBuffer();
            continue;
        }

        clearInputBuffer();
        return value;
    }
}

// Validate float input
float validateFloat(const char *prompt, float min, float max) {
    float value;
    int valid;

    while (1) {
        printf("%s", prompt);
        valid = scanf("%f", &value);

        if (valid != 1) {
            printf("Invalid input! Please enter a number.\n");
            clearInputBuffer();
            continue;
        }

        if (value < min || value > max) {
            printf("Value must be between %.2f and %.2f.\n", min, max);
            clearInputBuffer();
            continue;
        }

        clearInputBuffer();
        return value;
    }
}

// Get total fee based on grade
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

// Save all students to file
void saveToFile() {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
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

// Load all students from file
void loadFromFile() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("File '%s' not found. Starting with empty database.\n", FILENAME);
        return; // File doesn't exist yet
    }

    count = 0;
    char line[256];

    while (count < MAX && fgets(line, sizeof(line), file) != NULL) {
        // Parse the CSV line
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
    printf("Loaded %d student(s) from file.\n", count);
}

// Display student details
void displayStudentDetails(int index) {
    printf("\n**************************************************\n");
    printf("*            STUDENT DETAILS                     *\n");
    printf("**************************************************\n");
    printf("* Name       : %-35s *\n", students[index].name);
    printf("* Serial No  : %-35d *\n", students[index].serial);
    printf("* Class      : %-35d *\n", students[index].grade);
    printf("* Total Fee  : Rs. %-30.2f *\n", students[index].total_fee);
    printf("* Fee Paid   : Rs. %-30.2f *\n", students[index].fee_paid);
    printf("* Balance    : Rs. %-30.2f *\n", students[index].balance);
    printf("**************************************************\n");
}

// Add new student
void addStudent() {
    if (count >= MAX) {
        printf("Student database is full!\n");
        return;
    }

    printf("\n***************************************\n");
    printf("          ADD NEW STUDENT\n");
    printf("***************************************\n");

    Student newStudent;

    // Get serial number
    newStudent.serial = validateInteger("Enter Serial Number: ", 1, 99999);

    // Check if serial number already exists
    for (int i = 0; i < count; i++) {
        if (students[i].serial == newStudent.serial) {
            printf("Student with serial number %d already exists!\n", newStudent.serial);
            return;
        }
    }

    // Get name
    printf("Enter Name: ");
    fgets(newStudent.name, sizeof(newStudent.name), stdin);
    newStudent.name[strcspn(newStudent.name, "\n")] = '\0'; // Remove newline

    // Validate name is not empty
    if (strlen(newStudent.name) == 0) {
        printf("Name cannot be empty!\n");
        return;
    }

    // Get grade
    newStudent.grade = validateInteger("Enter Grade (1-12): ", 1, 12);

    // Set total fee based on grade
    newStudent.total_fee = getTotalFeeByGrade(newStudent.grade);

    // Get fee paid
    newStudent.fee_paid = validateFloat("Enter Fee Paid: Rs. ", 0.0, newStudent.total_fee);

    // Calculate balance
    newStudent.balance = newStudent.total_fee - newStudent.fee_paid;

    // Add student to array
    students[count++] = newStudent;

    // Save to file
    saveToFile();

    printf("Student added successfully!\n");
    printf("   Total Fee: Rs. %.2f\n", newStudent.total_fee);
    printf("   Balance: Rs. %.2f\n", newStudent.balance);
}

// Display all students
void displayAllStudents() {
    if (count == 0) {
        printf("No students in the system.\n");
        return;
    }

    printf("\n*************************************************************************************\n");
    printf("                                  ALL STUDENTS\n");
    printf("*************************************************************************************\n");
    printf("%-8s %-25s %-8s %-12s %-12s %-12s\n",
           "Serial", "Name", "Grade", "Total Fee", "Fee Paid", "Balance");
    printf("-------------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("%-8d %-25s %-8d Rs. %-8.2f Rs. %-8.2f Rs. %-8.2f\n",
               students[i].serial,
               students[i].name,
               students[i].grade,
               students[i].total_fee,
               students[i].fee_paid,
               students[i].balance);
    }

    printf("*************************************************************************************\n");
    printf("Total Students: %d\n", count);
}

// Search student by serial number
void searchStudent() {
    if (count == 0) {
        printf("No students in the system.\n");
        return;
    }

    int serial = validateInteger("\nEnter Serial Number to search: ", 1, 99999);

    for (int i = 0; i < count; i++) {
        if (students[i].serial == serial) {
            displayStudentDetails(i);
            return;
        }
    }

    printf("Student with serial number %d not found!\n", serial);
}

// Update fee payment
void updateFee() {
    if (count == 0) {
        printf("No students in the system.\n");
        return;
    }

    int serial = validateInteger("\nEnter Serial Number: ", 1, 99999);

    for (int i = 0; i < count; i++) {
        if (students[i].serial == serial) {
            printf("\nStudent: %s\n", students[i].name);
            printf("Current Balance: Rs. %.2f\n", students[i].balance);

            if (students[i].balance <= 0) {
                printf("No pending fees for this student!\n");
                return;
            }

            float payment = validateFloat("Enter Payment Amount: Rs. ", 0.01, students[i].balance);

            students[i].fee_paid += payment;
            students[i].balance -= payment;

            saveToFile();

            printf("Payment updated successfully!\n");
            printf("   Fee Paid: Rs. %.2f\n", students[i].fee_paid);
            printf("   Remaining Balance: Rs. %.2f\n", students[i].balance);
            return;
        }
    }

    printf("Student with serial number %d not found!\n", serial);
}

// Delete student record
void deleteStudent() {
    if (count == 0) {
        printf("No students in the system.\n");
        return;
    }

    int serial = validateInteger("\nEnter Serial Number to delete: ", 1, 99999);

    for (int i = 0; i < count; i++) {
        if (students[i].serial == serial) {
            printf("\nStudent found:\n");
            printf("Name: %s\n", students[i].name);
            printf("Class: %d\n", students[i].grade);
            printf("Balance: Rs. %.2f\n", students[i].balance);

            printf("\nAre you sure you want to delete this record? (Y/N): ");
            char confirm;
            scanf(" %c", &confirm);
            clearInputBuffer();

            if (confirm == 'Y' || confirm == 'y') {
                // Shift all students after this one to the left
                for (int j = i; j < count - 1; j++) {
                    students[j] = students[j + 1];
                }
                count--;

                saveToFile();
                printf("Student record deleted successfully!\n");
            } else {
                printf("Deletion cancelled.\n");
            }
            return;
        }
    }

    printf("Student with serial number %d not found!\n", serial);
}

// Admin login and menu
void adminLogin() {
    char username[50], password[50];

    printf("\n****************************************\n");
    printf("*         ADMIN LOGIN                  *\n");
    printf("****************************************\n");

    printf("Username: ");
    scanf("%49s", username);
    clearInputBuffer();

    printf("Password: ");
    scanf("%49s", password);
    clearInputBuffer();

    if (strcmp(username, "admin") != 0 || strcmp(password, "1234") != 0) {
        printf("Invalid username or password!\n");
        return;
    }

    printf("Login successful!\n");

    int choice;
    while (1) {
        printf("\n****************************************\n");
        printf("*         ADMIN MENU                   *\n");
        printf("****************************************\n");
        printf("* 1. Add Student                       *\n");
        printf("* 2. View All Students                 *\n");
        printf("* 3. Search Student                    *\n");
        printf("* 4. Update Fee                        *\n");
        printf("* 5. Delete Student                    *\n");
        printf("* 6. Logout                            *\n");
        printf("****************************************\n");

        choice = validateInteger("Enter your choice: ", 1, 6);

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                displayAllStudents();
                break;
            case 3:
                searchStudent();
                break;
            case 4:
                updateFee();
                break;
            case 5:
                deleteStudent();
                break;
            case 6:
                printf("Logged out successfully!\n");
                return;
        }
    }
}

// Student login
void studentLogin() {
    if (count == 0) {
        printf("No students in the system.\n");
        return;
    }

    printf("\n****************************************\n");
    printf("*         STUDENT LOGIN                *\n");
    printf("****************************************\n");

    int serial = validateInteger("Enter Serial Number: ", 1, 99999);
    int grade = validateInteger("Enter Class: ", 1, 12);

    for (int i = 0; i < count; i++) {
        if (students[i].serial == serial && students[i].grade == grade) {
            printf("Login successful!\n");

            // Auto-recalculate fee if grade's fee structure changed
            float currentTotalFee = getTotalFeeByGrade(students[i].grade);
            if (students[i].total_fee != currentTotalFee) {
                printf("Fee structure updated!\n");
                float oldTotalFee = students[i].total_fee;
                students[i].total_fee = currentTotalFee;
                students[i].balance = students[i].total_fee - students[i].fee_paid;

                // If new total fee is less than paid, adjust
                if (students[i].balance < 0) {
                    students[i].balance = 0;
                }

                saveToFile();
            }

            displayStudentDetails(i);
            return;
        }
    }

    printf("Invalid serial number or class!\n");
}

// Main function
int main() {
    loadFromFile();

    int choice;

    while (1) {
        printf("\n**************************************************\n");
        printf("*     SCHOOL FEE MANAGEMENT SYSTEM               *\n");
        printf("**************************************************\n");
        printf("* 1. Admin Login                                 *\n");
        printf("* 2. Student Login                               *\n");
        printf("* 3. Exit                                        *\n");
        printf("**************************************************\n");

        choice = validateInteger("Enter your choice: ", 1, 3);

        switch (choice) {
            case 1:
                adminLogin();
                break;
            case 2:
                studentLogin();
                break;
            case 3:
                printf("\n EXITING \n");
                return 0;
        }
    }

    return 0;
}
