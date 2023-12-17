package main

import (
    "crypto/tls"
    "fmt"
    "io/ioutil"
    "net/http"
    "path/filepath"
)

func main() {
    http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
        // Handle POST requests
        if r.Method == http.MethodPost {
            // Read the request body
            body, err := ioutil.ReadAll(r.Body)
            if err != nil {
                http.Error(w, "Error reading request body", http.StatusInternalServerError)
                return
            }

            // Print the received data on the server
            fmt.Printf("Received data: %s\n", body)

            // You can perform additional processing here if needed

            // Respond to the client with a success status
            w.WriteHeader(http.StatusOK)
        } else {
            http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
        }
    })

    // Load the certificate and key files from the same level directory
    certFile, err := ioutil.ReadFile(filepath.Join("ssl", "server.crt"))
    if err != nil {
        fmt.Println("Error reading certificate file:", err)
        return
    }

    keyFile, err := ioutil.ReadFile(filepath.Join("ssl", "server.key"))
    if err != nil {
        fmt.Println("Error reading key file:", err)
        return
    }

    // Generate a certificate and key pair
    cert, err := tls.X509KeyPair(certFile, keyFile)
    if err != nil {
        fmt.Println("Error loading certificate and key:", err)
        return
    }

    // Configure the TLS server with the loaded certificate and key
    server := &http.Server{
        Addr:    ":443",
        Handler: http.DefaultServeMux,
        TLSConfig: &tls.Config{
            Certificates: []tls.Certificate{cert},
        },
    }

    // Start the server with TLS
    fmt.Printf("Server is running on https://localhost:%d...\n", 443)
    err = server.ListenAndServeTLS("", "")
    if err != nil {
        fmt.Println("Error starting server:", err)
    }
}

