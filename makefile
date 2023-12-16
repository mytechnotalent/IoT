SERVER_BUILD_DIR = build
SRC_DIR = src
SSL_DIR = ssl
TLS_CLIENT_DIR := $(CURDIR)/TLS_client
CLIENT_BUILD_DIR := $(CURDIR)/TLS_client/build
PICO_SDK_PATH := $(CURDIR)/../pico-sdk

clean-server:
	rm -rf $(SERVER_BUILD_DIR)
	rm -rf $(SSL_DIR)
	rm -f capture.pcap

server: 
	rm -rf $(SERVER_BUILD_DIR)
	rm -rf $(SSL_DIR)
	mkdir -p $(SERVER_BUILD_DIR)
	mkdir -p $(SSL_DIR)
	openssl genpkey -algorithm RSA -out $(SSL_DIR)/server.key
	openssl req -new -key $(SSL_DIR)/server.key -out $(SSL_DIR)/server.csr -subj "/CN=localhost"
	openssl x509 -req -days 365 -in $(SSL_DIR)/server.csr -signkey $(SSL_DIR)/server.key -out $(SSL_DIR)/server.crt
	gcc -Wall -Wextra -pedantic -c -o $(SRC_DIR)/server.o $(SRC_DIR)/server.c
	gcc -Wall -Wextra -pedantic -c -o $(SRC_DIR)/server_instance.o $(SRC_DIR)/server_instance.c
	gcc -o $(SERVER_BUILD_DIR)/server $(SRC_DIR)/server_instance.o $(SRC_DIR)/server.o -lssl -lcrypto

stop-network:
	nmcli device disconnect wlan0

start-network:
	nmcli device disconnect wlan0
	read -p "enter the WiFi network name (SSID): " network_name
	read -p "enter the WiFi password: " network_password
	echo
	export WIFI_SSID="$network_name"
	export WIFI_PASSWORD="$network_password"
	nmcli device wifi hotspot ssid "$network_name" password "$network_password"

run-server:
	$(SERVER_BUILD_DIR)/server

clean-client:
	rm -rf $(CLIENT_BUILD_DIR)
	rm -f $(TLS_CLIENT_DIR)/CMakeCache.txt
	rm -rf $(TLS_CLIENT_DIR)/CMakeFiles

client:
	rm -rf $(CLIENT_BUILD_DIR)
	mkdir -p $(CLIENT_BUILD_DIR)
	rm -f $(TLS_CLIENT_DIR)/CMakeCache.txt
	rm -rf $(TLS_CLIENT_DIR)/CMakeFiles
	PICO_SDK_PATH=$(PICO_SDK_PATH) \
	read -p "Enter the WiFi network name (SSID): " network_name; \
	read -p "Enter the WiFi password: " network_password; \
	echo; \
	WIFI_SSID="$network_name" WIFI_PASSWORD="$network_password" \
	cd $(CLIENT_BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Debug $(TLS_CLIENT_DIR) && make

capture-dumpcap:
	touch capture.pcap
	dumpcap -i wlan0 -w capture.pcap

open-terminal:
	screen /dev/ttyACM0 115200

