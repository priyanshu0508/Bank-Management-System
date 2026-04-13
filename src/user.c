#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "user.h"
#include "storage.h"
#include "transaction.h"

bool create_account(const UserAccount *new_account) {
    // Check if account already exists
    UserAccount temp;
    if(storage_read_user(new_account->account_number, &temp)) {
        return false; // Already exists
    }
    
    if(storage_write_user(new_account)) {
        // Log opening deposit
        if (new_account->balance > 0) {
            TransactionRecord t;
            t.transaction_id = rand(); // Replace with better id gen later
            t.account_number = new_account->account_number;
            t.type = TRANS_DEPOSIT;
            t.amount = new_account->balance;
            t.timestamp_sec = time(NULL);
            t.related_account = 0;
            log_transaction(&t);
        }
        return true;
    }
    
    return false;
}

bool update_account(uint32_t acc_num, const UserAccount *updated) {
    return storage_update_user(acc_num, updated);
}

bool get_account(uint32_t acc_num, UserAccount *out_account) {
    return storage_read_user(acc_num, out_account);
}

bool perform_deposit(uint32_t acc_num, double amount) {
    if (amount <= 0) return false;
    
    UserAccount user;
    if(get_account(acc_num, &user)) {
        user.balance += amount;
        if(update_account(acc_num, &user)) {
            TransactionRecord t;
            t.transaction_id = rand();
            t.account_number = acc_num;
            t.type = TRANS_DEPOSIT;
            t.amount = amount;
            t.timestamp_sec = time(NULL);
            t.related_account = 0;
            log_transaction(&t);
            return true;
        }
    }
    return false;
}

bool perform_withdrawal(uint32_t acc_num, double amount) {
    if (amount <= 0) return false;
    
    UserAccount user;
    if(get_account(acc_num, &user)) {
        if (user.balance >= amount) {
            user.balance -= amount;
            if(update_account(acc_num, &user)) {
                TransactionRecord t;
                t.transaction_id = rand();
                t.account_number = acc_num;
                t.type = TRANS_WITHDRAWAL;
                t.amount = amount;
                t.timestamp_sec = time(NULL);
                t.related_account = 0;
                log_transaction(&t);
                return true;
            }
        }
    }
    return false;
}

bool perform_transfer(uint32_t from_acc, uint32_t to_acc, double amount) {
    if (amount <= 0 || from_acc == to_acc) return false;
    
    UserAccount from_user, to_user;
    if(get_account(from_acc, &from_user) && get_account(to_acc, &to_user)) {
        if(from_user.balance >= amount) {
            from_user.balance -= amount;
            to_user.balance += amount;
            
            if(update_account(from_acc, &from_user) && update_account(to_acc, &to_user)) {
                // Log transfer for BOTH users simultaneously (Data Redundancy Proof)
                // storage_read_transactions() inherently queries BOTH 'account_number' and 'related_account' IDs,
                // so we only write ONE structural footprint to the disk.
                TransactionRecord t1;
                t1.transaction_id = rand();
                t1.account_number = from_acc;
                t1.type = TRANS_TRANSFER;
                t1.amount = amount;
                t1.timestamp_sec = time(NULL);
                t1.related_account = to_acc;
                log_transaction(&t1);
                
                return true;
            }
        }
    }
    return false;
}
