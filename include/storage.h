#ifndef STORAGE_H
#define STORAGE_H

#include <stdbool.h>
#include <stdint.h>
#include "user.h"
#include "transaction.h"

// Initialize data files if they don't exist
void storage_init();

// Record level access
bool storage_write_user(const UserAccount *user);
bool storage_read_user(uint32_t acc_num, UserAccount *user);
bool storage_update_user(uint32_t acc_num, const UserAccount *updated_user);

bool storage_append_transaction(const TransactionRecord *record);
TransactionNode* storage_read_transactions(uint32_t acc_num);

// Iterate through the internal records safely
typedef void (*UserCallback)(const UserAccount*);
void storage_iterate_users(UserCallback cb);

#endif // STORAGE_H
