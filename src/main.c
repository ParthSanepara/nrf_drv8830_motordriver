#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/shell/shell.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>

#include "main.h"
#include "drv8830_driver.h"

// Define the motor run time in seconds
#define MOTOR_RUN_TIME_SEC 5 
#define MOTOR_SPEED	255  // 0-255

// Define button nodes
#define SW0_NODE DT_ALIAS(sw0)
#define SW1_NODE DT_ALIAS(sw1) 

// Define button GPIO specs
static const struct gpio_dt_spec button0 = GPIO_DT_SPEC_GET(SW0_NODE, gpios);
static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET(SW1_NODE, gpios);

// Define button callback data
static struct gpio_callback button0_cb_data;
static struct gpio_callback button1_cb_data;

// Flags for button presses and motor state
static bool button0_pressed_flag = false;
static bool button1_pressed_flag = false;
static bool motor_running = false;

// Button 0 callback function
void button0_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    // printk("Button 0 pressed \n");
    button0_pressed_flag = true;

}

// Button 1 callback function
void button1_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    // printk("Button 1 pressed \n");
    button1_pressed_flag = true;
}
int main(void)
{
    int ret;

    // Initialize button 0
    if (!device_is_ready(button0.port)) {
		return -1;
	}
    ret = gpio_pin_configure_dt(&button0, GPIO_INPUT);
	if (ret < 0) {
		return -1;
	}
    ret = gpio_pin_interrupt_configure_dt(&button0, GPIO_INT_EDGE_TO_ACTIVE);
    gpio_init_callback(&button0_cb_data, button0_pressed, BIT(button0.pin));
    gpio_add_callback(button0.port, &button0_cb_data);

    // Initialize button 1
    if (!device_is_ready(button1.port)) {
		return -1;
	}
    ret = gpio_pin_configure_dt(&button1, GPIO_INPUT);
	if (ret < 0) {
		return -1;
	}
    ret = gpio_pin_interrupt_configure_dt(&button1, GPIO_INT_EDGE_TO_ACTIVE);
    gpio_init_callback(&button1_cb_data, button1_pressed, BIT(button1.pin));
    gpio_add_callback(button1.port, &button1_cb_data);

    // Motor Driver Init
    drv8830_i2c_init();
    drv8830_channel_init(DRV8830_CH1);
    drv8830_channel_init(DRV8830_CH2);


    while (1)
    {
        // Check for button presses and handle motor logic
        if (button0_pressed_flag && !motor_running) {
            // printk("Button 0 pressed - Motor Reverse\n");
            drv8830_set_motor_rotation(DRV8830_CH1, MOTOR_SPEED, DRV8830_MODE_REVERSE);
            motor_running = true;
            k_sleep(K_SECONDS(MOTOR_RUN_TIME_SEC));
            drv8830_stop_motors(DRV8830_CH1);
            motor_running = false;
            // printk("Motor stopped.\n");
            button0_pressed_flag = false; // Reset the flag
            button1_pressed_flag = false;
        } else if (button1_pressed_flag && !motor_running) {
            // printk("Button 1 pressed - Motor Forward\n");
            drv8830_set_motor_rotation(DRV8830_CH1, MOTOR_SPEED, DRV8830_MODE_FORWARD);
            motor_running = true;
            k_sleep(K_SECONDS(MOTOR_RUN_TIME_SEC));
            drv8830_stop_motors(DRV8830_CH1);
            motor_running = false;
            // printk("Motor stopped.\n");
            button1_pressed_flag = false; // Reset the flag
            button0_pressed_flag = false;
        }

        k_sleep(K_MSEC(10)); // Short sleep to avoid busy waiting
    }
    
    return 0;
}

// Function to handle the 'forward' command
static int cmd_motor_forward(const struct shell *sh, size_t argc, char **argv)
{
    // Assuming DRV8830_CH1 is the desired channel and 127 is the desired speed
    drv8830_set_motor_rotation(DRV8830_CH1, MOTOR_SPEED, DRV8830_MODE_FORWARD);
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
    drv8830_set_motor_rotation(DRV8830_CH1, MOTOR_SPEED, DRV8830_MODE_REVERSE);
    // shell_print(sh, "Rotating motor reverse for 5 seconds...");
    k_sleep(K_SECONDS(MOTOR_RUN_TIME_SEC)); // Wait for 5 seconds
    drv8830_stop_motors(DRV8830_CH1);
    // shell_print(sh, "Motor stopped.");

    return 0;
}

// Creating root commands 'forward' and 'reverse'
SHELL_CMD_REGISTER(1, NULL, "Rotate motor forward for 5 seconds.", cmd_motor_forward);
SHELL_CMD_REGISTER(0, NULL, "Rotate motor reverse for 5 seconds.", cmd_motor_reverse);
