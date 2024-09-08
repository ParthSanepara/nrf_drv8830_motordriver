cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(i2c_scanner)

target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/grove_drv8830/drv8830_driver.c)


include_directories(src/)
include_directories(src/grove_drv8830/)