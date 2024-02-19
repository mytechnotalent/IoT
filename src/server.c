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
 * @file server.c
 *
 * @brief Minimalistic TLS/SSL server to work with the Raspberry Pi 5.
 * 
 * This contains a minimalistic TLS/SSL server to work with the Raspberry Pi 5
 * which you can build upon to make larger projects.
 *
 * @author Kevin Thomas
 * @date   12/15/2023
 */

#include "server.h"
#include "gpio.h"

/**
 * @brief Creates and initializes an SSL context for the server.
 *
 * This function initializes the SSL library, sets up algorithms, and creates
 * an SSL context for a server using TLS_server_method().
 *
 * @retval A pointer to the created SSL context.
 */
static SSL_CTX *create_context(void);

/**
 * @brief Configures an SSL context for the server.
 *
 * This function sets ECDH parameters, changes the working directory to
 * the "ssl" folder, and loads the server's certificate and private key files.
 *
 * @param ctx The SSL context to be configured.
 */
static void configure_context(SSL_CTX *ctx);

/**
 * @brief Creates a TCP socket for the server.
 *
 * This function creates a TCP socket for the server to listen for incoming connections.
 *
 * @retval The file descriptor of the created socket.
 */
static int8_t create_socket(void);

/**
 * @brief Binds a socket to a specific address and port.
 *
 * This function binds the server socket to a specified address and port.
 *
 * @param server_fd The file descriptor of the server socket.
 * @param server_addr The server's address structure.
 */
static void bind_socket(int8_t server_fd, struct sockaddr_in *server_addr);

/**
 * @brief Listens for incoming client connections on the server socket.
 *
 * This function puts the server socket in a passive mode to listen for incoming connections.
 *
 * @param server_fd The file descriptor of the server socket.
 */
static void listen_for_connections(int8_t server_fd);

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
static int8_t accept_connection(int8_t server_fd, struct sockaddr_in *client_addr, socklen_t *addr_len);

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
static SSL *create_ssl_connection(SSL_CTX *ctx, int8_t client_fd);

/**
 * @brief Handles communication over an established SSL connection.
 *
 * This function reads data from the client, prints it, sends a response,
 * and handles any errors that may occur during the process.
 *
 * @param ssl The SSL structure representing the connection.
 * @param buffer A buffer to store the received data.
 * @param func_ptrs A variadic number of IoT related functions to be executed.
 */
static void handle_ssl_connection(SSL *ssl, char *buffer, func_ptr *func_ptrs);

/**
 * @brief  URL-decodes a string in-place.
 *
 *         This function URL-decodes a string in-place, replacing percent-encoded
 *         characters with their corresponding ASCII values. The decoded string
 *         replaces the original input string.
 *
 * @param  str The string to be URL-decoded.
 */
static void url_decode(char *str);

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
static void close_ssl_connection(SSL *ssl, SSL_CTX *ctx, int8_t client_fd);

