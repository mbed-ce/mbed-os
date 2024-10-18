# Nuvoton M2354 RSA keypair

Two default RSA key pairs are given to the Nuvoton M2354 target.

Public keys were pre-compiled to `bl2.bin` and private keys are in
`nuvoton_m2354-root-rsa-3072.pem` and `nuvoton_m2354-root-rsa-3072_1.pem`
for Secure image and Non-Secure image separately.

DO NOT use them in production code, they are exclusively for testing!

Private keys must be stored in a safe place outside of the repository.

[Image tool](https://github.com/mcu-tools/mcuboot/blob/main/docs/imgtool.md)
can be used to generate new key pairs.
