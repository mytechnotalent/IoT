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
 * @file client.c
 *
 * @brief Minimalistic TLS/SSL client to work with the Pico W.
 * 
 * This contains a minimalistic TLS/SSL client to work with the Raspberry Pi 
 * Pico W which you can build upon to make larger projects.
 *
 * @author Kevin Thomas
 * @date   12/15/2023
 */

#include <string.h>
#include <time.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/altcp_tcp.h"
#include "lwip/altcp_tls.h"
#include "lwip/dns.h"

/**
 * @brief  Structure representing a TLS client state.
 *
 *         This structure encapsulates the state of a TLS client, including the
 *         PCB (Protocol Control Block) for the altcp connection, completion
 *         status, error code, HTTP request, and timeout value.
 */
typedef struct TLS_CLIENT_T_ {
    struct altcp_pcb *pcb;
    bool complete;
    int error;
    const char *http_request;
    int timeout;
} TLS_CLIENT_T;

/**
 * @brief  Global variable representing the TLS configuration for altcp.
 *
 *         This global variable holds the TLS configuration for altcp, used in
 *         TLS client operations. It is initially set to NULL and should be
 *         configured before using it in TLS client functions.
 */
static struct altcp_tls_config *tls_config = NULL;

/**
 * @brief  Callback function for closing the TLS client connection.
 *
 *         This function is a callback for closing the TLS client connection.
 *         It sets the completion status, removes the associated PCB (Protocol
 *         Control Block), and performs cleanup operations to close the connection.
 *
 * @param  arg: Pointer to the TLS client state (TLS_CLIENT_T)
 * @retval ERR_OK if the close operation is successful, ERR_ABRT otherwise
 */
static err_t tls_client_close(void *arg) {
    TLS_CLIENT_T *state = (TLS_CLIENT_T*)arg;
    err_t err = ERR_OK;

    state->complete = true;
    if (state->pcb != NULL) {
        altcp_arg(state->pcb, NULL);
        altcp_poll(state->pcb, NULL, 0);
        altcp_recv(state->pcb, NULL);
        altcp_err(state->pcb, NULL);
        err = altcp_close(state->pcb);
        if (err != ERR_OK) {
            printf("close failed %d, calling abort\n", err);
            altcp_abort(state->pcb);
            err = ERR_ABRT;
        }
        state->pcb = NULL;
    }
    return err;
}

/**
 * @brief  Callback function for handling a successful TLS client connection.
 *
 *         This function is a callback for handling a successful TLS client
 *         connection. It prints a success message, sends an HTTP request to the
 *         server, and returns ERR_OK. If any error occurs during the connection
 *         or data writing, it prints an error message and calls the
 *         tls_client_close function to close the connection.
 *
 * @param  arg: Pointer to the TLS client state (TLS_CLIENT_T)
 * @param  pcb: Pointer to the altcp PCB for the connection
 * @param  err: Error code indicating the success or failure of the connection
 * @retval ERR_OK if the connection is successful, ERR_ABRT otherwise
 */
static err_t tls_client_connected(void *arg, struct altcp_pcb *pcb, err_t err) {
    TLS_CLIENT_T *state = (TLS_CLIENT_T*)arg;

    if (err != ERR_OK) {
        printf("connect failed %d\n", err);
        return tls_client_close(state);
    }
    printf("connected to server, sending request\n");
    err = altcp_write(state->pcb, state->http_request, strlen(state->http_request), TCP_WRITE_FLAG_COPY);
    if (err != ERR_OK) {
        printf("error writing data, err=%d", err);
        return tls_client_close(state);
    }
    return ERR_OK;
}

/**
 * @brief  Callback function for handling a timeout during TLS client operation.
 *
 *         This function is a callback for handling a timeout during a TLS client
 *         operation. It prints a timeout message, sets the error code
 *         (PICO_ERROR_TIMEOUT), and calls the tls_client_close function to close
 *         the connection.
 *
 * @param  arg: Pointer to the TLS client state (TLS_CLIENT_T)
 * @param  pcb: Pointer to the altcp PCB for the connection
 * @retval ERR_OK if the timeout is handled successfully, ERR_ABRT otherwise
 */
static err_t tls_client_poll(void *arg, struct altcp_pcb *pcb) {
    TLS_CLIENT_T *state = (TLS_CLIENT_T*)arg;

    printf("timed out\n");
    state->error = PICO_ERROR_TIMEOUT;
    return tls_client_close(arg);
}

