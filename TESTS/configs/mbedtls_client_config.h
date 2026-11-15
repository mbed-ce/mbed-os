/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MBEDTLS_GREENTEA_CLIENT_CONFIG_H
#define MBEDTLS_GREENTEA_CLIENT_CONFIG_H

// The socket tests use TLS clients. Omit server code on flash-limited targets.
#undef MBEDTLS_SSL_SRV_C

#endif
