#include <stdlib.h>
#include "transaction.h"
#include "storage.h"

bool log_transaction(const TransactionRecord *record) {
    return storage_append_transaction(record);
}

TransactionNode* get_history(uint32_t account_number) {
    return storage_read_transactions(account_number);
}

void free_history(TransactionNode *head) {
    TransactionNode *curr = head;
    while(curr != NULL) {
        TransactionNode *tmp = curr;
        curr = curr->next;
        free(tmp);
    }
}
