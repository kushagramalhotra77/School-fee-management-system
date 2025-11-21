#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100   // maximum students

//--------------------------------------------
// Student Structure
//--------------------------------------------
typedef struct {
    int roll;
    char name[50];
    int grade;             // renamed from 'class' to 'grade'
    float total_fee;
    float fee_paid;
    float balance;
} Student;

Student students[MAX];
int count = 0;

//--------------------------------------------
// Function Declarations
//--------------------------------------------
void addStudent();
void displayAll();
void searchStudent();
void updateFee();
void saveToFile();
void loadFromFile();
void adminLogin();
void studentLogin();
float getTotalFee(int grade);

//--------------------------------------------
// Main
//--------------------------------------------
int main() {
    int choice;
    loadFromFile();

    while (1) {
        printf("\n=== School Fee Management System ===\n");
        printf("1. Admin Login\n");
        printf("2. Student Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("❌ Invalid input! Enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1: adminLogin(); break;
            case 2: studentLogin(); break;
            case 3:
                printf("\nSaving records and exiting...\n");
                saveToFile();
                exit(0);

            default:
                printf("❌ Invalid choice! Try again.\n");
        }
    }
}

//--------------------------------------------
// Student Login
//--------------------------------------------
void studentLogin() {
    if (count == 0) {
        printf("No records available!\n");
        return;
    }

    int r, grade, found = 0;

    printf("\n=== Student Login ===\n");

    while (1) {
        printf("Enter Roll No: ");
        if (scanf("%d", &r) != 1) {
            printf("❌ Invalid number! Try again.\n");
            while (getchar() != '\n');
            continue;
        }

        printf("Enter Class / Grade (1–12): ");
        if (scanf("%d", &grade) != 1 || grade < 1 || grade > 12) {
            printf("❌ Invalid class! Try again.\n");
            while (getchar() != '\n');
            continue;
        }

        for (int i = 0; i < count; i++) {
            if (students[i].roll == r && students[i].grade == grade) {

                float correctFee = getTotalFee(grade);
                if (students[i].total_fee != correctFee) {
                    students[i].total_fee = correctFee;
                    students[i].balance = correctFee - students[i].fee_paid;
                }

                printf("\n--- Student Fee Details ---\n");
                printf("Roll No   : %d\n", students[i].roll);
                printf("Name      : %s\n", students[i].name);
                printf("Class     : %d\n", students[i].grade);
                printf("Total Fee : ₹%.2f\n", students[i].total_fee);
                printf("Fee Paid  : ₹%.2f\n", students[i].fee_paid);
                printf("Balance   : ₹%.2f\n", students[i].balance);

                if (students[i].balance <= 0)
                    printf("\n✅ All fees cleared.\n");
                else
                    printf("\n⚠ Pending dues: ₹%.2f\n", students[i].balance);

                found = 1;
                break;
            }
        }

        if (found) break;
        else printf("❌ Invalid Roll No or Class! Try again.\n");
    }
}

//--------------------------------------------
// Admin Login
//--------------------------------------------
void adminLogin() {
    char username[20], password[20];

    printf("\n=== Admin Login ===\n");
    printf("Enter Username: ");
    if (scanf("%19s", username) != 1) { while (getchar() != '\n'); return; }
    printf("Enter Password: ");
    if (scanf("%19s", password) != 1) { while (getchar() != '\n'); return; }

    if (strcmp(username, "admin") == 0 && strcmp(password, "1234") == 0) {

        int choice;
        while (1) {
            printf("\n=== Admin Panel ===\n");
            printf("1. Add Student\n");
            printf("2. View All Students\n");
            printf("3. Search Student\n");
            printf("4. Update Fee\n");
            printf("5. Logout\n");
            printf("Enter your choice: ");

            if (scanf("%d", &choice) != 1) {
                printf("❌ Invalid input! Enter a number.\n");
                while (getchar() != '\n');
                continue;
            }

            switch (choice) {
                case 1: addStudent(); break;
                case 2: displayAll(); break;
                case 3: searchStudent(); break;
                case 4: updateFee(); break;
                case 5: return;
                default: printf("❌ Invalid choice! Try again.\n");
            }
        }

    } else {
        printf("❌ Invalid credentials!\n");
    }
}

//--------------------------------------------
// Add Student
//--------------------------------------------
void addStudent() {
    if (count >= MAX) {
        printf("Database full!\n");
        return;
    }

    Student s;

    printf("\n=== Add New Student ===\n");

    while (1) {
        printf("Enter Roll No: ");
        if (scanf("%d", &s.roll) != 1 || s.roll <= 0) {
            printf("❌ Invalid Roll No!\n");
            while (getchar() != '\n');
            continue;
        }
        break;
    }

    while (getchar() != '\n'); // flush newline before fgets
    printf("Enter Name: ");
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = '\0';

    while (1) {
        printf("Enter Class / Grade (1–12): ");
        if (scanf("%d", &s.grade) != 1 || s.grade < 1 || s.grade > 12) {
            printf("❌ Invalid class!\n");
            while (getchar() != '\n');
            continue;
        }
        break;
    }

    s.total_fee = getTotalFee(s.grade);
    printf("Total Fee for Class %d: ₹%.2f\n", s.grade, s.total_fee);

    while (1) {
        printf("Enter Fee Paid: ");
        if (scanf("%f", &s.fee_paid) != 1) {
            printf("❌ Invalid amount!\n");
            while (getchar() != '\n');
            continue;
        }
        if (s.fee_paid < 0.0f) {
            printf("❌ Fee paid cannot be negative.\n");
            continue;
        }
        if (s.total_fee > 0 && s.fee_paid > s.total_fee) {
            printf("❌ Fee paid cannot exceed total fee (₹%.2f).\n", s.total_fee);
            continue;
        }
        break;
    }

    s.balance = s.total_fee - s.fee_paid;

    students[count++] = s;

    printf("\n✅ Student added successfully!\n");
    saveToFile();
}

