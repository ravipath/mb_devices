#/bin/bash

REQUIRED_PKGS="autoconf automake libtool"
for PKG in $REQUIRED_PKGS; do
    PKG_OK=$(dpkg-query -W --showformat='${Status}\n' $PKG|grep "install ok installed")
    echo Checking for $PKG: $PKG_OK
    if [ "" = "$PKG_OK" ]; then
    echo "No $PKG. Setting up $PKG."
    sudo apt-get --yes install $PKG
    fi
done

LOCALDIR="libmodbus"
REPO="https://github.com/stephane/libmodbus.git"
cd ..
mkdir $LOCALDIR
echo "----------------------"
echo "Downloading LIBMODBUS"
echo "----------------------"
git clone $REPO $LOCALDIR
echo "------------------------------"
echo "LIBMODBUS download complete"
echo "------------------------------"
cd $LOCALDIR
source autogen.sh
./configure
sudo make install
sudo cp /usr/local/lib/libmodbus.* /usr/lib
cd $HOME

echo "------------------------"
echo "Cloning MB_DEVICES REPO"
echo "------------------------"

MB_DEVICE_DIR="mb_device"
REPO_MB_DEVICE="https://github.com/ravipath/mb_devices.git"
echo "------------------------"
echo "MB_DEVICES cloning done"
echo "------------------------"
mkdir $MB_DEVICE_DIR
git clone $REPO_MB_DEVICE $MB_DEVICE_DIR
cd $MB_DEVICE_DIR
git checkout mb_device_serial_nomapandthread
echo "------------------------"
echo "Building MB_DEVICES"
echo "------------------------"
gcc -o mb_device_rtu mb_device_rtu.c mb_sim.c -I /usr/local/include/modbus -lmodbus -lpthread
echo "------------------------"
echo "MB_DEVICES building done"
echo "------------------------"
echo "RUN"

./mb_device_rtu


