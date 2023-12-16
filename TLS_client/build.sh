#!/bin/sh

export PICO_SDK_PATH=~/Documents/pico-sdk
read -p "Enter the WiFi network name (SSID): " network_name
read -p "Enter the WiFi password: " network_password
echo
export WIFI_SSID="$network_name"
export WIFI_PASSWORD="$network_password"
cmake -DCMAKE_BUILD_TYPE=Debug .
make