//--------------------------------------------
// Display All Students
//--------------------------------------------
void displayAll() {
    if (count == 0) {
        printf("No records found!\n");
        return;
    }

    printf("\n%-5s %-20s %-7s %-10s %-10s %-10s\n",
           "Roll", "Name", "Class", "Total", "Paid", "Balance");

    for (int i = 0; i < count; i++) {
        printf("%-5d %-20s %-7d %-10.2f %-10.2f %-10.2f\n",
               students[i].roll,
               students[i].name,
               students[i].grade,
               students[i].total_fee,
               students[i].fee_paid,
               students[i].balance);
    }
}

//--------------------------------------------
// Search Student by Roll
//--------------------------------------------
void searchStudent() {
    if (count == 0) {
        printf("No records available!\n");
        return;
    }

    int r;
    printf("Enter Roll No: ");
    if (scanf("%d", &r) != 1) {
        printf("❌ Invalid number!\n");
        while (getchar() != '\n');
        return;
    }

    for (int i = 0; i < count; i++) {
        if (students[i].roll == r) {
            printf("\nRecord Found!\n");
            printf("Name: %s\n", students[i].name);
            printf("Class: %d\n", students[i].grade);
            printf("Total Fee: ₹%.2f\n", students[i].total_fee);
            printf("Paid: ₹%.2f\n", students[i].fee_paid);
            printf("Balance: ₹%.2f\n", students[i].balance);
            return;
        }
    }

    printf("❌ No record found for Roll No %d\n", r);
}

//--------------------------------------------
// Update Fee
//--------------------------------------------
void updateFee() {
    if (count == 0) {
        printf("No records available!\n");
        return;
    }

    int r;
    printf("Enter Roll No: ");
    if (scanf("%d", &r) != 1) {
        printf("❌ Invalid number!\n");
        while (getchar() != '\n');
        return;
    }

    for (int i = 0; i < count; i++) {
        if (students[i].roll == r) {
            float extra;

            while (1) {
                printf("Pending Balance: ₹%.2f\n", students[i].balance);
                printf("Enter additional payment: ");
                if (scanf("%f", &extra) != 1) {
                    printf("❌ Invalid amount!\n");
                    while (getchar() != '\n');
                    continue;
                }
                if (extra <= 0) {
                    printf("❌ Must enter a positive amount.\n");
                    continue;
                }
                if (students[i].balance >= 0 && extra > students[i].balance) {
                    printf("❌ Cannot accept ₹%.2f. Pending: ₹%.2f.\n", extra, students[i].balance);
                    continue;
                }

                students[i].fee_paid += extra;
                students[i].balance = students[i].total_fee - students[i].fee_paid;

                printf("\n✅ Record updated successfully!\n");
                printf("Total Fee : ₹%.2f\n", students[i].total_fee);
                printf("Fee Paid  : ₹%.2f\n", students[i].fee_paid);
                printf("Balance   : ₹%.2f\n", students[i].balance);
                saveToFile();
                return;
            }
        }
    }
    printf("❌ No record found for Roll No %d.\n", r);
}

//--------------------------------------------
// Save & Load
//--------------------------------------------
void saveToFile() {
    FILE *fp = fopen("fees.txt", "w");
    if (!fp) {
        perror("Error opening fees.txt for writing");
        return;
    }
    for (int i = 0; i < count; i++) {
        // Note: avoid commas inside name to keep CSV simple
        fprintf(fp, "%d,%s,%d,%.2f,%.2f,%.2f\n",
                students[i].roll, students[i].name, students[i].grade,
                students[i].total_fee, students[i].fee_paid, students[i].balance);
    }
    fclose(fp);
}

void loadFromFile() {
    FILE *fp = fopen("fees.txt", "r");
    if (!fp) {
        printf("fees.txt not found — starting fresh database.\n");
        return;
    }

    char line[256];
    count = 0;

    while (fgets(line, sizeof(line), fp) != NULL && count < MAX) {
        // Remove trailing newline
        line[strcspn(line, "\r\n")] = 0;

        if (strlen(line) < 3) continue;  // skip blank lines

        if (sscanf(line, "%d,%49[^,],%d,%f,%f,%f",
                   &students[count].roll,
                   students[count].name,
                   &students[count].grade,
                   &students[count].total_fee,
                   &students[count].fee_paid,
                   &students[count].balance) == 6)
        {
            count++;
        }
    }

    fclose(fp);

    printf("%d student records loaded successfully.\n", count);
}


//--------------------------------------------
// Fee by Class (grade)
//--------------------------------------------
float getTotalFee(int grade) {
    if (grade >= 1 && grade <= 4) return 40000.0f;
    if (grade >= 5 && grade <= 8) return 50000.0f;
    if (grade >= 9 && grade <= 12) return 60000.0f;
    return 0.0f;
}
