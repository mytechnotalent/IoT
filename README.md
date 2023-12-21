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
export HOSTNAME="<YOUR_HOSTNAME>"
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

<br>

## UNDER DEVELOPMENT STANDBY...

<br>

## License
[MIT](https://raw.githubusercontent.com/mytechnotalent/IoT/main/LICENSE)
