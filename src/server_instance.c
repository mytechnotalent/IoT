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
 * @file server_instance.c
 *
 * @brief Minimalistic TLS/SSL server instance to work with the Raspberry Pi 5.
 * 
 * This contains a minimalistic TLS/SSL server instance to work with the Raspberry Pi 5
 * which you can build upon to make larger projects.
 *
 * @author Kevin Thomas
 * @date   12/03/2023
 */

#include "server.h"

#include <ifaddrs.h>

int main(void) {
  struct sockaddr_in server_addr;
  socklen_t addr_len = sizeof(struct sockaddr_in);
  SSL_CTX *ctx;
  SSL *ssl;
  char buffer[BUFFER_SIZE];

  // initialize SSL
  SSL_library_init();

  // create and configure SSL context
  ctx = create_context();
  configure_context(ctx);

  // get list of all network interfaces
  struct ifaddrs *ifa_list, *ifa;
  if (getifaddrs(&ifa_list) == -1) {
    perror("getifaddrs");
    exit(1);
  }

  // find the wlan0 interface and use its address
  for (ifa = ifa_list; ifa; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET &&
        strcmp(ifa->ifa_name, "wlan0") == 0) {
      server_addr.sin_addr = ((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
      break;
    }
  }

  // free the list of interfaces
  freeifaddrs(ifa_list);

  // check if wlan0 was found
  if (!ifa) {
    printf("Error: wlan0 interface not found!\n");
    exit(1);
  }

  // create socket
  int server_fd = create_socket();

  // set up server address struct
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERVER_PORT);

  // bind the socket to wlan0
  bind_socket(server_fd, &server_addr);

  // listen for incoming connections
  listen_for_connections(server_fd);

  printf("server listening on port %d (wlan0)...\n", SERVER_PORT);

  // accept incoming connections
  int client_fd = accept_connection(server_fd, &server_addr, &addr_len);

  // create new SSL connection state
  ssl = create_ssl_connection(ctx, client_fd);

  // handle SSL connection
  handle_ssl_connection(ssl, buffer);

  // close the SSL connection and free the context
  close_ssl_connection(ssl, ctx, client_fd);

  // close the server socket
  close(server_fd);

  return 0;
}
