#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct account {
    char username[50];
    int pin;
    float balance;
};

struct transaction {
    char username[50];
    char type[20];
    float amount;
};

void createAccount();
int login(char username[]);                // now fills username on success
void deposit(const char username[]);
void withdraw(const char username[]);
void checkBalance(const char username[]);
void transactionHistory(const char username[]);
void saveTransaction(const char username[], const char type[], float amount);

int main() {
    int choice, logged_in;
    char username[50];

    while (1) {
        printf("\n===== ONLINE BANKING SYSTEM =====\n");
        printf("1. Create Account\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {        // validate input
            printf("Invalid input. Exiting.\n");
            return 1;
        }

        switch (choice) {
            case 1:
                createAccount();
                break;

            case 2:
                if (login(username)) {
                    printf("\nLogin Successful! Welcome, %s\n", username);

                    int option;
                    while (1) {
                        printf("\n===== BANK MENU =====\n");
                        printf("1. Deposit Money\n");
                        printf("2. Withdraw Money\n");
                        printf("3. Check Balance\n");
                        printf("4. Transaction History\n");
                        printf("5. Logout\n");
                        printf("Enter choice: ");
                        if (scanf("%d", &option) != 1) {
                            printf("Invalid input. Returning to main menu.\n");
                            break;
                        }

                        switch (option) {
                            case 1:
                                deposit(username);
                                break;
                            case 2:
                                withdraw(username);
                                break;
                            case 3:
                                checkBalance(username);
                                break;
                            case 4:
                                transactionHistory(username);
                                break;
                            case 5:
                                goto end_session;
                            default:
                                printf("Invalid option!\n");
                        }
                    }
                } else {
                    printf("Invalid username or PIN (or no accounts exist).\n");
                }
                break;

            case 3:
                printf("Goodbye!\n");
                exit(0);

            default:
                printf("Invalid choice!\n");
        }

        end_session: ;
    }
    return 0;
}

// Create a new account
void createAccount() {
    struct account acc;
    FILE *fp = fopen("accounts.dat", "ab");
    if (fp == NULL) {
        perror("Error opening accounts file for writing");
        return;
    }

    printf("\nEnter Username: ");
    scanf("%49s", acc.username);           // limit input
    printf("Create 4-digit PIN: ");
    if (scanf("%d", &acc.pin) != 1) {
        printf("Invalid PIN input.\n");
        fclose(fp);
        return;
    }

    acc.balance = 0.0f;

    fwrite(&acc, sizeof(acc), 1, fp);
    fclose(fp);

    printf("\nAccount created successfully!\n");
}

// Login function: fills username[] and returns 1 on success
int login(char username[]) {
    char inputUser[50];
    int pin;

    printf("\nEnter Username: ");
    scanf("%49s", inputUser);
    printf("Enter PIN: ");
    if (scanf("%d", &pin) != 1) {
        printf("Invalid PIN input.\n");
        return 0;
    }

    struct account acc;
    FILE *fp = fopen("accounts.dat", "rb");
    if (fp == NULL) {
        // no accounts file yet
        // printf("No accounts found. Please create an account first.\n");
        return 0;
    }

    while (fread(&acc, sizeof(acc), 1, fp) == 1) {
        if (strcmp(acc.username, inputUser) == 0 && acc.pin == pin) {
            fclose(fp);
            strcpy(username, inputUser);   // return username to caller
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

// Deposit money
void deposit(const char username[]) {
    struct account acc;
    FILE *fp = fopen("accounts.dat", "rb+");
    if (fp == NULL) {
        printf("Accounts database not found. Create an account first.\n");
        return;
    }

    float amount;
    printf("Enter amount to deposit: ");
    if (scanf("%f", &amount) != 1 || amount <= 0.0f) {
        printf("Invalid amount.\n");
        fclose(fp);
        return;
    }

    while (fread(&acc, sizeof(acc), 1, fp) == 1) {
        if (strcmp(acc.username, username) == 0) {
            acc.balance += amount;

            fseek(fp, -((long)sizeof(acc)), SEEK_CUR);
            fwrite(&acc, sizeof(acc), 1, fp);
            fflush(fp);

            saveTransaction(username, "Deposit", amount);
            printf("Amount deposited successfully! New balance: ₹%.2f\n", acc.balance);

            fclose(fp);
            return;
        }
    }

    printf("Account not found.\n");
    fclose(fp);
}

// Withdraw money
void withdraw(const char username[]) {
    struct account acc;
    FILE *fp = fopen("accounts.dat", "rb+");
    if (fp == NULL) {
        printf("Accounts database not found. Create an account first.\n");
        return;
    }

    float amount;
    printf("Enter amount to withdraw: ");
    if (scanf("%f", &amount) != 1 || amount <= 0.0f) {
        printf("Invalid amount.\n");
        fclose(fp);
        return;
    }

    while (fread(&acc, sizeof(acc), 1, fp) == 1) {
        if (strcmp(acc.username, username) == 0) {
            if (acc.balance >= amount) {
                acc.balance -= amount;

                fseek(fp, -((long)sizeof(acc)), SEEK_CUR);
                fwrite(&acc, sizeof(acc), 1, fp);
                fflush(fp);

                saveTransaction(username, "Withdraw", amount);
                printf("Amount withdrawn successfully! New balance: ₹%.2f\n", acc.balance);
            } else {
                printf("Insufficient balance! Current balance: ₹%.2f\n", acc.balance);
            }

            fclose(fp);
            return;
        }
    }

    printf("Account not found.\n");
    fclose(fp);
}

// Check balance
void checkBalance(const char username[]) {
    struct account acc;
    FILE *fp = fopen("accounts.dat", "rb");
    if (fp == NULL) {
        printf("Accounts database not found.\n");
        return;
    }

    while (fread(&acc, sizeof(acc), 1, fp) == 1) {
        if (strcmp(acc.username, username) == 0) {
            printf("\nCurrent Balance: ₹%.2f\n", acc.balance);
            fclose(fp);
            return;
        }
    }

    printf("Account not found.\n");
    fclose(fp);
}

// Save each transaction
void saveTransaction(const char username[], const char type[], float amount) {
    struct transaction trans;
    FILE *fp = fopen("transactions.dat", "ab");
    if (fp == NULL) {
        perror("Warning: could not open transactions file for writing");
        return;
    }

    strncpy(trans.username, username, sizeof(trans.username)-1);
    trans.username[sizeof(trans.username)-1] = '\0';
    strncpy(trans.type, type, sizeof(trans.type)-1);
    trans.type[sizeof(trans.type)-1] = '\0';
    trans.amount = amount;

    fwrite(&trans, sizeof(trans), 1, fp);
    fclose(fp);
}

// Show user transactions
void transactionHistory(const char username[]) {
    struct transaction trans;
    FILE *fp = fopen("transactions.dat", "rb");
    if (fp == NULL) {
        printf("No transactions found.\n");
        return;
    }

    printf("\n=== TRANSACTION HISTORY for %s ===\n", username);
    int found = 0;
    while (fread(&trans, sizeof(trans), 1, fp) == 1) {
        if (strcmp(trans.username, username) == 0) {
            printf("%s : ₹%.2f\n", trans.type, trans.amount);
            found = 1;
        }
    }

    if (!found) printf("No transactions found for this user.\n");
    fclose(fp);
}
