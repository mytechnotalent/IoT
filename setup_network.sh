#!/bin/bash

sudo nmcli device disconnect wlan0
read -p "enter the WiFi network name (SSID): " network_name
read -p "enter the WiFi password: " network_password
echo
export WIFI_SSID="$network_name"
export WIFI_PASSWORD="$network_password"
sudo nmcli device wifi hotspot ssid "$network_name" password "$network_password"

