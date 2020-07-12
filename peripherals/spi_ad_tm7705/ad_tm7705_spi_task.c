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
#include "spi_ad_tm7705.h"
#include "ad_tm7705_spi_task.h"
#include "ht1622_lcd_task.h"


#define	d_VOLTAGE			1250000		//uV

#define DELAY_TIME_BETWEEN_ITEMS_MS 1000 /*!< delay time between different test items */

static const char *TAG = "tm7705-spi-example";

void tm7705_spi_task(void *arg)
{

    //uint16_t 	voltage = 0;  //debug
		uint64_t		Dtv	=	d_VOLTAGE;
    esp_err_t ret;
    ad_value	val;
    //uint16_t val;
    int cnt = 0;
		int chn;
		spi_bus_tm7705_init();
		TM7705_reset_and_reconfig(spi_ad_tm7705);
		    
    while (1) {

        vTaskDelay( 1000 / portTICK_RATE_MS);
        if(0 == gpio_get_level(TM7705_PIN_NUM_DRDY)){
        	vTaskDelay( 3 / portTICK_RATE_MS);
        chn	=	cnt%2;
 				ret = TM7705_read_channel(spi_ad_tm7705,chn, &val.channel_1_value);

        if (ret == ESP_ERR_TIMEOUT) {
            ESP_LOGE(TAG, " Timeout");
        } else if (ret == ESP_OK) {
            printf("***************************\n");
            printf("SPI MASTER WRITE ( tm7705 )\n");
            printf("****************************\n");
            ESP_LOGI(TAG, "channel %d value:%03x,cnt: %d ",chn, val.channel_1_value, cnt++);
            printf("channel %d input voltage: %.001f [mV]\r\n",chn, (Dtv * val.channel_1_value) /( 65535 * 1000.0) );
            sprintf((char*)display_acsii_buf,"%.01f",(Dtv * val.channel_1_value) /( 65535 * 1000.0));
        }else {
            ESP_LOGW(TAG, "%s: No ack, channel 1 sensor not connected...skip...", esp_err_to_name(ret));
        }
      }
    }
    vTaskDelete(NULL);
}
