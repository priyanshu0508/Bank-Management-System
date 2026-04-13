#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "storage.h"

#define USER_DB "data/users.dat"
#define TRANS_DB "data/transactions.dat"

#ifdef _WIN32
  #include <direct.h>
  #define MKDIR(path) _mkdir(path)
#else
  #define MKDIR(path) mkdir(path, 0777)
#endif

// Create data directory if it doesn't exist
void storage_init() {
    MKDIR("data");
    // Ensure files exist
    FILE *f1 = fopen(USER_DB, "ab");
    if(f1) fclose(f1);
    
    FILE *f2 = fopen(TRANS_DB, "ab");
    if(f2) fclose(f2);
}

bool storage_write_user(const UserAccount *user) {
    FILE *f = fopen(USER_DB, "ab");
    if (!f) return false;
    size_t written = fwrite(user, sizeof(UserAccount), 1, f);
    fclose(f);
    return written == 1;
}

bool storage_read_user(uint32_t acc_num, UserAccount *user) {
    FILE *f = fopen(USER_DB, "rb");
    if (!f) return false;
    
    bool found = false;
    while(fread(user, sizeof(UserAccount), 1, f) == 1) {
        if(user->account_number == acc_num) {
            found = true;
            break;
        }
    }
    fclose(f);
    return found;
}

bool storage_update_user(uint32_t acc_num, const UserAccount *updated_user) {
    FILE *f = fopen(USER_DB, "rb+");
    if (!f) return false;
    
    UserAccount temp;
    bool updated = false;
    while(fread(&temp, sizeof(UserAccount), 1, f) == 1) {
        if(temp.account_number == acc_num) {
            fseek(f, -(long)sizeof(UserAccount), SEEK_CUR);
            fwrite(updated_user, sizeof(UserAccount), 1, f);
            updated = true;
            break;
        }
    }
    fclose(f);
    return updated;
}

bool storage_append_transaction(const TransactionRecord *record) {
    FILE *f = fopen(TRANS_DB, "ab");
    if (!f) return false;
    size_t written = fwrite(record, sizeof(TransactionRecord), 1, f);
    fclose(f);
    return written == 1;
}

TransactionNode* storage_read_transactions(uint32_t acc_num) {
    FILE *f = fopen(TRANS_DB, "rb");
    if (!f) return NULL;
    
    TransactionNode* head = NULL;
    TransactionNode* tail = NULL;
    TransactionRecord record;
    
    while(fread(&record, sizeof(TransactionRecord), 1, f) == 1) {
        if(record.account_number == acc_num || record.related_account == acc_num) {
            TransactionNode* new_node = (TransactionNode*)malloc(sizeof(TransactionNode));
            new_node->record = record;
            new_node->next = NULL;
            
            if(head == NULL) {
                head = new_node;
                tail = new_node;
            } else {
                tail->next = new_node;
                tail = new_node;
            }
        }
    }
    fclose(f);
    return head;
}

void storage_iterate_users(UserCallback cb) {
    FILE *f = fopen(USER_DB, "rb");
    if (!f) return;
    UserAccount user;
    while(fread(&user, sizeof(UserAccount), 1, f) == 1) {
        cb(&user);
    }
    fclose(f);
}
