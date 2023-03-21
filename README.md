# Pico PWM Expander
A project written using the Raspberry Pi Pico C SDK that turns your Raspberry Pico into a 16-pin PWM expander. Communication between master and slave happens with the I2C communication protocol.

## Installation

### Preparation

0) Buy yourself a Raspberry Pi Pico. The whole project is based around, so nothing can be done without it
1) Get the "GNU Embedded Toolchain for Arm". There are several ways of doing this, depending on the platform. For Linux, install it by running the following commands: `sudo apt update` to update the APT registry, then `sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential` to install the toolchain. For other platforms (MacOS, MS Windows), check the "Getting started with Raspberry Pi Pico" document [here](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf)
2) Clone the repository with `git clone https://github.com/Oakchris1955/pico-pwm-expander.git`
3) `cd` to the cloned directory and initiate the `pico-sdk` submodule by typing `git submodule init`, then `git submodule update`

### Building from source

4) Create a new directory using the `mkdir` command and name it `build` (`mkdir build`
5) `cd` to the `build` directory and type `export PICO_SDK_PATH=../pico-sdk`. This way, CMake will know the path of the Pico C SDK (Disclaimer: you might have to rerun this if you close your terminal before running the next step) 
6) Let CMake know the path of the Pico C SDK by typing `cmake ..`
7) Finally, building the binaries by typing `make` in the terminal (this could take several minutes if you are running this command the first time

Once you are done, you should see that the `build` directory contains some files and folders. From all of them, we only need `main.uf2`

### Injecting UF2 binary into the Raspberry Pi Pico

8) Insert you Raspberry Pi Pico into the USB port of your computer. If you are using a brand new one, you should see it show up as an ejectable drive. If not, unplug it, hold the BOOTSEL and plug it again while still holding the BOOTSEL button
9) Drag the UF2 binary into the drive that just showed up. It should disconnect from your computer immediately. This means that you have successfully injected the U2F binary into the Raspberry Pi Pico


## Usage

**TODO**

## Contributing

If you wanna contribute to this project, then you can simply make a pull request. If you find something wrong about this project but don't know how to fix it, open an issue
