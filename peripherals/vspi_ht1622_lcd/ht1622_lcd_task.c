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

#include "vspi_ht1622_lcd.h"
#include "ht1622_lcd_task.h"
#include "lcd_cdc_3077t_p14.h"

static const char *TAG = "ht1622-vspi-lcd";


uint8_t	display_acsii_buf[8]={'3','.','1','4','1','5','9','2'};    

#define DELAY_TIME_BETWEEN_ITEMS_MS 1000 /*!< delay time between different test items */

void ht1622_lcd_task(void *arg)
{
   esp_err_t ret;
   int cnt = 0;
   lcd_power_off_clear();
   
    while (1) { 
				cnt	=	+1;
        ret = ESP_OK;
        ret	=	lcd_bcd_display(display_acsii_buf,sizeof(display_acsii_buf - 1 ));
        vTaskDelay(1000 / portTICK_RATE_MS);
        //sprintf(display_buf,"%d",cnt);
				
        if (ret == ESP_ERR_TIMEOUT) {
            ESP_LOGE(TAG, " Timeout");
        } else if (ret == ESP_OK) {
					 ESP_LOGI(TAG, "ht1622_lcd_task test %d times \n",cnt++);
        } else {
            ESP_LOGW(TAG, "%s: ht1622 device destroy...", esp_err_to_name(ret));
        }
        
    }
    vTaskDelete(NULL);
}
