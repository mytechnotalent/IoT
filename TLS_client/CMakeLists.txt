cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico_w)
set(PICO_SDK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../pico-sdk)

include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

project(IoT C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

if (PICO_CYW43_SUPPORTED) # set by PICO_BOARD=pico_w
    if (NOT TARGET pico_cyw43_arch)
        message("Skipping Pico W examples as support is not available")
    else()

        if (DEFINED ENV{WIFI_SSID} AND (NOT WIFI_SSID))
            set(WIFI_SSID $ENV{WIFI_SSID})
            message("Using WIFI_SSID from environment ('${WIFI_SSID}')")
        endif()

        if (DEFINED ENV{WIFI_PASSWORD} AND (NOT WIFI_PASSWORD))
            set(WIFI_PASSWORD $ENV{WIFI_PASSWORD})
            message("Using WIFI_PASSWORD from environment")
        endif()

        set(WIFI_SSID "${WIFI_SSID}" CACHE INTERNAL "WiFi SSID for examples")
        set(WIFI_PASSWORD "${WIFI_PASSWORD}" CACHE INTERNAL "WiFi password for examples")
    endif()
endif()

set(WIFI_SSID "${WIFI_SSID}" CACHE INTERNAL "WiFi SSID")
set(WIFI_PASSWORD "${WIFI_PASSWORD}" CACHE INTERNAL "WiFi PASSWORD")

add_executable(IoT
        main.c
        tls_common.c
        )
target_compile_definitions(IoT PRIVATE
        WIFI_SSID=\"${WIFI_SSID}\"
        WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
        )
target_include_directories(IoT PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts
        )
target_link_libraries(IoT
        pico_cyw43_arch_lwip_threadsafe_background
        pico_lwip_mbedtls
        pico_mbedtls
        pico_stdlib
        )

pico_enable_stdio_usb(IoT 0)
pico_enable_stdio_uart(IoT 1)

pico_add_extra_outputs(IoT)
