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
 * @brief  Runs an SSL/TLS client continuously.
 *
 *         This function runs an SSL/TLS client continuously. It performs the
 *         following steps in an infinite loop:
 *         - Initializes the CYW43 WiFi module.
 *         - Enables station (STA) mode on the WiFi module.
 *         - Connects to the specified Wi-Fi network using the provided credentials.
 *         - Runs an SSL/TLS client test with the specified server and HTTP request.
 *         - Deinitializes the CYW43 WiFi module.
 *         - Prints a completion message.
 *
 * @param  TLS_CLIENT_SERVER:        The server to connect to (hostname or IP address)
 * @param  TLS_CLIENT_HTTP_REQUEST: The HTTP request to send to the server
 * @retval None
 */
void run_client(const char *TLS_CLIENT_SERVER, const char *TLS_CLIENT_HTTP_REQUEST);

#endif // SERVER_H

