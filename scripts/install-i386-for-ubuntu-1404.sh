# check host architecture
dpkg --print-architecture
# check if host support multi architecutres
dpkg --print-foreign-architectures
sudo dpkg --add-architecture i386
sudo apt-get update
sudo apt-get install -y libc6:i386 libstdc++6:i386
