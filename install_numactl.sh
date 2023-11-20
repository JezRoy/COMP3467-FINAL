# FROM: https://github.com/numactl/numactl/archive/master.zip

directory = "/home/"
#!/bin/bash

# Download the numactl source code from GitHub
wget https://github.com/numactl/numactl/archive/master.zip -d "$directory"
sudo apt-get install libnuma-dev

# Unzip the downloaded file
unzip "$directory/master.zip"

# Enter the numactl directory
cd "$directory/numactl-master"

# Configure, build, and install numactl locally in the home directory
./autogen.sh
./configure
make install DESTDIR="$directory"

echo "numactl installed at $directory/numactl-master"

export PATH="$PATH:$directory/numactl-master/"

echo "numactl has now been added to the path"

# Display information about the system's memory architecture using numactl -H
$HOME/numactl_install/bin/numactl -H > numactl.out

echo "numactl installed and information saved in numactl.out"

cd home2