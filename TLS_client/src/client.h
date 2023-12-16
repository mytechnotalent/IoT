/**
 * MIT License
 *
 * Copyright (c) 2023 My Techno Talent
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

/**
 * @file client.h
 *
 * @brief Minimalistic TLS/SSL client header to work with the Pico W.
 * 
 * This contains a minimalistic TLS/SSL client header to work with the Raspberry Pi 
 * Pico W which you can build upon to make larger projects.
 *
 * @author Kevin Thomas
 * @date   12/15/2023
 */

#ifndef SERVER_H
#define SERVER_H

#include <string.h>
#include <time.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/altcp_tcp.h"
#include "lwip/altcp_tls.h"
#include "lwip/dns.h"

/**
 * @brief  Runs a TLS client test with the specified parameters.
 *
 *         This function initializes a TLS client, sets up a TLS configuration
 *         using the provided certificate, and performs a TLS handshake with the
 *         server. It then sends an HTTP request and waits for the completion of
 *         the request. The function returns true if the test is successful,
 *         indicating that the TLS client was able to establish a connection and
 *         complete the request without errors.
 *
 * @param  cert: Pointer to the certificate data
 * @param  cert_len: Length of the certificate data
 * @param  server: The server address or hostname
 * @param  request: The HTTP request to be sent
 * @param  timeout: Timeout value for the TLS client operation in seconds
 * @retval true if the TLS client test is successful, false otherwise
 */
bool run_tls_client_test(const uint8_t *cert, size_t cert_len, const char *server, const char *request, uint8_t timeout);

#endif // SERVER_H

