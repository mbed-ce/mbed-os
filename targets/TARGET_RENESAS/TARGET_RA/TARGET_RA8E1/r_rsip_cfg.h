/* generated configuration header file - do not edit */
#ifndef R_RSIP_CFG_H_
#define R_RSIP_CFG_H_
#define RSIP_CFG_ENC_KEY_SIZE(wrappped_key_value_size)                      (wrappped_key_value_size - 4)

            #define RSIP_CFG_PARAM_CHECKING_ENABLE                                      BSP_CFG_PARAM_CHECKING_ENABLE
            #define RSIP_CFG_AES_128_ENABLE                                             (1)
            #define RSIP_CFG_AES_192_ENABLE                                             (1)
            #define RSIP_CFG_AES_256_ENABLE                                             (1)
            #define RSIP_CFG_XTS_AES_128_ENABLE                                         (1)
            #define RSIP_CFG_XTS_AES_256_ENABLE                                         (1)
            #define RSIP_CFG_AES_ECB_CBC_CTR_ENABLE                                     (1)
            #define RSIP_CFG_AES_XTS_ENABLE                                             (1)
            #define RSIP_CFG_AES_GCM_ENABLE                                             (1)
            #define RSIP_CFG_AES_CCM_ENABLE                                             (1)
            #define RSIP_CFG_AES_CMAC_ENABLE                                            (1)

            #define RSIP_CFG_CHACHA20_ENABLE                                            (0)
            #define RSIP_CFG_CHACHA20_POLY1305_ENABLE                                   (0)

            #define RSIP_CFG_ECC_SECP256R1_ENABLE                                       (1)
            #define RSIP_CFG_ECC_SECP384R1_ENABLE                                       (1)
            #define RSIP_CFG_ECC_SECP521R1_ENABLE                                       (1)
            #define RSIP_CFG_ECC_SECP256K1_ENABLE                                       (1)
            #define RSIP_CFG_ECC_BRAINPOOLP256R1_ENABLE                                 (1)
            #define RSIP_CFG_ECC_BRAINPOOLP384R1_ENABLE                                 (1)
            #define RSIP_CFG_ECC_BRAINPOOLP512R1_ENABLE                                 (1)
            #define RSIP_CFG_ECC_EDWARDS25519_ENABLE                                    (1)

            #define RSIP_CFG_RSA_1024_ENABLE                                            (0)
            #define RSIP_CFG_RSA_2048_ENABLE                                            (1)
            #define RSIP_CFG_RSA_3072_ENABLE                                            (1)
            #define RSIP_CFG_RSA_4096_ENABLE                                            (1)

            #define RSIP_CFG_SHA1_ENABLE                                                (0)
            #define RSIP_CFG_SHA224_ENABLE                                              (1)
            #define RSIP_CFG_SHA256_ENABLE                                              (1)
            #define RSIP_CFG_SHA384_ENABLE                                              (1)
            #define RSIP_CFG_SHA512_ENABLE                                              (1)
            #define RSIP_CFG_SHA512_224_ENABLE                                          (1)
            #define RSIP_CFG_SHA512_256_ENABLE                                          (1)
            #define RSIP_CFG_SHA3_224_ENABLE                                            (0)
            #define RSIP_CFG_SHA3_256_ENABLE                                            (0)
            #define RSIP_CFG_SHA3_384_ENABLE                                            (0)
            #define RSIP_CFG_SHA3_512_ENABLE                                            (0)
            #define RSIP_CFG_HMAC_SHA1_ENABLE                                           (0)
            #define RSIP_CFG_HMAC_SHA224_ENABLE                                         (1)
            #define RSIP_CFG_HMAC_SHA256_ENABLE                                         (1)
            #define RSIP_CFG_HMAC_SHA384_ENABLE                                         (1)
            #define RSIP_CFG_HMAC_SHA512_ENABLE                                         (1)
            #define RSIP_CFG_HMAC_SHA512_224_ENABLE                                     (0)
            #define RSIP_CFG_HMAC_SHA512_256_ENABLE                                     (0)

            #define RSIP_CFG_KDF_SHA256_ENABLE                                          (1)
            #define RSIP_CFG_KDF_SHA384_ENABLE                                          (1)
            #define RSIP_CFG_KDF_SHA_ENABLE                                             (RSIP_CFG_KDF_SHA256_ENABLE || RSIP_CFG_KDF_SHA384_ENABLE)
            #define RSIP_CFG_KDF_HMAC_SHA256_ENABLE                                     (1)
            #define RSIP_CFG_KDF_HMAC_SHA384_ENABLE                                     (1)
            #define RSIP_CFG_KDF_HMAC_SHA512_ENABLE                                     (1)
            #define RSIP_CFG_KDF_HMAC_ENABLE                                            (RSIP_CFG_KDF_HMAC_SHA256_ENABLE || RSIP_CFG_KDF_HMAC_SHA384_ENABLE || RSIP_CFG_KDF_HMAC_SHA512_ENABLE)

            #define RSIP_CFG_FW_UPDATE_ENABLE                                           (1)

            #define RSIP_CFG_SHA_BUF_BLOCKS                                             (1U)
            #define RSIP_CFG_HMAC_BUF_BLOCKS                                            (1U)

            #define RSIP_CFG_WORD_SIZE_ENCRYPTED_KEY_OVERHEAD                           (4U)
            #define RSIP_CFG_WORD_SIZE_WRAPPED_KEY_OVERHEAD                             (5U)
            #define RSIP_CFG_WORD_SIZE_WRAPPED_PUBLIC_KEY_PREFIX                        (1U)
#endif /* R_RSIP_CFG_H_ */
