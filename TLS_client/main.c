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

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#define TLS_CLIENT_SERVER       "10.42.0.1"
#define MESSAGE                 "hello%20world"
#define MESSAGE_LEN             "13"
#define TLS_CLIENT_HTTP_REQUEST "POST / HTTP/1.1\r\n" \
                                "Host: " TLS_CLIENT_SERVER "\r\n" \
                                "Connection: close\r\n" \
                                "Content-Type: application/x-www-form-urlencoded\r\n" \
                                "Content-Length: " MESSAGE_LEN "\r\n" \
                                "\r\n" \
                                MESSAGE
#define TLS_CLIENT_TIMEOUT_SECS 15

extern bool run_tls_client_test(const uint8_t *cert, size_t cert_len, const char *server, const char *request, int timeout);

int main(void) {
    stdio_init_all();

    if (cyw43_arch_init()) {
        printf("failed to initialise\r\n");
        return 1;
    }
    cyw43_arch_enable_sta_mode();

    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect\r\n");
        return 1;
    }
    bool pass = run_tls_client_test(NULL, 0, TLS_CLIENT_SERVER, TLS_CLIENT_HTTP_REQUEST, TLS_CLIENT_TIMEOUT_SECS);
    if (pass) {
        printf("Test passed.\r\n");
    } else {
        printf("Test failed.\r\n");
    }
    /* sleep a bit to let usb stdio write out any buffer to host */
    sleep_ms(100);
    cyw43_arch_deinit();
    printf("All done...\r\n");
    return pass ? 0 : 1;
}

