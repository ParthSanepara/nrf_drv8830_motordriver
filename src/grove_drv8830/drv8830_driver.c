/**
 * @file drv8830_driver.c
 * @author Parth Sanepara
 * @brief 
 * @version 0.1
 * @date 2024-09-05
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>

#include "drv8830_driver.h"

#define DRV8830_CHANNEL_1_NODE DT_NODELABEL(drv8830_ch1)
#define DRV8830_CHANNEL_2_NODE DT_NODELABEL(drv8830_ch2)

static const struct i2c_dt_spec i2c_dev[2] = {
    I2C_DT_SPEC_GET(DRV8830_CHANNEL_1_NODE),
    I2C_DT_SPEC_GET(DRV8830_CHANNEL_2_NODE)
};

int drv8830_i2c_init(void) {
    int ret = 0;

    for (int i = 0; i < 2; i++) {
        if (!device_is_ready(i2c_dev[i].bus)) {
            // printk("I2C bus %s is not ready!\n\r", i2c_dev[i].bus->name);
            ret = -1; // Indicate an error but continue checking the other channel
        } else {
            // printk("I2C bus %s is ready!\n\r", i2c_dev[i].bus->name);
        }
    }

    return ret;
}

/**
 * @brief Write a value to a register of a specific DRV8830 channel.
 *
 * @param channel Index of the DRV8830 channel (0 for CH1, 1 for CH2).
 * @param reg_addr Register address to write to.
 * @param value Value to write to the register.
 * @return 0 on success, or an error code on failure.
 */
int drv8830_write_register(uint8_t channel, uint8_t reg_addr, uint8_t value) {
    if (channel > 1) {
        // printk("Invalid channel: %d\n", channel);
        return -EINVAL;  // Invalid argument error
    }

    uint8_t buf[2] = {reg_addr, value};  // Buffer to hold the register address and value

    // int ret = i2c_write(i2c_dev[channel].bus, buf, sizeof(buf), i2c_dev[channel].addr);

    int ret = i2c_write_dt(&i2c_dev[channel], buf, sizeof(buf));
    if (ret < 0) {
        // printk("Failed to write to DRV8830 channel %d (I2C addr: 0x%02x), error: %d\n",
                // channel, i2c_dev[channel].addr, ret);
        return ret;
    }

    // printk("Successfully wrote 0x%02x to register 0x%02x on channel %d\n", value, reg_addr, channel);
    return 0;
}

int drv8830_channel_init(uint8_t channel)
{
    int ret = 0;

        ret = drv8830_write_register(channel, DRV8830_CONTROL_REG, DRV8830_MODE_STANDBY);
    if (ret < 0) {
        // printk("Could not initialize the control register: %d\n", ret);
        return ret;
    }

        ret = drv8830_write_register(channel, DRV8830_FAULT_REG, DRV8830_FAULT_CLEAR);
    if (ret < 0) {
        // printk("Could not clear the fault register: %d\n", ret);
        return ret;
    }

    return 0;
}

/**
 * @brief Read a value from a register of a specific DRV8830 channel.
 *
 * @param channel Index of the DRV8830 channel (0 for CH1, 1 for CH2).
 * @param reg_addr Register address to read from.
 * @param value Pointer to store the read value.
 * @return 0 on success, or an error code on failure.
 */
int drv8830_read_register(uint8_t channel, uint8_t reg_addr, uint8_t *value) {
    if (channel > 1) {
        // printk("Invalid channel: %d", channel);
        return -EINVAL;  // Invalid argument error
    }

    int ret = i2c_write_read(i2c_dev[channel].bus, i2c_dev[channel].addr,
                             &reg_addr, sizeof(reg_addr),
                             value, sizeof(*value));
    if (ret < 0) {
        // printk("Failed to read from DRV8830 channel %d (I2C addr: 0x%02x), error: %d",
                // channel, i2c_dev[channel].addr, ret);
        return ret;
    }

    // printk("Successfully read 0x%02x from register 0x%02x on channel %d", *value, reg_addr, channel);
    return 0;
}

/**
 * @brief Set the rotation of a motor on a specific DRV8830 channel.
 *
 * @param channel Index of the DRV8830 channel (0 for CH1, 1 for CH2).
 * @param speed  Rotation speed and direction (-127: full reverse, 0: stop, 127: full forward).
 * @return 0 on success, or an error code on failure.
 */
int drv8830_set_motor_rotation(uint8_t channel, int8_t speed, enum drv8830_mode mode) {
    int ret = 0;
    if (channel > 1 || speed < -127 || speed > 127) {
        // printk("Invalid arguments: channel=%d, speed=%d\n", channel, speed);
        return -EINVAL;
    }


    // uint8_t direction_bit = (speed > 0) ? 0 : 1; // 0: Forward, 1: Reverse
    uint8_t speed_value = abs(speed);           // Get the absolute speed value


    // - Speed is set for both motors simultaneously
    uint8_t control_reg_value = (speed_value << 2) | mode;


    ret = drv8830_write_register(channel, DRV8830_CONTROL_REG, control_reg_value);
    if (ret < 0) {
        // printk("Failed to set motor direction, and speed error: %d\n", ret);
        return ret;
    }

    // printk("Motor on channel %d set to speed %d\n", channel, speed);
    return 0;
}

/**
 * @brief Stop both motors on a specific DRV8830 channel.
 *
 * @param channel Index of the DRV8830 channel (0 for CH1, 1 for CH2).
 * @return 0 on success, or an error code on failure.
 */
int drv8830_stop_motors(uint8_t channel) 
{
    if (channel > 1) {
        // printk("Invalid channel: %d\n", channel);
        return -EINVAL;  // Invalid argument error
    }

    // Assuming Bit 0 controls Motor 1 and Bit 1 controls Motor 2 in the control register
    uint8_t control_reg_value = 0x00; // Set both direction bits to 0 to stop

    int ret = drv8830_write_register(channel, DRV8830_CONTROL_REG, control_reg_value);
    if (ret < 0) {
        // printk("Failed to stop motors on channel %d, error: %d\n", channel, ret);
        return ret;
    }

    // printk("Motors stopped on channel %d\n", channel);
    return 0;
}