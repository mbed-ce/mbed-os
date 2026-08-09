/**
 * AES-CBC chaining tests.
 *
 * mbedtls_aes_crypt_cbc() promises a streaming contract: on return, the
 * callers iv[] holds the last ciphertext block processed. Splitting the
 * message over many calls should give the same result.
 *
 * Targets with MBEDTLS_AES_ALT (like the STM32 AES drivers) may rewrite this,
 * and may get this wrong.
 *
 * Plain MBEDTLS API level. Equally valid on software-only targets.
 */

#include <stdio.h>
#include <string.h>

#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if !defined(MBEDTLS_AES_C)
#error [NOT_SUPPORTED] MBEDTLS_AES_C undefined
#elif !defined(MBEDTLS_CIPHER_MODE_CBC)
#error [NOT_SUPPORTED] MBEDTLS_CIPHER_MODE_CBC undefined
#else

#include "mbedtls/aes.h"

using namespace utest::v1;

#define BLOCK_SIZE  16
#define MSG_SIZE    64  // 4 AES blocks, so a message can be split several ways

/* The following CBC example vectors were taken from NIST SP 800-38A, Appendix F:
 * CBC-AES128 from F.2.1 and F.2.2, and CBC-AES256 from F.2.5 and F.2.6,
 * both share the same plaintext and IV.
 * https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-38a.pdf
 */

static const unsigned char AES_IV[BLOCK_SIZE] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};

static const unsigned char PLAINTEXT[MSG_SIZE] = {
    0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
    0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
    0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c,
    0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
    0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11,
    0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
    0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17,
    0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10
};

static const unsigned char AES128_KEY[16] = {
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
};

static const unsigned char AES128_CIPHERTEXT[MSG_SIZE] = {
    0x76, 0x49, 0xab, 0xac, 0x81, 0x19, 0xb2, 0x46,
    0xce, 0xe9, 0x8e, 0x9b, 0x12, 0xe9, 0x19, 0x7d,
    0x50, 0x86, 0xcb, 0x9b, 0x50, 0x72, 0x19, 0xee,
    0x95, 0xdb, 0x11, 0x3a, 0x91, 0x76, 0x78, 0xb2,
    0x73, 0xbe, 0xd6, 0xb8, 0xe3, 0xc1, 0x74, 0x3b,
    0x71, 0x16, 0xe6, 0x9e, 0x22, 0x22, 0x95, 0x16,
    0x3f, 0xf1, 0xca, 0xa1, 0x68, 0x1f, 0xac, 0x09,
    0x12, 0x0e, 0xca, 0x30, 0x75, 0x86, 0xe1, 0xa7
};

static const unsigned char AES256_KEY[32] = {
    0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
    0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
    0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
    0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4
};

static const unsigned char AES256_CIPHERTEXT[MSG_SIZE] = {
    0xf5, 0x8c, 0x4c, 0x04, 0xd6, 0xe5, 0xf1, 0xba,
    0x77, 0x9e, 0xab, 0xfb, 0x5f, 0x7b, 0xfb, 0xd6,
    0x9c, 0xfc, 0x4e, 0x96, 0x7e, 0xdb, 0x80, 0x8d,
    0x67, 0x9f, 0x77, 0x7b, 0xc6, 0x70, 0x2c, 0x7d,
    0x39, 0xf2, 0x33, 0x69, 0xa9, 0xd9, 0xba, 0xcf,
    0xa5, 0x30, 0xe2, 0x63, 0x04, 0x23, 0x14, 0x61,
    0xb2, 0xeb, 0x05, 0xe2, 0xc3, 0x9b, 0xe9, 0xfc,
    0xda, 0x6c, 0x19, 0x07, 0x8c, 0x6a, 0x9d, 0x1b
};

/* Every way of cutting a 4-block message into calls that is worth testing.
 * A single-block-only sweep, or one that never puts a multi-block call in front
 * of another call, is exactly what let the encrypt bug survive. */
struct split_t {
    const char *name;
    size_t      chunk[5];   // It has to fit the zero-terminated as well
};

static const split_t SPLITS[] = {
    { "64",          { 64, 0 } },
    { "16+48",       { 16, 48, 0 } },
    { "48+16",       { 48, 16, 0 } },
    { "32+32",       { 32, 32, 0 } },
    { "16+32+16",    { 16, 32, 16, 0 } },
    { "16+16+16+16", { 16, 16, 16, 16, 0 } }
};

static const size_t NUM_SPLITS = sizeof(SPLITS) / sizeof(SPLITS[0]);

static char msg[128];

static void setkey(mbedtls_aes_context *ctx, int mode,
                   const unsigned char *key, unsigned int keybits)
{
    if (mode == MBEDTLS_AES_ENCRYPT) {
        TEST_ASSERT_EQUAL_INT(0, mbedtls_aes_setkey_enc(ctx, key, keybits));
    } else {
        TEST_ASSERT_EQUAL_INT(0, mbedtls_aes_setkey_dec(ctx, key, keybits));
    }
}

