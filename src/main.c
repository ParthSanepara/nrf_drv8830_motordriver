/**
 * @file main.c
 * @author Parth Sanepara
 * @brief 
 * @version 0.1
 * @date 2024-09-05
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "main.h"
#include "drv8830_driver.h"



int main(void)
{
    printk("Hello World! %s\n", CONFIG_BOARD);

    // Motor Driver Init
    drv8830_i2c_init();
    drv8830_channel_init(DRV8830_CH1);
    // drv8830_channel_init(DRV8830_CH2);


    while (1)
    {
        /* code */
        drv8830_set_motor_rotation(DRV8830_CH1, 127, DRV8830_MODE_FORWARD);
        k_msleep(5000);
        drv8830_set_motor_rotation(DRV8830_CH1, 127, DRV8830_MODE_REVERSE);
        k_msleep(5000);
        drv8830_stop_motors(DRV8830_CH1);
        k_msleep(5000);
        
    }
        

    return 0;
}
