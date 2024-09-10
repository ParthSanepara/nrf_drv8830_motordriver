#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/shell/shell.h>

#include "main.h"
#include "drv8830_driver.h"


// Define the motor run time in seconds
#define MOTOR_RUN_TIME_SEC 5 


int main(void)
{
    // Motor Driver Init
    drv8830_i2c_init();
    drv8830_channel_init(DRV8830_CH1);
    drv8830_channel_init(DRV8830_CH2);

    return 0;
}

// Function to handle the 'forward' command
static int cmd_motor_forward(const struct shell *sh, size_t argc, char **argv)
{
    // Assuming DRV8830_CH1 is the desired channel and 127 is the desired speed
    drv8830_set_motor_rotation(DRV8830_CH1, 127, DRV8830_MODE_FORWARD);
    // shell_print(sh, "Rotating motor forward for 5 seconds...");
    k_sleep(K_SECONDS(MOTOR_RUN_TIME_SEC)); // Wait for 5 seconds
    drv8830_stop_motors(DRV8830_CH1);
    // shell_print(sh, "Motor stopped.");

    return 0;
}

// Function to handle the 'reverse' command
static int cmd_motor_reverse(const struct shell *sh, size_t argc, char **argv)
{
    // Assuming DRV8830_CH1 is the desired channel and 127 is the desired speed
    drv8830_set_motor_rotation(DRV8830_CH1, 127, DRV8830_MODE_REVERSE);
    // shell_print(sh, "Rotating motor reverse for 5 seconds...");
    k_sleep(K_SECONDS(MOTOR_RUN_TIME_SEC)); // Wait for 5 seconds
    drv8830_stop_motors(DRV8830_CH1);
    // shell_print(sh, "Motor stopped.");

    return 0;
}

// Creating root commands 'forward' and 'reverse'
SHELL_CMD_REGISTER(1, NULL, "Rotate motor forward for 5 seconds.", cmd_motor_forward);
SHELL_CMD_REGISTER(0, NULL, "Rotate motor reverse for 5 seconds.", cmd_motor_reverse);