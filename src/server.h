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
 * @brief  Defines a type alias for a function pointer to functions with no arguments and void return type.
 *
 *         This typedef creates an alias named 'func_ptr' for a pointer to a function that takes no arguments
 *         and returns void. It is commonly used to improve code readability and simplify declarations of
 *         function pointers with the specified signature.
 */
typedef void (*func_ptr)(void);

/**
 * @brief  Runs a server in an Internet of Things (IoT) context using the specified function pointer.
 *
 *         This function initiates the execution of a server within the context of Internet of Things (IoT)
 *         operations, using the provided function pointer. The details of server implementation and IoT context
 *         are expected to be specified within the function passed as a parameter. The function performs the
 *         following steps:
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
 * @param  func_ptrs Variadic function pointers to the IoT-specific server implementation.
 * @retval None
 */
void run_server(func_ptr *func_ptrs);

#endif // SERVER_H