/**
 * @brief  Callback function for handling TLS client errors.
 *
 *         This function is a callback for handling errors during TLS client
 *         operation. It prints an error message, calls the tls_client_close
 *         function to close the connection, and sets the error code to
 *         PICO_ERROR_GENERIC.
 *
 * @param  arg: Pointer to the TLS client state (TLS_CLIENT_T)
 * @param  err: Error code indicating the nature of the TLS client error
 * @retval None
 */
static void tls_client_err(void *arg, err_t err) {
    TLS_CLIENT_T *state = (TLS_CLIENT_T*)arg;

    printf("tls_client_err %d\n", err);
    tls_client_close(state);
    state->error = PICO_ERROR_GENERIC;
}

/**
 * @brief  Callback function for handling received data in the TLS client.
 *
 *         This function is a callback for handling received data in the TLS
 *         client. It checks if the connection is closed and calls the
 *         tls_client_close function if necessary. If data is received, it copies
 *         and prints the data, then frees the pbuf. It returns ERR_OK on
 *         successful data handling.
 *
 * @param  arg: Pointer to the TLS client state (TLS_CLIENT_T)
 * @param  pcb: Pointer to the altcp PCB for the connection
 * @param  p:   Pointer to the pbuf containing the received data
 * @param  err: Error code indicating the success or failure of the data reception
 * @retval ERR_OK if the data is handled successfully, ERR_ABRT otherwise
 */
static err_t tls_client_recv(void *arg, struct altcp_pcb *pcb, struct pbuf *p, err_t err) {
    TLS_CLIENT_T *state = (TLS_CLIENT_T*)arg;
    char buf[p->tot_len + 1];

    if (!p) {
        printf("connection closed\n");
        return tls_client_close(state);
    }
    if (p->tot_len > 0) {
        pbuf_copy_partial(p, buf, p->tot_len, 0);
        buf[p->tot_len] = 0;
        printf("***\nnew data received from server:\n***\n\n%s\n", buf);
        altcp_recved(pcb, p->tot_len);
    }
    pbuf_free(p);
    return ERR_OK;
}

/**
 * @brief  Initiates a connection to a server using its IP address in the TLS client.
 *
 *         This function initiates a connection to a server using its IP address
 *         in the TLS client. It prints a connection message, attempts to connect
 *         to the specified IP address and port, and handles errors by printing an
 *         error message and calling the tls_client_close function.
 *
 * @param  ipaddr: Pointer to the IP address of the server
 * @param  state:  Pointer to the TLS client state (TLS_CLIENT_T)
 * @retval None
 */
static void tls_client_connect_to_server_ip(const ip_addr_t *ipaddr, TLS_CLIENT_T *state)
{
    err_t err;
    u16_t port = 443;

    printf("connecting to server IP %s port %d\n", ipaddr_ntoa(ipaddr), port);
    err = altcp_connect(state->pcb, ipaddr, port, tls_client_connected);
    if (err != ERR_OK) {
        fprintf(stderr, "error initiating connect, err=%d\n", err);
        tls_client_close(state);
    }
}

/**
 * @brief  Callback function for handling DNS resolution results in the TLS client.
 *
 *         This function is a callback for handling DNS resolution results in
 *         the TLS client. If the DNS resolution is successful, it prints a
 *         completion message and calls the tls_client_connect_to_server_ip
 *         function to initiate a connection to the resolved IP address. If there
 *         is an error in resolving the hostname, it prints an error message and
 *         calls the tls_client_close function.
 *
 * @param  hostname: Hostname that was resolved
 * @param  ipaddr:   Pointer to the resolved IP address
 * @param  arg:      Pointer to the TLS client state (TLS_CLIENT_T)
 * @retval None
 */
static void tls_client_dns_found(const char* hostname, const ip_addr_t *ipaddr, void *arg)
{
    if (ipaddr) {
        printf("DNS resolving complete\n");
        tls_client_connect_to_server_ip(ipaddr, (TLS_CLIENT_T *) arg);
    } else {
        printf("error resolving hostname %s\n", hostname);
        tls_client_close(arg);
    }
}

