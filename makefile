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
	openssl req -new -key $(SSL_DIR)/server.key -out $(SSL_DIR)/server.csr -subj "/C=US/ST=VA/L=Ashburn/O=Organization/CN=10.42.0.1"
	openssl x509 -req -days 365 -in $(SSL_DIR)/server.csr -signkey $(SSL_DIR)/server.key -out $(SSL_DIR)/server.crt
	gcc -Wall -Wextra -pedantic -c -o $(SRC_DIR)/server.o $(SRC_DIR)/server.c
	gcc -Wall -Wextra -pedantic -c -o $(SRC_DIR)/server_instance.o $(SRC_DIR)/server_instance.c
	gcc -Wall -Wextra -pedantic -c -o $(SRC_DIR)/gpio.o $(SRC_DIR)/gpio.c
	gcc -o $(SERVER_BUILD_DIR)/server $(SRC_DIR)/server_instance.o $(SRC_DIR)/server.o $(SRC_DIR)/gpio.o -lssl -lcrypto

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
	cd $(CLIENT_BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Debug -DWIFI_SSID="$$WIFI_SSID" -DWIFI_PASSWORD="$$WIFI_PASSWORD" $(TLS_CLIENT_DIR) && make

flash-client:
	echo "/media/$(USER_NAME)/RPI-RP2" \
	@for i in $$(seq 1 10); do \
		if [ -d "/media/$(USER_NAME)/RPI-RP2" ]; then \
		echo "Flashing UF2..."; \
			break; \
		fi; \
		sleep 1; \
	done; \
	if [ ! -d "/media/$(USER_NAME)/RPI-RP2" ]; then \
		echo "Please put Pico W into boot mode!"; \
	else \
		sudo cp $(CLIENT_BUILD_DIR)/IoT.uf2 /media/$(USER_NAME)/RPI-RP2; \
	fi

run-server:
	$(SERVER_BUILD_DIR)/server

capture-dumpcap:
	touch capture.pcap
	dumpcap -i wlan0 -w capture.pcap

open-terminal:
	screen /dev/ttyACM0 115200

