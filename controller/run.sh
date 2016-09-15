#!/bin/bash
echo "Building and uploading Trogdor controller"
sudo ino build -m nano328;
sudo ino upload -m nano328 -p /dev/ttyUSB0;
