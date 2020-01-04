# check host architecture
dpkg --print-architecture
# check if host support multi architecutres
dpkg --print-foreign-architectures
sudo dpkg --add-architecture i386
sudo apt-get update
sudo apt-get install libc6:i386 libstdc++6:i386
# libz.so.1
sudo apt-get install lib32z1
