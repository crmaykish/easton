#!/bin/bash
echo "Trogdor Server build process started."

echo "Compiling CSS with Sass..."
sass static/style.scss static/style.css;
echo "Done"

echo "Starting Trogdor Server..."
sudo python3 api.py;