/* CBC one message in the given chunk sizes, carrying iv[] across calls, then check
 * against the expected vector. */
static void run_cbc(const char *label, int mode,
                    const unsigned char *key, unsigned int keybits,
                    const unsigned char *input, const unsigned char *expected,
                    const split_t *split)
{
    mbedtls_aes_context ctx;
    unsigned char iv[BLOCK_SIZE];
    unsigned char output[MSG_SIZE];
    size_t offset = 0;

    memset(output, 0, sizeof(output));
    memcpy(iv, AES_IV, sizeof(iv));

    mbedtls_aes_init(&ctx);
    setkey(&ctx, mode, key, keybits);

    for (const size_t *chunk = split->chunk; *chunk != 0; chunk++) {
        TEST_ASSERT_EQUAL_INT(0, mbedtls_aes_crypt_cbc(&ctx, mode, *chunk, iv,
                                                       input + offset,
                                                       output + offset));
        offset += *chunk;
    }

    mbedtls_aes_free(&ctx);

    TEST_ASSERT_EQUAL_UINT32(MSG_SIZE, offset);
    snprintf(msg, sizeof(msg), "%s, split %s", label, split->name);
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected, output, MSG_SIZE, msg);
}

static void run_all_splits(const char *label, int mode,
                           const unsigned char *key, unsigned int keybits,
                           const unsigned char *input,
                           const unsigned char *expected)
{
    for (size_t i = 0; i < NUM_SPLITS; i++) {
        run_cbc(label, mode, key, keybits, input, expected, &SPLITS[i]);
    }
}

static void check_iv_contract(const char *label, int mode,
                              const unsigned char *key, unsigned int keybits,
                              const unsigned char *input,
                              const unsigned char *ciphertext,
                              const split_t *split)
{
    mbedtls_aes_context ctx;
    unsigned char iv[BLOCK_SIZE];
    unsigned char output[MSG_SIZE];
    size_t offset = 0;

    memset(output, 0, sizeof(output));
    memcpy(iv, AES_IV, sizeof(iv));

    mbedtls_aes_init(&ctx);
    setkey(&ctx, mode, key, keybits);

    for (const size_t *chunk = split->chunk; *chunk != 0; chunk++) {
        TEST_ASSERT_EQUAL_INT(0, mbedtls_aes_crypt_cbc(&ctx, mode, *chunk, iv,
                                                       input + offset,
                                                       output + offset));
        offset += *chunk;

        snprintf(msg, sizeof(msg), "%s, split %s: IV after %u bytes",
                 label, split->name, (unsigned)offset);
        TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(ciphertext + offset - BLOCK_SIZE,
                                              iv, BLOCK_SIZE, msg);
    }

    mbedtls_aes_free(&ctx);
}

static void check_iv_contract_all_splits(const char *label, int mode,
                                         const unsigned char *key,
                                         unsigned int keybits,
                                         const unsigned char *input,
                                         const unsigned char *ciphertext)
{
    for (size_t i = 0; i < NUM_SPLITS; i++) {
        check_iv_contract(label, mode, key, keybits, input, ciphertext,
                          &SPLITS[i]);
    }
}

/* Baselines */

static void test_aes128_encrypt_single_call()
{
    run_cbc("AES-128-CBC encrypt", MBEDTLS_AES_ENCRYPT, AES128_KEY, 128,
            PLAINTEXT, AES128_CIPHERTEXT, &SPLITS[0]);
}

static void test_aes128_decrypt_single_call()
{
    run_cbc("AES-128-CBC decrypt", MBEDTLS_AES_DECRYPT, AES128_KEY, 128,
            AES128_CIPHERTEXT, PLAINTEXT, &SPLITS[0]);
}

/* The regression proper: every split, both directions, both key sizes. */

static void test_aes128_encrypt_all_splits()
{
    run_all_splits("AES-128-CBC encrypt", MBEDTLS_AES_ENCRYPT, AES128_KEY, 128,
                   PLAINTEXT, AES128_CIPHERTEXT);
}

static void test_aes128_decrypt_all_splits()
{
    run_all_splits("AES-128-CBC decrypt", MBEDTLS_AES_DECRYPT, AES128_KEY, 128,
                   AES128_CIPHERTEXT, PLAINTEXT);
}

static void test_aes256_encrypt_all_splits()
{
    run_all_splits("AES-256-CBC encrypt", MBEDTLS_AES_ENCRYPT, AES256_KEY, 256,
                   PLAINTEXT, AES256_CIPHERTEXT);
}

static void test_aes256_decrypt_all_splits()
{
    run_all_splits("AES-256-CBC decrypt", MBEDTLS_AES_DECRYPT, AES256_KEY, 256,
                   AES256_CIPHERTEXT, PLAINTEXT);
}

static void test_aes128_encrypt_iv_contract()
{
    check_iv_contract_all_splits("AES-128-CBC encrypt", MBEDTLS_AES_ENCRYPT,
                                 AES128_KEY, 128, PLAINTEXT, AES128_CIPHERTEXT);
}

