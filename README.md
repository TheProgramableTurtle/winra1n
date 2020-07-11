# winra1n
Environment for running checkra1n on Windows
# ONLY USE ON A TESTING DEVICE. THIS HAS NOT YET BEEN FULLY TESTED.
## What it does
The winra1n installer installs a WSL 2 linux distro, builds and registers a custom WSL 2 kernel with USB support, and enables USB/IP in WSL. In the future it will configure libusbK drivers.

winra1n itself starts a USB/IP server on Windows that captures the connected iDevice in DFU mode, attaches the device to the USB/IP client in WSL, and runs checkra1n in command-line mode.
## Why it does this
checkra1n is currently only available on macOS and Linux systems. It is not yet available on Windows because, as it says on the checkra1n website, "[the checkra1n developers] need to write a kernel driver to support Windows".
### libusbK
The main functionality missing in the Windows drivers is the ability to perform a "USB Reset", as required by the checkm8 exploit. libusbK drivers, however, support USB Reset. checkra1n uses (as far as I can tell) libusb to access the USB stack, so libusbK should provide the necessary control.
### WSL 2
Assuming libusbK works, there still needs to be a way to run checkra1n on Windows. Linux is easier to virtualize than macOS, so winra1n uses the Linux version of checkra1n. checkra1n could potentially be run in a VM, but winra1n uses WSL 2 instead. The default WSL 2 kernel does not support USB, so winra1n builds a custom kernel (the Microsft kernel but with USB support enabled in the .config file). 
### USB/IP
To connect a device to WSL 2, the USB must be forwarded over a TCP connection as there is no built in USB passthrough yet. For this, a usbip-win server runs on the host and a usbip client in WSL. The device is detected and attached in DFU mode when winra1n starts.
## Questions
### Why WSL 2 not WSL 1?
WSL 2 uses a real Linux kernel (which can have USB support enabled) but WSL 1 does not.
### Why WSL not a VM?
VMs are slow and require bulky additional software. Soon WSL 2 will be on all up-to-date Windows 10 computers (excluding some versions), so it simply needs to be enabled. With WSL winra1n opens a terminal to run checkra1n whereas a VM opens in a window (usually), making WSL feel more native.
### Will this work?
No, but give it a go anyway if you have a spare device.
## Requirements
To build, Visual Studio 2019 and WDK.
To use, a device with support for WSL 2 (Windows 10 version 2004 or later).
## Building
Build submodules first.
### usbip-win
Follow usbip-win build instructions. Create a directory somewhere to install winra1n. Copy the output program files and driver files and place them inside a subdirectory called `bin`.
### winra1n installer
Build the `install` project using Visual Studio 2019 in debug configuration for x64. Copy the output program files and place them inside `bin` within the installation folder. Copy the `script` and `distro` folders and place them within the installation folder.
### winra1n
Build the `winra1n` project using Visual Studio 2019 in debug configuration for x64. Copy the output program files and place them inside `bin` within the installation folder.
## Using
### Linux distro
Download a rootfs Ubuntu 16.04 build and name it `distro.rootfs.tar.gz`. Place it in the `distro` folder.
### libusbK
This project does not yet configure the libusbK drivers. Use Zadig.
### Install
Open an administrator command prompt and run `install.exe`. Follow the instructions and run the program again after restarting.
### Certificates
Install the usbip-win test certificates as described in the usbip-win instructions.
### Running winra1n
Connect the iDevice and place it in DFU mode. Run `usbip list -l` from the `bin` directory and note down the busID for your device. Run `winra1n {busID}` using the busID for your device. winra1n will attach your device to WSL and run checkra1n.
## Best practice
Use the USB ports on the motherboard IO (don't use a USB hub, it won't work). Stick to the same USB port (if it works). Close other programs when running winra1n. Expect a billion errors (including bluescreens).
## Issues
Please report any issues no matter how small as it will help to improve this project.
