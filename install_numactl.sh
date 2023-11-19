# FROM: https://github.com/numactl/numactl/archive/master.zip


#!/bin/bash

# Download the numactl source code from GitHub
wget https://github.com/numactl/numactl/archive/master.zip

# Unzip the downloaded file
unzip master.zip

# Enter the numactl directory
cd numactl-master

# Configure, build, and install numactl locally in the home directory
./autogen.sh
./configure --prefix=$HOME/numactl_install
make
make install

# Display information about the system's memory architecture using numactl -H
$HOME/numactl_install/bin/numactl -H > numactl.out

echo "numactl installed and information saved in numactl.out"