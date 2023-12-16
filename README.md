<img src="https://github.com/mytechnotalent/IoT/blob/main/IoT.png?raw=true" width="400">

## FREE Reverse Engineering Self-Study Course [HERE](https://github.com/mytechnotalent/Reverse-Engineering-Tutorial)

<br>

# IoT
Iot TLS minimalistic project utilizing a Raspberry Pi 5 as a TLS server and a PicoW as a TLS client.

<br>

# Schematic RP5
![image](https://github.com/mytechnotalent/IoT/blob/main/rp5-proj-schematic.png?raw=true)

# Schematic Pico W
![image](https://github.com/mytechnotalent/IoT/blob/main/picow-proj-schematic.png?raw=true)

<br>

# Setup
```bash
sudo ./install.sh
./clone_pico-sdk.sh
./clone_openocd.sh
sudo ./build_openocd.sh
```

<br>

# Usage: Terminal 1
```bash
export WIFI_SSID="<YOUR_WIFI_SSID>" WIFI_PASSWORD="<YOUR_WIFI_PASSWORD>"
sudo nmcli device disconnect wlan0
sudo nmcli device wifi hotspot ssid $WIFI_SSID password $WIFI_PASSWORD
sudo make clean-server
sudo make server
make clean-client
make client
sudo make flash-client
sudo make run-server
```

# Usage: Terminal 2
```bash
make open-terminal
```

# `client_instance.c` within `TLS_client` Folder
```c

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
```

## UNDER DEVELOPMENT STANDBY...

<br>

## License
[MIT](https://raw.githubusercontent.com/mytechnotalent/IoT/main/LICENSE)
