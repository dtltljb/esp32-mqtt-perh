/* i2c - Example

   For other examples please check:
   https://github.com/espressif/esp-idf/tree/master/examples

   See README.md file to get detailed usage of this example.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"

#include "esp_log.h"
#include "sdkconfig.h"
#include "spi_dac121s101.h"

static const char *TAG = "dac121s101-spi-example";
#define DELAY_TIME_BETWEEN_ITEMS_MS 1000 /*!< delay time between different test items */

void dac121s101_spi_task(void *arg)
{

    int16_t 	voltage = 255;  //debug
		int32_t		Vref	=	VOLTAGE_REF;
    esp_err_t ret;
    spi_dac_121s101_init();

    int cnt = 1;
    while (1) {

        ret = spi_dac121s101_set_out_voltage(spi_dac121s101, voltage);
        ESP_LOGI(TAG, "voltage val:%03x,cnt: %d \n",voltage,cnt++);        
        if (ret == ESP_ERR_TIMEOUT) {
            ESP_LOGE(TAG, " Timeout");
        } else if (ret == ESP_OK) {
            printf("***************************\n");
            printf("SPI MASTER WRITE ( DAC121 )\n");
            printf("****************************\n");
            printf("out voltage: %.01f [mV]\n", (Vref * voltage) / 4095.0);
        } else {
            ESP_LOGW(TAG, "%s: No ack, sensor not connected...skip...", esp_err_to_name(ret));
        }
        vTaskDelay((DELAY_TIME_BETWEEN_ITEMS_MS * 1 )/ portTICK_RATE_MS);
    }
    
    vTaskDelete(NULL);
}
