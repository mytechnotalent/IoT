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

#include "client.h"

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

int main(void) {
    stdio_init_all();

    run_client(&TLS_CLIENT_SERVER, &TLS_CLIENT_HTTP_REQUEST);
}