static SSL_CTX *create_context(void) {
    SSL_CTX *ctx;

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(TLS_server_method());
    if (!ctx) {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}

static void configure_context(SSL_CTX *ctx) {
    SSL_CTX_set_ecdh_auto(ctx, 1);
    chdir("ssl");
    if (SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        abort();
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        abort();
    }
    chdir("..");
}

static int8_t create_socket(void) {
    int8_t server_fd;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    return server_fd;
}

static void bind_socket(int8_t server_fd, struct sockaddr_in *server_addr) {
    int opt = 1;

    // allow the socket to be reused immediately after it is closed
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    if (bind(server_fd, (struct sockaddr *)server_addr, sizeof(*server_addr)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
}

static void listen_for_connections(int8_t server_fd) {
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
}

static int8_t accept_connection(int8_t server_fd, struct sockaddr_in *client_addr, socklen_t *addr_len) {
    int8_t client_fd;

    if ((client_fd = accept(server_fd, (struct sockaddr *)client_addr, addr_len)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    return client_fd;
}

static SSL *create_ssl_connection(SSL_CTX *ctx, int8_t client_fd) {
    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, client_fd);

    if (SSL_accept(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    return ssl;
}

static void handle_ssl_connection(SSL *ssl, char *buffer, func_ptr *func_ptrs) {
    printf("SSL connection established!\n");
    // read data from the client
    uint64_t bytes_received = SSL_read(ssl, buffer, BUFFER_SIZE - 1);
    if (bytes_received <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    buffer[bytes_received] = '\0';  // null-terminate the received data
    printf("Received: %s\n", buffer);
    
    // parse the HTTP request to check if it's a POST request
    const char *post_token = "POST /";
    if (strncmp(buffer, post_token, strlen(post_token)) == 0) {
        // extract the POST data
        const char *content_length_token = "Content-Length: ";
        char *content_length_start = strstr(buffer, content_length_token);
        if (content_length_start) {
            uint32_t content_length = atoi(content_length_start + strlen(content_length_token));
            // move buffer pointer to the start of POST data
            char *post_data_start = strstr(buffer, "\r\n\r\n") + 4;
            if (post_data_start) {
                printf("Content-Length: %d\n", content_length);
                printf("POST Data: %.*s\n", content_length, post_data_start);
                // parse and handle the POST data (you can customize this part)
                // for example, extract the 'message' parameter
                char message[256];
                strncpy(message, post_data_start, content_length);
                message[content_length] = '\0';  // null-terminate the message
                // decode the URL-encoded data
                url_decode(message);
                // process the decoded message as needed
                printf("Decoded message: %s\n", message);
                // call custom functionality
                for (uint8_t i = 0; func_ptrs[i] != NULL; ++i)
                    func_ptrs[i]();
                // send a response to the client
                const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello from the server!";
                SSL_write(ssl, response, strlen(response));
            }
        }
    } else {
        // send a default response for other types of requests
        const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello from the server!";
        SSL_write(ssl, response, strlen(response));
    }
}

static void url_decode(char *str) {
    char *pos = str;

    while (*str) {
        if (*str == '%' && *(str + 1) && *(str + 2)) {
            char hex[3] = { *(str + 1), *(str + 2), '\0' };
            *pos = (char)strtol(hex, NULL, 16);
            str += 2; // move forward by two characters
        } else {
            *pos = *str;
        }
        ++str;
        ++pos;
    }
    *pos = '\0'; // null-terminate the decoded string
}

static void close_ssl_connection(SSL *ssl, SSL_CTX *ctx, int8_t client_fd) {
    // close the SSL connection and free the context
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    close(client_fd);
}

void run_server(func_ptr *func_ptrs) {
    struct sockaddr_in server_addr;
    struct ifaddrs *ifa_list, *ifa;
    socklen_t addr_len = sizeof(struct sockaddr_in);
    SSL_CTX *ctx;
    SSL *ssl;
    char buffer[BUFFER_SIZE];
    int8_t server_fd = 0;
    int8_t client_fd = 0;

    while (1) {
        // initialize SSL
        SSL_library_init();
        
        // create and configure SSL context
        ctx = create_context();
        configure_context(ctx);
        
        // get list of all network interfaces
        if (getifaddrs(&ifa_list) == -1) {
            perror("getifaddrs");
            exit(1);
        }
        
        // find the wlan0 interface and use its address
        for (ifa = ifa_list; ifa; ifa = ifa->ifa_next) {
            if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET && strcmp(ifa->ifa_name, "wlan0") == 0) {
                server_addr.sin_addr = ((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
                break;
            }
        }
        
        // free the list of interfaces
        freeifaddrs(ifa_list);
        
        // check if wlan0 was found
        if (!ifa) {
            printf("Error: wlan0 interface not found!\r\n");
            exit(1);
        }
        
        // create socket
        server_fd = create_socket();
        
        // set up server address struct
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(SERVER_PORT);
        
        // bind the socket to wlan0
        bind_socket(server_fd, &server_addr);
        
        // listen for incoming connections
        listen_for_connections(server_fd);
        printf("Server listening on port %d (wlan0)...\r\n", SERVER_PORT);
        
        // accept incoming connections
        client_fd = accept_connection(server_fd, &server_addr, &addr_len);
        
        // create new SSL connection state
        ssl = create_ssl_connection(ctx, client_fd);
        
        // handle SSL connection
        handle_ssl_connection(ssl, buffer, func_ptrs);
        
        // close the SSL connection and free the context
        close_ssl_connection(ssl, ctx, client_fd);
        
        // close the server socket
        close(server_fd);
    }
}
