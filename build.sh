make clean

make


rm -f /workplace/rootfs/lib/modules/4.19.94-gbe5389fd85/chardevice.ko
cp ./chardevice.ko /workplace/rootfs/lib/modules/4.19.94-gbe5389fd85

rm ./chardeviceAPP

arm-linux-gnueabihf-gcc chardeviceAPP.c -o chardeviceAPP

rm -f /workplace/rootfs/apptest/chardeviceAPP

cp ./chardeviceAPP /workplace/rootfs/apptest
