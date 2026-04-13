#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdint.h>
#include <time.h>
#include <stdbool.h>

// Types of transactions
typedef enum {
    TRANS_DEPOSIT,
    TRANS_WITHDRAWAL,
    TRANS_TRANSFER
} TransactionType;

typedef struct {
    uint32_t transaction_id;
    uint32_t account_number;
    TransactionType type;
    double amount;
    time_t timestamp_sec;
    uint32_t related_account; // For transfers
} TransactionRecord;

// Node for a linked list of transactions (used for in-memory history before file flush)
typedef struct TransactionNode {
    TransactionRecord record;
    struct TransactionNode *next;
} TransactionNode;

// Function Prototypes
bool log_transaction(const TransactionRecord *record);
TransactionNode* get_history(uint32_t account_number);
void free_history(TransactionNode *head);

#endif // TRANSACTION_H
