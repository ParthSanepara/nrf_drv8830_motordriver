/**
 * @file drv8830_driver.h
 * @author Parth Sanepara
 * @brief 
 * @version 0.1
 * @date 2024-09-05
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef DRV8830_DRIVER_H_
#define DRV8830_DRIVER_H_


#define DRV8830_CH1     0
#define DRV8830_CH2     1

/* Registers */
#define DRV8830_CONTROL_REG 0x00
#define DRV8830_FAULT_REG   0x01

/* Fault IDs */
#define DRV8830_FAULT_FAULT  0x01
#define DRV8830_FAULT_OCP    0x02
#define DRV8830_FAULT_UVLO   0x04
#define DRV8830_FAULT_UTS    0x08
#define DRV8830_FAULT_ILIMIT 0x10
#define DRV8830_FAULT_CLEAR  0x80

#define DRV8830_VSET_MAX 0x3f

#define DRV8830_VREF 1.285f

enum drv8830_mode {
	DRV8830_MODE_STANDBY = 0x00,
	DRV8830_MODE_REVERSE = 0x01,
	DRV8830_MODE_FORWARD = 0x02,
	DRV8830_MODE_BREAK = 0x03,
};


int drv8830_i2c_init(void);
int drv8830_channel_init(uint8_t channel);
int drv8830_write_register(uint8_t channel, uint8_t reg_addr, uint8_t value);
int drv8830_read_register(uint8_t channel, uint8_t reg_addr, uint8_t *value);
int drv8830_stop_motors(uint8_t channel);

/**
 * @brief Set the rotation of a motor on a specific DRV8830 channel.
 *
 * @param channel Index of the DRV8830 channel (0 for CH1, 1 for CH2).
 * @param speed  Rotation speed and direction (-127: full reverse, 0: stop, 127: full forward).
 * @return 0 on success, or an error code on failure.
 */
int drv8830_set_motor_rotation(uint8_t channel, int8_t speed, enum drv8830_mode mode);

#endif /* DRV8830_DRIVER_H_ */