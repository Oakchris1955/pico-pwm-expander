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

Currently, the Raspberry Pi Pico utilises 18 out of the 26 available GPIOs (The datasheet says 30, however it also mentions that 4 are used for internal board functions). GPIOs 0 and 1 are part of the I2C interface used by the Raspbbery Pi Pico to interface with other devices. GPIOs 2 to 17 are put into PWM Mode and can be controlled using the I2C interface.

### The I2C interface

You use the I2C protocol in order to communicate with the Raspberry Pi Pico. The pins utilised for this are GPIO 0 and 1: GPIO 0 is the Data line, while GPIO 1 is the Clock line.

Internally, the Raspberry Pi Pico uses 16 "registers" that hold 8-bit values. Each register corresponds to a GPIO pin (GPIO 2 corresponds to register 0, GPIO 3 corresponds to register 1, etc.). The 8-bit value of each register controls the duty cycle of the corresponding GPIO/PWM pin (18 means a duty cycle of 18/255\*100 = 7%, 237 meands a duty cycle of 237/255\*100 = 93%, etc.). Note that since the register sare 8-bit, their values cap at 255.

In order to change a corresponding register, you must send the register address to the Raspberry Pi Pico on write mode, followed by the value to write to the register on write mode. Keep in mind that you must first send the 7 bits of the I2C address of the Raspberry Pi Pico before all of these followed by a 1 bit, which indicates write mode (if it was a 0 bit, it would be a read request). Also, out of the register address, only the first 4 bits are read, since there are 16 PWM pins available, so that last 4 pins can be of any value. If you wanna read from a register address, you must send the register address in write mode, followed by a read mode request. Lastly, if you wanna wrtie to or read from multiple addresses in a row, you can specify the initial address by sending its bits on write mode, then either keep writing or switch to read mode and start reading from that address

## Contributing

If you wanna contribute to this project, then you can simply make a pull request. If you find something wrong about this project but don't know how to fix it, open an issue
