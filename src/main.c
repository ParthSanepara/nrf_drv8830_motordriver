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
        printk("Hello World!\n");

        return 0;
}
