#!/bin/bash

# This script is used to install the latest version of the mkpj tool.
# It is used to download the binary from the latest release and install it /usr/local/bin
# https://github.com/BaptisteP31/mkpj/

# Check if curl and wget are installed
if ! [ -x "$(command -v curl)" ]; then
    echo "Error: curl is not installed." >&2
    exit 1
fi

if ! [ -x "$(command -v wget)" ]; then
    echo "Error: wget is not installed." >&2
    exit 1
fi

# Check if the user is root, this part is complicated because the script is downloaded with curl
# if [ "$(id -u)" != "0" ]; then
#    echo "Error: You must be root to run this script." >&2
#    exit 1
# fi

# Get the latest release
LATEST_RELEASE=$(curl -s https://api.github.com/repos/BaptisteP31/mkpj/releases | grep "tag_name" | cut -d '"' -f 4 | head -n 1)

echo "Installing mkpj $LATEST_RELEASE, please wait..."

# Download the binary from the latest release
wget https://github.com/BaptisteP31/mkpj/releases/download/$LATEST_RELEASE/mkpj -O /usr/local/bin/mkpj &> /dev/null

# Make the binary executable
chmod +x /usr/local/bin/mkpj

# Check if the binary is installed
if [ -x "$(command -v mkpj)" ]; then
    echo "mkpj $LATEST_RELEASE was successfully installed."
else
    echo "Error: mkpj was not installed." >&2
fi
