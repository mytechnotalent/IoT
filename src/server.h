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

/* constants */
#define SERVER_PORT 443
#define BUFFER_SIZE 1024

/**
 * @brief Creates and initializes an SSL context for the server.
 *
 * This function initializes the SSL library, sets up algorithms, and creates
 * an SSL context for a server using TLS_server_method().
 *
 * @retval A pointer to the created SSL context.
 */
SSL_CTX *create_context(void);

/**
 * @brief Configures an SSL context for the server.
 *
 * This function sets ECDH parameters, changes the working directory to
 * the "ssl" folder, and loads the server's certificate and private key files.
 *
 * @param ctx The SSL context to be configured.
 */
void configure_context(SSL_CTX *ctx);

/**
 * @brief Creates a TCP socket for the server.
 *
 * This function creates a TCP socket for the server to listen for incoming connections.
 *
 * @retval The file descriptor of the created socket.
 */
int8_t create_socket(void);

/**
 * @brief  Binds a socket to the specified address.
 *
 *         This function binds a socket to the specified address. It sets the
 *         socket option to allow immediate reuse after closure and binds the
 *         socket to the provided address. If any error occurs during this
 *         process, it prints an error message, closes the socket, and exits with
 *         a failure status.
 *
 * @param  server_fd:   File descriptor of the socket
 * @param  server_addr: Pointer to the sockaddr_in structure representing the address
 * @retval None
 */
void bind_socket(int8_t server_fd, struct sockaddr_in *server_addr);

/**
 * @brief Listens for incoming client connections on the server socket.
 *
 * This function puts the server socket in a passive mode to listen for incoming connections.
 *
 * @param server_fd The file descriptor of the server socket.
 */
void listen_for_connections(int8_t server_fd);

/**
 * @brief Accepts a client connection and returns the client socket.
 *
 * This function accepts an incoming connection on the server socket and returns
 * the file descriptor of the client socket.
 *
 * @param server_fd The file descriptor of the server socket.
 * @param client_addr The client's address structure.
 * @param addr_len The length of the client's address structure.
 * @retval The file descriptor of the accepted client socket.
 */
int8_t accept_connection(int8_t server_fd, struct sockaddr_in *client_addr, socklen_t *addr_len);

/**
 * @brief Creates an SSL connection using the provided SSL context and client socket.
 *
 * This function creates an SSL connection using the given SSL context and
 * associates it with the provided client socket.
 *
 * @param ctx The SSL context to be used for the connection.
 * @param client_fd The file descriptor of the client socket.
 * @retval A pointer to the created SSL structure.
 */
SSL *create_ssl_connection(SSL_CTX *ctx, int8_t client_fd);

/**
 * @brief Handles communication over an established SSL connection.
 *
 * This function reads data from the client, prints it, sends a response,
 * and handles any errors that may occur during the process.
 *
 * @param ssl The SSL structure representing the connection.
 * @param buffer A buffer to store the received data.
 */
void handle_ssl_connection(SSL *ssl, char *buffer);

/**
 * @brief  URL-decodes a string in-place.
 *
 *         This function URL-decodes a string in-place, replacing percent-encoded
 *         characters with their corresponding ASCII values. The decoded string
 *         replaces the original input string.
 *
 * @param  str: The string to be URL-decoded.
 */
void url_decode(char *str);

/**
 * @brief Closes the SSL connection and cleans up associated resources.
 *
 * This function shuts down the SSL connection, frees the SSL context,
 * and closes the client socket.
 *
 * @param ssl The SSL structure representing the connection.
 * @param ctx The SSL context associated with the connection.
 * @param client_fd The file descriptor of the client socket.
 */
void close_ssl_connection(SSL *ssl, SSL_CTX *ctx, int8_t client_fd);

/**
 * @brief  Initializes and runs an SSL/TLS server.
 *
 *         This function initializes and runs an SSL/TLS server. It performs the
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
 * @retval None
 */
void init_server(void);

#endif // SERVER_H
