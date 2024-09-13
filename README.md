# nrf_drv8830_motordriver


This Project provides a Zephyr driver for the Grove - Mini I2C Motor Driver v1.0, designed for use with nRF53 & nRF91 series development boards.
[Device driver build for Grove - Mini I2C Motor Driver v1.0](https://wiki.seeedstudio.com/Grove-Mini_I2C_Motor_Driver_v1.0/)

## Features

* Control the speed and direction of two DC motors.
* Simple API for Controll both channel with direction.

## Hardware Requirements

* nRF5 development board (e.g., nRF9160-DK, nRF5340-DK)
* Grove - Mini I2C Motor Driver v1.0
* Two DC motors

## Pin Configuration

The following table shows the pin configuration for the Grove - Mini I2C Motor Driver v1.0 and the corresponding pins on the nRF9160-DK and nRF5340-DK boards:

| Driver Module | nRF9160-DK | nRF5340-DK |
|--------------|------------|------------|
| SDA          | P0.30      | P1.02      |
| SCL          | P0.31      | P1.03      |


## Software Requirements

*  nRF Connect SDK V2.6.1
*  nRF Connect SDK V2.7.0

## Build Code
### nRF9160-DK
* `nrf9160dk_nrf9160_ns` board
### nRF5340-DK
* `nrf5340dk_nrf5340_cpuapp` board





