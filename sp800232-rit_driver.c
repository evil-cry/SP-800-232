#include "sp800232-rit.h"
#include <assert.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int bbb_test_num = 0;
char* bbb_k = NULL;
char* bbb_n = NULL;
char* bbb_a = NULL;
char* bbb_p = NULL;
char* bbb_c = NULL;

/* string to bytes */
static void hex2bytes(unsigned char* out, const char* in)
{
    int i;
    int len;
    assert(out != NULL);
    assert(in != NULL);

    len = strlen(in);
    assert((len & 1) == 0);

    for (i = 0; i < len; i += 2)
        assert(sscanf(in + i, "%02hhx", out++) == 1);
}

static void test_encdec(int enc, int stream)
{
    ascon_aead128_ctx ctx;
    unsigned char *a = NULL, *p = NULL, *c = NULL;
    unsigned char k[16], n[16];
    unsigned char tag[16];
    int i, len = 0;

    if (bbb_a != NULL && bbb_a[0] != '?') {
        a = malloc(strlen(bbb_a));
    }
    if (bbb_c != NULL && bbb_c[0] != '?') {
        len = strlen(bbb_c);
        c = malloc(len);
        if (p == NULL)
            p = malloc(len + sizeof(tag));
    }
    if (bbb_p != NULL && bbb_p[0] != '?') {
        len = strlen(bbb_p);
        p = malloc(len);
        if (c == NULL)
            c = malloc(len + sizeof(tag));
    }
    if (k != NULL && bbb_k != NULL)
        hex2bytes(k, bbb_k);
    if (n != NULL && bbb_n != NULL)
        hex2bytes(n, bbb_n);
    if (a != NULL && bbb_a != NULL && bbb_a[0] != '?')
        hex2bytes(a, bbb_a);
    if (p != NULL && bbb_p != NULL && bbb_p[0] != '?')
        hex2bytes(p, bbb_p);
    if (c != NULL && bbb_c != NULL && bbb_c[0] != '?')
        hex2bytes(c, bbb_c);

    /* bytes are half the string length */
    len >>= 1;

    if (enc) {
        ascon_aead128_init(&ctx, k, n);
        if (a != NULL) {
            if (stream) {
                for (i = 0; i < strlen(bbb_a) / 2; i++)
                    ascon_aead128_aad_update(&ctx, a + i, 1);
            } else
                ascon_aead128_aad_update(&ctx, a, strlen(bbb_a) / 2);
        }

        if (stream) {
            for (i = 0; i < len; i++)
                ascon_aead128_encrypt_update(&ctx, c + i, p + i, 1);
        } else
            ascon_aead128_encrypt_update(&ctx, c, p, len);
        ascon_aead128_encrypt_final(&ctx, tag);

        for (i = 0; i < len; i++) {
            printf("%02X", c[i]);
        }
        for (i = 0; i < 16; i++) {
            printf("%02X", tag[i]);
        }
        printf("\n");
    } else {
        ascon_aead128_init(&ctx, k, n);
        if (a != NULL) {
            if (stream) {
                for (i = 0; i < strlen(bbb_a) / 2; i++)
                    ascon_aead128_aad_update(&ctx, a + i, 1);
            } else
                ascon_aead128_aad_update(&ctx, a, strlen(bbb_a) / 2);
        }
        if (stream) {
            for (i = 0; i < len - 16; i++)
                ascon_aead128_decrypt_update(&ctx, p + i, c + i, 1);
        } else
            ascon_aead128_decrypt_update(&ctx, p, c, len - 16);
        ascon_aead128_decrypt_final(&ctx, tag);

        for (i = 0; i < len - 16; i++) {
            printf("%02X", p[i]);
        }
        for (i = 0; i < 16; i++) {
            printf("%02X", tag[i]);
        }
        printf("\n");
    }
    free(a);
    free(p);
    free(c);
}

int main(int argc, char* argv[])
{
    int opt;

    /* parse command line options */
    while ((opt = getopt(argc, argv, "T:k:n:a:p:c:h")) != -1) {
        switch (opt) {
        case 'T':
            bbb_test_num = atoi(optarg);
            if (bbb_test_num < 0 || bbb_test_num > 99) {
                fprintf(stderr, "Error: Test number must be between 0 and 99\n");
                return 1;
            }
            break;
        case 'k':
            bbb_k = (char*)optarg;
            break;

        case 'n':
            bbb_n = (char*)optarg;
            break;
        case 'a':
            bbb_a = (char*)optarg;
            break;
        case 'p':
            bbb_p = (char*)optarg;
            break;
        case 'c':
            bbb_c = (char*)optarg;
            break;
        case 'h':
            printf("Usage: %s [OPTIONS]\n", argv[0]);
            printf("Options:\n");
            printf("  -T NUM    Run test number NUM (0-99)\n");
            printf("  -h        Display this help message\n");
            printf("\nAvailable tests:\n");
            printf("  -T0       internal debug\n");
            printf("  -T1       first test vector\n");
            return 0;
        default:
            fprintf(stderr, "Unknown option: -%c\n", opt);
            return 1;
        }
    }

    /* dispatch to appropriate test function */
    switch (bbb_test_num) {
    case 0:
        ascon_test_state();
        break;
    case 1:
        test_encdec(1, 0);
        break;
    case 2:
        test_encdec(1, 1);
        break;
    case 3:
        test_encdec(0, 0);
        break;
    case 4:
        test_encdec(0, 1);
        break;
    default:
        fprintf(stderr, "Test %d not implemented yet\n", bbb_test_num);
        return 1;
    }

    return 0;
}
