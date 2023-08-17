# Teltonika IoT Cloud Daemon (RutOS Ready)

This project allows the user to launch the program as a daemon process through a web interface that was written in `Lua` using OpenWrt's `LuCI` web interface tool. The program is implemented in C using Tuya IoT Core SDK. Upon launching the program starts sending memory usage data to Tuya cloud. The memory information is collected using `UBUS` inter-process communication (IPC) system. 

## Requirements:

* Any router with OpenWrt or (RutOS).
* OpenWRT (RutOS) build system.
* Tuya account with a new device for development.

## OpenWRT (RutOS) build system:

* Download and update sources:
```bash
git clone https://git.openwrt.org/openwrt/openwrt.git
cd openwrt
git pull
```
* Update the feeds, select firmware and build your system:
```bash
./scripts/feeds update -a
./scripts/feeds install -a
make menuconfig
make
```

## Tuya account setup:

Follow this [guide](https://github.com/tuya/tuya-iot-core-sdk) on how to setup an account and a device for development.

## Usage:

* Specify the package in `make menuconfig`:
```bash
cd openwrt
make menuconfig
```
* You need to find your package and while it is selected you press Y and then save. The package will be specified.

![menuconfig](menuconfig.png)

* Then you compile the packages:
```bash
make package/<package_name>/compile
```
* If you encounter errors - compile with V=s flag for extra verbosity:
```bash
make package/<package_name>/compile V=s
```
* The resulting ipk will be in the `bin/targets/[target]/[subtarget]/packages` directory.
* To install the file onto the router you will need to copy the package and install it.
* Connect your router to your computer via ethernet cable and run:
```bash
scp <package_name_post_compilation>.ipk root@192.168.0.1:/tmp
```
Or (Depending on what is your router's default IP address):
```bash
scp <package_name_post_compilation>.ipk root@192.168.1.1:/tmp
```
If your router uses a different IP address - use that.

* Installing the package on your router:
```bash
ssh root@192.168.0.1
```
Or (Depending on what is your router's default IP address):
```bash
ssh root@192.168.1.1
```
If your router uses a different IP address - use that.

* After connecting to the router:
```bash
cd /tmp
opkg install <package_name_post_compilation>.ipk
```
To force dependencies or reinstall you can add these flags `--force-depends --force-reinstall` after `opkg install`. Something like:
```bash
opkg install --force-depends --force-reinstall <package_name_post_compilation>.ipk
```

* Lauch the program through a web interface:

In the address bar type in you router's IP address. Navigate to Tuya IoT service, enable the program and input your parameters. After applying changes the daemon will launch and start sending memory usage data to the Tuya cloud.

![Tuya](Tuya.png)