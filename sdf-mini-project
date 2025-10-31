#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100  // Maximum number of students

// Global arrays
int roll[MAX];
char name[MAX][50];
char class_name[MAX][20];
float total_fee[MAX];
float fee_paid[MAX];
float balance[MAX];

int count = 0; // student counter

// Function declarations
void addStudent();
void displayAll();
void searchStudent();
void updateFee();
void saveToFile();
void loadFromFile();
void studentLogin();
void adminLogin();
float getTotalFee(int cls);

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
            case 1:
                adminLogin();
                break;
            case 2:
                studentLogin();
                break;
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
// Student Login (stays until valid input)
//--------------------------------------------
void studentLogin() {
    if (count == 0) {
        printf("No records available!\n");
        return;
    }

    int r, cls, found = 0;

    printf("\n=== Student Login ===\n");

    while (1) {
        printf("Enter Roll No: ");
        if (scanf("%d", &r) != 1) {
            printf("❌ Invalid number! Try again.\n");
            while (getchar() != '\n');
            continue;
        }

        printf("Enter Class (1–12): ");
        if (scanf("%d", &cls) != 1 || cls < 1 || cls > 12) {
            printf("❌ Invalid class! Please enter between 1 and 12.\n");
            while (getchar() != '\n');
            continue;
        }

        for (int i = 0; i < count; i++) {
            if (roll[i] == r && atoi(class_name[i]) == cls) {
                float correctFee = getTotalFee(cls);
                if (total_fee[i] != correctFee) {
                    total_fee[i] = correctFee;
                    balance[i] = total_fee[i] - fee_paid[i];
                }

                printf("\n--- Student Fee Details ---\n");
                printf("Roll No   : %d\n", roll[i]);
                printf("Name      : %s\n", name[i]);
                printf("Class     : %s\n", class_name[i]);
                printf("Total Fee : ₹%.2f\n", total_fee[i]);
                printf("Fee Paid  : ₹%.2f\n", fee_paid[i]);
                printf("Balance   : ₹%.2f\n", balance[i]);

                if (balance[i] <= 0)
                    printf("\n✅ All fees cleared.\n");
                else
                    printf("\n⚠ Pending dues: ₹%.2f\n", balance[i]);
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
    scanf("%s", username);
    printf("Enter Password: ");
    scanf("%s", password);

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
                case 5: printf("Logging out...\n"); return;
                default: printf("❌ Invalid choice! Try again.\n");
            }
        }
    } else {
        printf("❌ Invalid credentials!\n");
    }
}

//--------------------------------------------
// Add Student (loops until valid input)
//--------------------------------------------
void addStudent() {
    if (count >= MAX) {
        printf("Database full! Cannot add more students.\n");
        return;
    }

    int tempRoll, tempClass;
    char tempName[50];
    float tempPaid;

    printf("\n=== Add New Student ===\n");

    while (1) {
        printf("Enter Roll No: ");
        if (scanf("%d", &tempRoll) != 1 || tempRoll <= 0) {
            printf("❌ Invalid Roll No! Try again.\n");
            while (getchar() != '\n');
            continue;
        }

        getchar(); // clear buffer
        printf("Enter Name: ");
        fgets(tempName, sizeof(tempName), stdin);
        tempName[strcspn(tempName, "\n")] = '\0';

        printf("Enter Class (1–12): ");
        if (scanf("%d", &tempClass) != 1 || tempClass < 1 || tempClass > 12) {
            printf("❌ Invalid class! Please enter between 1 and 12.\n");
            while (getchar() != '\n');
            continue;
        }

        int exists = 0;
        for (int i = 0; i < count; i++) {
            if (roll[i] == tempRoll && atoi(class_name[i]) == tempClass) {
                printf("⚠ Student already exists!\n");
                exists = 1;
                break;
            }
        }
        if (exists) continue;

        total_fee[count] = getTotalFee(tempClass);
        printf("Total Fee for class %d: ₹%.2f\n", tempClass, total_fee[count]);

        while (1) {
            printf("Enter Fee Paid: ");
            if (scanf("%f", &tempPaid) != 1 || tempPaid < 0 || tempPaid > total_fee[count]) {
                printf("❌ Invalid amount! Must be between 0 and total fee.\n");
                while (getchar() != '\n');
                continue;
            }
            break;
        }

        roll[count] = tempRoll;
        strcpy(name[count], tempName);
        sprintf(class_name[count], "%d", tempClass);
        fee_paid[count] = tempPaid;
        balance[count] = total_fee[count] - tempPaid;

        count++;
        printf("\n✅ Student added successfully!\n");
        saveToFile();
        break;
    }
}

