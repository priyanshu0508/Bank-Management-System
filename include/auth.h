#ifndef AUTH_H
#define AUTH_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "user.h"

// SHA-256 definitions
#define SHA256_BLOCK_SIZE 32            // SHA256 outputs a 32 byte digest

typedef struct {
	uint8_t data[64];
	uint32_t datalen;
	unsigned long long bitlen;
	uint32_t state[8];
} SHA256_CTX;

// SHA256 core functions
void sha256_init(SHA256_CTX *ctx);
void sha256_update(SHA256_CTX *ctx, const uint8_t data[], size_t len);
void sha256_final(SHA256_CTX *ctx, uint8_t hash[]);

// Wrappers for ease of use
void hash_string(const char *input, char *output_hex);
bool verify_password(const char *input_password, const char *stored_hash);

// Session management
bool login(uint32_t acc_num, const char *password, UserAccount *logged_in_user);

#endif // AUTH_H
