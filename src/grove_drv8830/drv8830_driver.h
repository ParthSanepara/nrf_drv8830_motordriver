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

#define DRV8830_CONTROL_REG 0x00
#define DRV8830_FAULT_REG   0x01

int drv8830_i2c_init(void);

int drv8830_stop_motors(uint8_t channel);

/**
 * @brief Set the rotation of a motor on a specific DRV8830 channel.
 *
 * @param channel Index of the DRV8830 channel (0 for CH1, 1 for CH2).
 * @param speed  Rotation speed and direction (-127: full reverse, 0: stop, 127: full forward).
 * @return 0 on success, or an error code on failure.
 */
int drv8830_set_motor_rotation(uint8_t channel, int8_t speed);

#endif /* DRV8830_DRIVER_H_ */