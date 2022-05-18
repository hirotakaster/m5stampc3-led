#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "led_strip.h"

void setcolor(led_strip_t *led, rgb_t *color) {
    led_strip_set_pixel(led, 0, *color);
    led_strip_flush(led);
    led_strip_wait(led, 5 * 1000);
    vTaskDelay(10 / portTICK_PERIOD_MS);
}

void app_main(void) {

    // setup led strip
    led_strip_t ledstrip;

    ledstrip.is_rgbw = true;
    ledstrip.length = 1;
    ledstrip.gpio = GPIO_NUM_2;
    ledstrip.channel = RMT_CHANNEL_0;
    ledstrip.type = LED_STRIP_SK6812;

    led_strip_install();
    led_strip_init(&ledstrip);

    // set color r->g-b
    rgb_t color;
    color.r = color.g = color.b = 0;
    int vec = 1;
    int val = 0;
    int stg = 0;
    while (1)  {
        val += vec;
        if (val >= 255 || val <= 0) vec *= -1;
        switch (stg)  {
        case 0:
            color.r = val;
            if (val == 0) stg = 1;
            break;
        case 1:
            color.g = val;
            if (val == 0) stg = 2;
            break;
        case 2:
            color.b = val;
            if (val == 0) stg = 0;
            break;
        default:
            break;
        }
        setcolor(&ledstrip, &color);
    }
    esp_restart();
}