static void test_aes128_decrypt_iv_contract()
{
    check_iv_contract_all_splits("AES-128-CBC decrypt", MBEDTLS_AES_DECRYPT,
                                 AES128_KEY, 128, AES128_CIPHERTEXT,
                                 AES128_CIPHERTEXT);
}

static void test_aes256_encrypt_iv_contract()
{
    check_iv_contract_all_splits("AES-256-CBC encrypt", MBEDTLS_AES_ENCRYPT,
                                 AES256_KEY, 256, PLAINTEXT, AES256_CIPHERTEXT);
}

static void test_aes256_decrypt_iv_contract()
{
    check_iv_contract_all_splits("AES-256-CBC decrypt", MBEDTLS_AES_DECRYPT,
                                 AES256_KEY, 256, AES256_CIPHERTEXT,
                                 AES256_CIPHERTEXT);
}

/* A zero-length call must be a no-op */
static void test_zero_length_call_is_a_noop()
{
    mbedtls_aes_context ctx;
    unsigned char iv[BLOCK_SIZE];
    unsigned char output[MSG_SIZE];

    memcpy(iv, AES_IV, sizeof(iv));
    mbedtls_aes_init(&ctx);
    setkey(&ctx, MBEDTLS_AES_ENCRYPT, AES128_KEY, 128);

    TEST_ASSERT_EQUAL_INT(0, mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_ENCRYPT, 0,
                                                   iv, PLAINTEXT, output));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(AES_IV, iv, BLOCK_SIZE);

    TEST_ASSERT_EQUAL_INT(0, mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_ENCRYPT,
                                                   MSG_SIZE, iv, PLAINTEXT,
                                                   output));
    mbedtls_aes_free(&ctx);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(AES128_CIPHERTEXT, output, MSG_SIZE);
}

/* Two live contexts share one AES peripheral e.g. alternating block by block. */
static void test_decrypt_interleaved_contexts()
{
    mbedtls_aes_context ctx_a, ctx_b;
    unsigned char iv_a[BLOCK_SIZE], iv_b[BLOCK_SIZE];
    unsigned char out_a[MSG_SIZE], out_b[MSG_SIZE];

    memcpy(iv_a, AES_IV, sizeof(iv_a));
    memcpy(iv_b, AES_IV, sizeof(iv_b));

    mbedtls_aes_init(&ctx_a);
    mbedtls_aes_init(&ctx_b);
    TEST_ASSERT_EQUAL_INT(0, mbedtls_aes_setkey_dec(&ctx_a, AES128_KEY, 128));
    TEST_ASSERT_EQUAL_INT(0, mbedtls_aes_setkey_dec(&ctx_b, AES256_KEY, 256));

    for (size_t offset = 0; offset < MSG_SIZE; offset += BLOCK_SIZE) {
        TEST_ASSERT_EQUAL_INT(0, mbedtls_aes_crypt_cbc(&ctx_a, MBEDTLS_AES_DECRYPT,
                                                       BLOCK_SIZE, iv_a,
                                                       AES128_CIPHERTEXT + offset,
                                                       out_a + offset));
        TEST_ASSERT_EQUAL_INT(0, mbedtls_aes_crypt_cbc(&ctx_b, MBEDTLS_AES_DECRYPT,
                                                       BLOCK_SIZE, iv_b,
                                                       AES256_CIPHERTEXT + offset,
                                                       out_b + offset));
    }

    mbedtls_aes_free(&ctx_a);
    mbedtls_aes_free(&ctx_b);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(PLAINTEXT, out_a, MSG_SIZE);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(PLAINTEXT, out_b, MSG_SIZE);
}

static utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(30, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

static Case cases[] = {
    Case("AES-128-CBC (enc) single call", test_aes128_encrypt_single_call),
    Case("AES-128-CBC (dec) single call", test_aes128_decrypt_single_call),
    Case("AES-128-CBC (enc) call all splits", test_aes128_encrypt_all_splits),
    Case("AES-128-CBC (dec) call all splits", test_aes128_decrypt_all_splits),
    Case("AES-256-CBC (enc) call all splits", test_aes256_encrypt_all_splits),
    Case("AES-256-CBC (dec) call all splits", test_aes256_decrypt_all_splits),
    Case("AES-128-CBC (enc) leaves last cipher block in IV", test_aes128_encrypt_iv_contract),
    Case("AES-128-CBC (dec) leaves last cipher block in IV", test_aes128_decrypt_iv_contract),
    Case("AES-256-CBC (enc) leaves last cipher block in IV", test_aes256_encrypt_iv_contract),
    Case("AES-256-CBC (dec) leaves last cipher block in IV", test_aes256_decrypt_iv_contract),
    Case("AES-CBC zero-length call is a no-op", test_zero_length_call_is_a_noop),
    Case("AES-CBC decrypt with interleaved contexts", test_decrypt_interleaved_contexts)
};

static Specification specification(greentea_setup, cases);

int main()
{
    return !Harness::run(specification);
}

#endif // MBEDTLS_AES_C && MBEDTLS_CIPHER_MODE_CBC
