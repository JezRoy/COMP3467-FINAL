# FROM: https://github.com/numactl/numactl/archive/master.zip

#!/bin/bash

# Download the numactl source code from GitHub
wget https://github.com/numactl/numactl/archive/master.zip -d "$HOME"
sudo apt-get install libnuma-dev

# Unzip the downloaded file
unzip "$HOME/master.zip"

# Enter the numactl directory
cd "$HOME/numactl-master"

# Configure, build, and install numactl locally in the home directory
./autogen.sh
./configure
make install DESTDIR="$HOME"

echo "numactl installed at $HOME/numactl-master"

export PATH="$PATH:$HOME/numactl-master/"

echo "numactl has now been added to the path"

# Display information about the system's memory architecture using numactl -H
$HOME/numactl_install/bin/numactl -H > numactl.out

echo "numactl installed and information saved in numactl.out"

cd home2