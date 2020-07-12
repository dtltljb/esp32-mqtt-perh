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
//#include "driver/i2c.h"
#include "sdkconfig.h"
#include "v_i2c_htu21d.h"
#include "htu21d_i2c_sensor_task.h"

static const char *TAG = "htu21-i2c-example";

SemaphoreHandle_t print_mux = NULL;


#define _I2C_NUMBER(num) I2C_NUM_##num
#define I2C_NUMBER(num) _I2C_NUMBER(num)

#define DATA_LENGTH 32                  /*!< Data buffer length of test buffer */
#define RW_TEST_LENGTH 128               /*!< Data length for r/w test, [0,DATA_LENGTH] */
#define DELAY_TIME_BETWEEN_ITEMS_MS 1000 /*!< delay time between different test items */



#define I2C_MASTER_SCL_IO CONFIG_I2C_MASTER_SCL               /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO CONFIG_I2C_MASTER_SDA               /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUMBER(CONFIG_I2C_MASTER_PORT_NUM) /*!< I2C port number for master dev */




 void i2c_htu21d_sensor_task(void *arg)
{
    esp_err_t ret;
    float temperature;
    int16_t humidity;
    i2c_sensor_htu21d_init();
    
    int cnt = 0,failure =0;
    while (1) {

				ESP_LOGI(TAG, "TASK test cnt: %d",  cnt++);

        printf("\n********HTS221 HUMIDITY&TEMPERATURE SENSOR********\n");
        ret	=	iot_htu21d_get_measure(htu21d,HTU21D_CMD_NTTM,&temperature);
        if ( ret != ESP_OK )
        	ESP_LOGI(TAG, "temperature test failure: %d",  failure++);
        else
        	printf("temperature value is: %2.2f\n", temperature );
        printf("**************************************************\n");
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}