/**
 * @brief  Opens a TLS client connection to the specified hostname.
 *
 *         This function opens a TLS client connection to the specified hostname.
 *         It creates a new altcp PCB with the provided TLS configuration, sets
 *         callback functions, and initiates DNS resolution for the hostname. If
 *         the hostname resolution is successful, it connects to the resolved IP
 *         address. If there is an error during the process, it prints an error
 *         message and returns false. The function returns true if the connection
 *         process is successful or in progress.
 *
 * @param  hostname: Hostname of the server to connect to
 * @param  arg:      Pointer to the TLS client state (TLS_CLIENT_T)
 * @retval true if the connection process is successful or in progress, false otherwise
 */
static bool tls_client_open(const char *hostname, void *arg) {
    err_t err;
    ip_addr_t server_ip;
    TLS_CLIENT_T *state = (TLS_CLIENT_T*)arg;

    state->pcb = altcp_tls_new(tls_config, IPADDR_TYPE_ANY);
    if (!state->pcb) {
        printf("failed to create pcb\n");
        return false;
    }
    altcp_arg(state->pcb, state);
    altcp_poll(state->pcb, tls_client_poll, state->timeout * 2);
    altcp_recv(state->pcb, tls_client_recv);
    altcp_err(state->pcb, tls_client_err);
    mbedtls_ssl_set_hostname(altcp_tls_context(state->pcb), hostname);
    printf("resolving %s\n", hostname);
    cyw43_arch_lwip_begin();
    err = dns_gethostbyname(hostname, &server_ip, tls_client_dns_found, state);
    if (err == ERR_OK) {
        /* host is in DNS cache */
        tls_client_connect_to_server_ip(&server_ip, state);
    }
    else if (err != ERR_INPROGRESS) {
        printf("error initiating DNS resolving, err=%d\n", err);
        tls_client_close(state->pcb);
    }
    cyw43_arch_lwip_end();
    return err == ERR_OK || err == ERR_INPROGRESS;
}

/**
 * @brief  Initializes a TLS client state.
 *
 *         This function initializes a TLS client state by allocating memory for
 *         the TLS_CLIENT_T structure. It returns a pointer to the allocated
 *         memory. If the allocation fails, it prints an error message and
 *         returns NULL.
 *
 * @retval Pointer to the initialized TLS_CLIENT_T structure or NULL on failure
 */
static TLS_CLIENT_T* tls_client_init(void) {
    TLS_CLIENT_T *state = calloc(1, sizeof(TLS_CLIENT_T));

    if (!state) {
        printf("failed to allocate state\n");
        return NULL;
    }
    return state;
}

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
static bool run_tls_client_test(const uint8_t *cert, size_t cert_len, const char *server, const char *request) {
    TLS_CLIENT_T *state = tls_client_init();
    int err = state->error;

    /* no CA certificate checking */
    tls_config = altcp_tls_create_config_client(cert, cert_len);
    assert(tls_config);
    if (!state)
        return false;
    state->http_request = request;
    state->timeout = 10;
    if (!tls_client_open(server, state))
        return false;
    while(!state->complete) {
#if PICO_CYW43_ARCH_POLL
        cyw43_arch_poll();
        cyw43_arch_wait_for_work_until(make_timeout_time_ms(1000));
#else
        sleep_ms(1000);
#endif
    }
    free(state);
    altcp_tls_free_config(tls_config);
    return err == 0;
}

void init_client(const char *TLS_CLIENT_SERVER, const char *TLS_CLIENT_HTTP_REQUEST) {
  int8_t pico_error_code = 0;
  int8_t retries = 3;

  if (cyw43_arch_init()) {
    printf("failed to initialise\r\n");
    return;
  }

  cyw43_arch_enable_sta_mode();

  while (retries > 0) {
    if (pico_error_code = cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
      printf("Failed to connect: %d\r\n", pico_error_code);
      retries--;
      continue;
    }

    bool pass = run_tls_client_test(NULL, 0, TLS_CLIENT_SERVER, TLS_CLIENT_HTTP_REQUEST);
    if (pass) {
      printf("Test passed.\r\n");
      break;
    } else {
      printf("Test failed.\r\n");
      retries--;
    }
  }

  if (retries == 0) {
    printf("Exceeded retry limit.\r\n");
  }

  sleep_ms(100);
  cyw43_arch_deinit();
  printf("All done...\r\n");
}
