#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "driver/hw_timer.h"

#include "servo.h"

#define SERVO_PIN 2

void hw_timer_cb(void *arg)
{
	gpio_set_level(SERVO_PIN, 0);
}

void servo_init(void)
{
	gpio_config_t io_conf;

	io_conf.pull_up_en = 0;
	io_conf.pull_down_en = 0;
	io_conf.mode = GPIO_MODE_OUTPUT;
	io_conf.pin_bit_mask = 1UL << SERVO_PIN;
	io_conf.intr_type = GPIO_INTR_DISABLE;
	gpio_config(&io_conf);

	hw_timer_init(hw_timer_cb, 0);
}

void servo_pwmdc_set(int dc)
{
	gpio_set_level(SERVO_PIN, 1);
	hw_timer_alarm_us(dc, false);
}
