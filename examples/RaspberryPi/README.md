# Capture  demo usage
This is a simple demo to show how to use the capture feature of Arducam Mega with Raspberry Pi.
## Install dependence lib
- Install libgpiod
```shell
sudo apt-get install libgpiod-dev
```
## How to build
```shell
cd capture
make
```
## Execute
```shell
./Arducam_Mega
```
## Clean
```shell
make clean
```
# Preview demo usage
This is a simple demo to show how to use the preview feature of Arducam Mega with Raspberry Pi.
## Install dependence lib
- Install libgpiod
```shell
sudo apt-get install libgpiod-dev
```
## How to build
```shell
cd preview
mkdir build
cd build && cmake ..
```
## Execute
```shell
./preview
```
## Clean
```shell
make clean
```

