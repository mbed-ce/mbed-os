/*
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef MBEDTLS_CONFIG_KWX1_H
#define MBEDTLS_CONFIG_KWX1_H

// Leave room for Nanostack on KWx1 devices by omitting TLS server support.
#undef MBEDTLS_SSL_SRV_C

#endif
