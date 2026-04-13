#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
  #include <conio.h>
  #include <windows.h>
#else
  #include <termios.h>
  #include <unistd.h>
#endif

#include "ui.h"
#include "auth.h"
#include "user.h"
#include "storage.h"

void ui_clear_screen() {
    printf("%s", ANSI_CLEAR_SCREEN);
}

void ui_print_header(const char* title) {
    ui_clear_screen();
    printf("%s%s========================================\n", ANSI_BLUE, ANSI_BOLD);
    printf("        %s\n", title);
    printf("========================================%s\n\n", ANSI_RESET);
}

void ui_print_error(const char* msg) {
    printf("%s[ERROR] %s%s\n", ANSI_RED, msg, ANSI_RESET);
}

void ui_print_success(const char* msg) {
    printf("%s[SUCCESS] %s%s\n", ANSI_GREEN, msg, ANSI_RESET);
}

void ui_get_string(const char* prompt, char* buffer, int max_len) {
    printf("%s%s%s ", ANSI_CYAN, prompt, ANSI_RESET);
    if(fgets(buffer, max_len, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;
        buffer[strcspn(buffer, "\r")] = 0;
    }
}

// OS-Specific get_password with asterisks
void ui_get_password(const char* prompt, char* buffer, int max_len) {
    printf("%s%s%s ", ANSI_CYAN, prompt, ANSI_RESET);
    int i = 0;
#ifdef _WIN32
    while (i < max_len - 1) {
        int c = _getch();
        if (c == '\r' || c == '\n') break;
        if (c == '\b') {
            if (i > 0) {
                printf("\b \b");
                i--;
            }
        } else {
            buffer[i++] = (char)c;
            printf("*");
        }
    }
    buffer[i] = '\0';
    printf("\n");
#else
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while (i < max_len - 1) {
        int c = getchar();
        if (c == '\n' || c == EOF) break;
        if (c == 127 || c == 8) { // Backspace or DEL
            if (i > 0) {
                printf("\b \b");
                i--;
            }
        } else {
            buffer[i++] = (char)c;
            printf("*");
        }
    }
    buffer[i] = '\0';
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\n");
#endif
}

static bool is_empty_or_whitespace(const char* str) {
    int len = strlen(str);
    if (len == 0) return true;
    for(int i = 0; i < len; i++) {
        if(str[i] != ' ' && str[i] != '\t') return false;
    }
    return true;
}

int ui_get_int(const char* prompt) {
    char buffer[32];
    while(1) {
        ui_get_string(prompt, buffer, 32);
        if (is_empty_or_whitespace(buffer)) {
            ui_print_error("Input cannot be empty. Please enter a valid number.");
            continue;
        }
        
        bool valid = true;
        int len = strlen(buffer);
        for(int i = 0; i < len; i++) {
            if(i == 0 && buffer[i] == '-') continue;
            if(buffer[i] < '0' || buffer[i] > '9') {
                valid = false;
                break;
            }
        }
        
        if (valid) return atoi(buffer);
        ui_print_error("Invalid format. Please enter numbers only.");
    }
}

double ui_get_double(const char* prompt) {
    char buffer[64];
    while(1) {
        ui_get_string(prompt, buffer, 64);
        if (is_empty_or_whitespace(buffer)) {
            ui_print_error("Input cannot be empty. Please enter a valid amount.");
            continue;
        }
        
        bool valid = true;
        bool has_dot = false;
        int len = strlen(buffer);
        for(int i = 0; i < len; i++) {
            if(i == 0 && buffer[i] == '-') continue;
            if(buffer[i] == '.') {
                if (has_dot) { valid = false; break; }
                has_dot = true;
                continue;
            }
            if(buffer[i] < '0' || buffer[i] > '9') {
                valid = false;
                break;
            }
        }
        if (valid) return atof(buffer);
        ui_print_error("Invalid format. Please enter a numerical decimal format only (e.g., 50.00).");
    }
}

void ui_pause() {
    printf("\n%sPress ENTER to continue...%s", ANSI_YELLOW, ANSI_RESET);
    char buf[2];
    fgets(buf, sizeof(buf), stdin);
}

static void get_valid_string(const char* prompt, char* buffer, int max_len) {
    while (1) {
        ui_get_string(prompt, buffer, max_len);
        if (!is_empty_or_whitespace(buffer)) break;
        ui_print_error("Field cannot be blank or contain only spaces. Please enter valid data.");
    }
}

static void get_valid_password(const char* prompt, char* buffer, int max_len) {
    while (1) {
        ui_get_password(prompt, buffer, max_len);
        if (!is_empty_or_whitespace(buffer)) break;
        ui_print_error("Password cannot be empty or just spaces.");
    }
}

static void get_valid_phone(const char* prompt, char* buffer, int max_len) {
    while (1) {
        ui_get_string(prompt, buffer, max_len);
        if (is_empty_or_whitespace(buffer)) {
            ui_print_error("Phone cannot be empty.");
            continue;
        }
        bool valid = true;
        for(int i = 0; i < (int)strlen(buffer); i++) {
            if(buffer[i] < '0' || buffer[i] > '9') { valid = false; break; }
        }
        if(valid && strlen(buffer) >= 7) break;
        ui_print_error("Invalid phone number. Digits only (min 7).");
    }
}

static void get_valid_email(const char* prompt, char* buffer, int max_len) {
    while (1) {
        ui_get_string(prompt, buffer, max_len);
        if (is_empty_or_whitespace(buffer)) {
            ui_print_error("Email cannot be empty.");
            continue;
        }
        if (strchr(buffer, '@') != NULL && strchr(buffer, '.') != NULL) break;
        ui_print_error("Invalid email format (missing '@' or domain).");
    }
}

void process_customer_registration() {
    ui_print_header("Register New Customer");
    UserAccount newUser;
    memset(&newUser, 0, sizeof(UserAccount));

    get_valid_string("Enter Full Name:", newUser.full_name, MAX_NAME_LEN);
    get_valid_phone("Enter Phone:", newUser.phone, MAX_PHONE_LEN);
    get_valid_email("Enter Email:", newUser.email, MAX_EMAIL_LEN);
    
    char password[64];
    get_valid_password("Enter secure password:", password, 64);
    hash_string(password, newUser.password_hash);
    
    newUser.balance = ui_get_double("Enter Initial Deposit:");
    if (newUser.balance < 0) newUser.balance = 0;
    
    // Auto-generate account number (Time-based simple generator)
    newUser.account_number = (uint32_t)time(NULL) % 1000000;
    if(newUser.account_number < 100000) newUser.account_number += 100000;
    
    newUser.role = ROLE_CUSTOMER;
    newUser.status = STATUS_ACTIVE;
    
    if(create_account(&newUser)) {
        char msg[128];
        snprintf(msg, sizeof(msg), "Account Created! Your Account Number is: %u", newUser.account_number);
        ui_print_success(msg);
    } else {
        ui_print_error("Failed to create account.");
    }
    ui_pause();
}

void process_login() {
    ui_print_header("Login");
    uint32_t acc_num = ui_get_int("Enter Account Number:");
    char password[64];
    ui_get_password("Enter Password:", password, 64);
    
    UserAccount user;
    if (login(acc_num, password, &user)) {
        if (user.role == ROLE_CUSTOMER) {
            ui_show_customer_menu(&user);
        } else {
            ui_show_admin_menu(&user);
        }
    } else {
        ui_print_error("Invalid account number or password.");
        ui_pause();
    }
}

void ui_show_main_menu() {
#ifdef _WIN32
    // Enable ANSI escape codes in Windows 10 Console
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif

    ui_print_header("Welcome to Advanced Bank Management System");
    printf("1. Login\n");
    printf("2. Register as Customer\n");
    printf("3. Exit\n");
    printf("----------------------------------------\n");
    int choice = ui_get_int("Select an option:");
    
    switch(choice) {
        case 1: process_login(); break;
        case 2: process_customer_registration(); break;
        case 3: 
            printf("%sGoodbye!%s\n", ANSI_YELLOW, ANSI_RESET);
            exit(0);
        default: ui_print_error("Invalid selection."); ui_pause();
    }
}

void ui_show_customer_menu(UserAccount* user) {
    while(1) {
        char title[128];
        snprintf(title, sizeof(title), "Customer Dashboard - %s", user->full_name);
        ui_print_header(title);
        printf("%sCurrent Balance: $%.2f%s\n\n", ANSI_GREEN, user->balance, ANSI_RESET);
        
        printf("1. Deposit\n");
        printf("2. Withdraw\n");
        printf("3. Transfer Funds\n");
        printf("4. Transaction History\n");
        printf("5. Logout\n");
        printf("----------------------------------------\n");
        int choice = ui_get_int("Select an option:");
        
        if (choice == 1) {
            double amount = ui_get_double("Enter deposit amount:");
            if(perform_deposit(user->account_number, amount)) {
                ui_print_success("Deposit successful.");
                storage_read_user(user->account_number, user); // Refresh user
            } else {
                ui_print_error("Deposit failed.");
            }
            ui_pause();
        } 
        else if (choice == 2) {
            double amount = ui_get_double("Enter withdrawal amount:");
            if(perform_withdrawal(user->account_number, amount)) {
                ui_print_success("Withdrawal successful.");
                storage_read_user(user->account_number, user); // Refresh
            } else {
                ui_print_error("Withdrawal failed. Insufficient funds or invalid amount.");
            }
            ui_pause();
        } 
        else if (choice == 3) {
            uint32_t target_acc = ui_get_int("Enter target account number:");
            double amount = ui_get_double("Enter transfer amount:");
            if(perform_transfer(user->account_number, target_acc, amount)) {
                ui_print_success("Transfer successful.");
                storage_read_user(user->account_number, user); // Refresh
            } else {
                ui_print_error("Transfer failed.");
            }
            ui_pause();
        }
        else if (choice == 4) {
            ui_print_header("Transaction History");
            TransactionNode* head = storage_read_transactions(user->account_number);
            if(head == NULL) {
                printf("No transactions found.\n");
            } else {
                printf("%-20s | %-12s | %s\n", "Date", "Type", "Amount");
                printf("---------------------------------------------------\n");
                TransactionNode* curr = head;
                while(curr != NULL) {
                    struct tm *tm_info = localtime(&curr->record.timestamp_sec);
                    char date[30];
                    strftime(date, 30, "%Y-%m-%d %H:%M:%S", tm_info);
                    
                    const char* type_str = "Unknown";
                    if(curr->record.type == TRANS_DEPOSIT) type_str = "Deposit";
                    if(curr->record.type == TRANS_WITHDRAWAL) type_str = "Withdrawal";
                    if(curr->record.type == TRANS_TRANSFER) type_str = "Transfer";
                    
                    printf("%-20s | %-12s | $%.2f\n", date, type_str, curr->record.amount);
                    curr = curr->next;
                }
                free_history(head);
            }
            ui_pause();
        }
        else if (choice == 5) {
            break; // logout
        }
    }
}

static void print_single_user(const UserAccount* user) {
    if (user->role == ROLE_CUSTOMER) {
         printf("%-10u | %-20s | %-12s | %-25s | %-15s | $%.2f\n", 
                user->account_number, user->full_name, user->phone, user->email, "[ENCRYPTED_HASH]", user->balance);
    }
}

void ui_show_admin_menu(UserAccount* admin) {
    (void)admin;
    while(1) {
        ui_print_header("Admin Control Panel");
        printf("1. View All Registered Customers\n");
        printf("2. Exit\n");
        printf("----------------------------------------\n");
        int choice = ui_get_int("Select an option:");
        
        if (choice == 1) {
            ui_print_header("Registered Customers Database");
            printf("%-10s | %-20s | %-12s | %-25s | %-15s | %-10s\n", 
                "Account ID", "Full Name", "Phone", "Email", "Password Status", "Balance");
            printf("---------------------------------------------------------------------------------------------------------\n");
            storage_iterate_users(print_single_user);
            ui_pause();
        } else if (choice == 2) {
            break;
        }
    }
}
