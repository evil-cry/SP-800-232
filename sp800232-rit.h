#ifndef SP800232_RIT_H
#define SP800232_RIT_H
#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint64_t state[5];
    uint64_t key[2];
    size_t offset;
    uint64_t flags;
} ascon_aead128_ctx;

#ifndef OPENSSL_BUILDING_OPENSSL
void ascon_test_state();
#endif
void ascon_aead128_init(ascon_aead128_ctx* ctx, const unsigned char* k, const unsigned char* n);
void ascon_aead128_encrypt_update(ascon_aead128_ctx* ctx, unsigned char* ct, const unsigned char* pt, size_t len);
void ascon_aead128_final(ascon_aead128_ctx* ctx, unsigned char* tag);
void ascon_aead128_aad_update(ascon_aead128_ctx* ctx, const unsigned char* in, size_t len);
void ascon_aead128_decrypt_update(ascon_aead128_ctx* ctx, unsigned char* pt, const unsigned char* ct, size_t len);
#define ascon_aead128_encrypt_final ascon_aead128_final
#define ascon_aead128_decrypt_final ascon_aead128_final
#endif /* SP800232_RIT_H */
