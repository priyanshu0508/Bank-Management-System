#ifndef USER_H
#define USER_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_NAME_LEN 64
#define MAX_PHONE_LEN 16
#define MAX_EMAIL_LEN 64
#define SHA256_HEX_LEN 65

// Enums for Roles and Status
typedef enum {
    ROLE_CUSTOMER = 0,
    ROLE_ADMIN = 1
} Role;

typedef enum {
    STATUS_ACTIVE = 0,
    STATUS_FROZEN = 1,
    STATUS_CLOSED = 2
} AccountStatus;

// User Account structure representing data stored down on disk
typedef struct {
    uint32_t account_number;
    char full_name[MAX_NAME_LEN];
    char phone[MAX_PHONE_LEN];
    char email[MAX_EMAIL_LEN];
    char password_hash[SHA256_HEX_LEN];
    double balance;
    Role role;
    AccountStatus status;
} UserAccount;

// Function Prototypes for operations
bool create_account(const UserAccount *new_account);
bool update_account(uint32_t acc_num, const UserAccount *updated);
bool get_account(uint32_t acc_num, UserAccount *out_account);

// Core banking logic
bool perform_deposit(uint32_t acc_num, double amount);
bool perform_withdrawal(uint32_t acc_num, double amount);
bool perform_transfer(uint32_t from_acc, uint32_t to_acc, double amount);

#endif // USER_H
