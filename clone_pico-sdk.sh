#!/bin/bash

cd ..
rm -rf pico-sdk
git clone https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule update --init

