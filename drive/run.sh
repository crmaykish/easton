#!/bin/bash
echo "Building and uploading Arduino sketch..."
sudo ino build -m nano328;
sudo ino upload -p /dev/ttyUSB0 -m nano328;
