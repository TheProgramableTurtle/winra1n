# winra1n USB/IP installation script

echo Loading USB/IP module...
sudo modprobe vhci-hcd

echo Testing USB/IP...
usbip port

echo Done