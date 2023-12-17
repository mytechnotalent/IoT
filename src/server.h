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
 * @file server.h
 *
 * @brief Minimalistic TLS/SSL server header to work with a client on the Raspberry Pi 5.
 * 
 * This contains a minimalistic TLS/SSL header file to work with the server to work with the Raspberry Pi 5
 * which you can build upon to make larger projects.
 *
 * @author Kevin Thomas
 * @date   12/15/2023
 */

#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <ifaddrs.h>
#include <signal.h>

/* constants */
#define SERVER_PORT 443
#define BUFFER_SIZE 1024

/**
 * @brief  Runs an SSL/TLS server continuously.
 *
 *         This function runs an SSL/TLS server continuously. It performs the
 *         following steps in an infinite loop:
 *         - Initializes the SSL library.
 *         - Creates and configures the SSL context.
 *         - Retrieves the IP address of the "wlan0" interface.
 *         - Creates a socket and binds it to the "wlan0" interface.
 *         - Listens for incoming connections.
 *         - Accepts an incoming connection and creates an SSL connection state.
 *         - Handles the SSL connection, using a buffer for data exchange.
 *         - Closes the SSL connection and frees the SSL context.
 *         - Closes the server socket.
 *
 * @retval None
 */
void run_server(void);

#endif // SERVER_H