//--------------------------------------------
// Display All
//--------------------------------------------
void displayAll() {
    if (count == 0) {
        printf("No records found!\n");
        return;
    }

    printf("\n%-5s %-20s %-10s %-10s %-10s %-10s\n",
           "Roll", "Name", "Class", "Total", "Paid", "Balance");

    for (int i = 0; i < count; i++) {
        printf("%-5d %-20s %-10s %-10.2f %-10.2f %-10.2f\n",
               roll[i], name[i], class_name[i],
               total_fee[i], fee_paid[i], balance[i]);
    }
}

//--------------------------------------------
// Search Student
//--------------------------------------------
void searchStudent() {
    if (count == 0) {
        printf("No records available!\n");
        return;
    }

    int r, found = 0;
    while (1) {
        printf("Enter Roll No to search: ");
        if (scanf("%d", &r) != 1) {
            printf("❌ Invalid number! Try again.\n");
            while (getchar() != '\n');
            continue;
        }

        for (int i = 0; i < count; i++) {
            if (roll[i] == r) {
                printf("\nRecord Found!\n");
                printf("Name: %s\nClass: %s\nTotal Fee: ₹%.2f\nPaid: ₹%.2f\nBalance: ₹%.2f\n",
                       name[i], class_name[i], total_fee[i], fee_paid[i], balance[i]);
                found = 1;
                break;
            }
        }

        if (found) break;
        else printf("❌ No record found for Roll No %d. Try again.\n", r);
    }
}

//--------------------------------------------
// Update Fee (keeps looping until valid input)
//--------------------------------------------
void updateFee() {
    if (count == 0) {
        printf("No records available!\n");
        return;
    }

    int r, found = 0;
    float extra;

    while (1) {
        printf("Enter Roll No to update: ");
        if (scanf("%d", &r) != 1) {
            printf("❌ Invalid number! Try again.\n");
            while (getchar() != '\n');
            continue;
        }

        for (int i = 0; i < count; i++) {
            if (roll[i] == r) {
                found = 1;
                while (1) {
                    printf("\nPending Balance: ₹%.2f\n", balance[i]);
                    printf("Enter additional amount paid: ");
                    if (scanf("%f", &extra) != 1) {
                        printf("❌ Invalid number! Try again.\n");
                        while (getchar() != '\n');
                        continue;
                    }
                    if (extra <= 0) {
                        printf("❌ Must enter a positive amount.\n");
                        continue;
                    }
                    if (extra > balance[i]) {
                        printf("❌ Cannot accept ₹%.2f. Pending: ₹%.2f.\n", extra, balance[i]);
                        continue;
                    }

                    fee_paid[i] += extra;
                    balance[i] = total_fee[i] - fee_paid[i];

                    printf("\n✅ Record updated successfully!\n");
                    printf("Total Fee : ₹%.2f\n", total_fee[i]);
                    printf("Fee Paid  : ₹%.2f\n", fee_paid[i]);
                    printf("Balance   : ₹%.2f\n", balance[i]);
                    saveToFile();
                    return;
                }
            }
        }
        if (!found)
            printf("❌ No record found for Roll No %d. Try again.\n", r);
    }
}

//--------------------------------------------
// Save & Load
//--------------------------------------------
void saveToFile() {
    FILE *fp = fopen("fees.txt", "w");
    if (!fp) return;
    for (int i = 0; i < count; i++)
        fprintf(fp, "%d,%s,%s,%.2f,%.2f,%.2f\n",
                roll[i], name[i], class_name[i],
                total_fee[i], fee_paid[i], balance[i]);
    fclose(fp);
}

void loadFromFile() {
    FILE *fp = fopen("fees.txt", "r");
    if (!fp) return;
    while (count < MAX &&
           fscanf(fp, "%d,%49[^,],%19[^,],%f,%f,%f\n",
                  &roll[count], name[count], class_name[count],
                  &total_fee[count], &fee_paid[count], &balance[count]) == 6)
        count++;
    fclose(fp);
}

//--------------------------------------------
// Get Total Fee by Class
//--------------------------------------------
float getTotalFee(int cls) {
    switch (cls) {
        case 1: case 2: case 3: case 4: return 40000;
        case 5: case 6: case 7: case 8: return 50000;
        case 9: case 10: case 11: case 12: return 60000;
        default:
            printf("❌ Invalid class! Only 1–12 allowed.\n");
            return 0;
    }
}
