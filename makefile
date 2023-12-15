CC = gcc
CFLAGS = -Wall -Wextra -pedantic
LDFLAGS = -lssl -lcrypto

BUILD_DIR = build
SRC_DIR = src
SSL_DIR = ssl

SERVER_OBJ = $(BUILD_DIR)/server.o
SERVER_INSTANCE_OBJ = $(BUILD_DIR)/server_instance.o

all: $(BUILD_DIR)/server_instance 

install:
	apt install libssl-dev
	apt install tshark

$(BUILD_DIR)/server_instance: $(SERVER_INSTANCE_OBJ) $(SERVER_OBJ) $(SSL_DIR)/server.crt $(SSL_DIR)/server.key | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $(SERVER_INSTANCE_OBJ) $(SERVER_OBJ) $(LDFLAGS)

$(BUILD_DIR)/client_instance: $(CLIENT_INSTANCE_OBJ) $(CLIENT_OBJ) $(SSL_DIR)/server.crt $(SSL_DIR)/server.key | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $(CLIENT_INSTANCE_OBJ) $(CLIENT_OBJ) $(LDFLAGS)

$(SERVER_OBJ): $(SRC_DIR)/server.c $(SRC_DIR)/server.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/server_instance.o: $(SRC_DIR)/server_instance.c $(SRC_DIR)/server.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(SSL_DIR)/server.key:
	mkdir -p $(SSL_DIR)
	openssl genpkey -algorithm RSA -out $(SSL_DIR)/server.key

$(SSL_DIR)/server.crt: $(SSL_DIR)/server.key | $(SSL_DIR)
	openssl req -new -key $(SSL_DIR)/server.key -out $(SSL_DIR)/server.csr
	openssl x509 -req -days 365 -in $(SSL_DIR)/server.csr -signkey $(SSL_DIR)/server.key -out $(SSL_DIR)/server.crt

$(SSL_DIR):
	mkdir -p $(SSL_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(SSL_DIR)
	rm -f capture.pcap

dumpcap:
	touch capture.pcap
	dumpcap -i any -w capture.pcap

