# winra1n WSL 2 Kernel build script

echo Installing build dependencies...
sudo apt install build-essential flex bison libssl-dev libelf-dev libncurses-dev autoconf libudev-dev libtool -y

echo Cloning kernel...
git clone https://github.com/microsoft/WSL2-Linux-Kernel kernel

echo Building kernel...
cd ./kernel
git checkout v4.19.104
cp ../wsl.config ./.config
sudo make
sudo make modules_install

echo Done