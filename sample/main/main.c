#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"

#include "soc/timer_group_struct.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"

#include "driver/ledc.h"
#include "esp_err.h"
#include "esp_system.h"

#include "esp_wifi.h"
#include "esp_event_loop.h" 
#include "esp_log.h"

#include "nvs_flash.h"
#include "iothub_mqtt_client_sample.h"

#include "esp_smartconfig.h"
#include "iot_smartconfig.h"

#include "i2s_dac_speaker.h"

#include "adc_example_task.h"
#include "led_example_task.h"
#include "timer_group_task.h"

#include "spi_ad_tm7705.h"
#include "spi_dac121s101.h"

#include "dac121s101_spi_task.h"
#include "ad_tm7705_spi_task.h"
#include "spi_master_example_main.h"
#include "htu21d_i2c_sensor_task.h"
#include "ht1622_lcd_task.h"
#include "gpio_int_task.h"
#include "lcd_cdc_3077t_p14.h"


#define EXAMPLE_WIFI_SSID CONFIG_WIFI_SSID
#define EXAMPLE_WIFI_PASS CONFIG_WIFI_PASSWORD




/* FreeRTOS event group to signal when we are connected & ready to make a request */
static EventGroupHandle_t wifi_event_group;

/* The event group allows multiple bits for each event,
   but we only care about one event - are we connected
   to the AP with an IP? */
const int CONNECTED_BIT = BIT0;

static const char *TAG = "mqtt-iot-solution";

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        /* This is a workaround as ESP32 WiFi libs don't currently
           auto-reassociate. */
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
        break;
    default:
        break;
    }
    return ESP_OK;
}

static void initialise_wifi()
{
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_WIFI_SSID,
            .password = EXAMPLE_WIFI_PASS,
        },
    };
    
    ESP_LOGI(TAG, "Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    //ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, wc) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}

void iot_task(void *pvParameter)
{
    bool isGatewayDevice = false;
    wifi_event_group = xEventGroupCreate();

    ESP_LOGI(TAG, "Waiting for WiFi access point ...");
    //xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT,false, true, portMAX_DELAY);
	xEventGroupWaitBits(
            wifi_event_group,   /* The event group being tested. */
            CONNECTED_BIT, /* The bits within the event group to wait for. */
            pdFALSE,        /* BIT_0 & BIT_4 should be cleared before returning. */
            pdTRUE,       /* false Don't wait for both bits, either bit will do. */
            portMAX_DELAY );/* Wait a maximum of 100ms for either bit to be set. */
    ESP_LOGI(TAG, "Connected to access point success");
    iothub_mqtt_client_run();
    
    while(1){
    	vTaskDelay(1000);
    }

}


//#define 		WP_POL_O   		21

void app_main()
{
	esp_err_t res;
	gpio_set_level(Supply_En2_CONTROL, 1);   //power on control
	init_lcd_cdc3077();
	lcd_power_on_disp();
	
  nvs_flash_init();
	int16_t	voltage = 1000;
 	wifi_config_t wifi_config;
	memset(&wifi_config,0,sizeof(wifi_config));
  initialise_wifi(&wifi_config);
	//example_i2s_init();
		
  spi_dac_121s101_init();
  res = spi_dac121s101_set_out_voltage(spi_dac121s101, voltage);
  //esp_log_level_set("I2S", ESP_LOG_INFO);
  
	//xTaskCreate(&iot_task, "iot_task", 8192, NULL, 5, NULL);
	//xTaskCreate(&dac121s101_spi_task,"dac121s101_spi_task", 8192, NULL, 5, NULL);
  //xTaskCreate(example_i2s_adc_dac, "example_i2s_adc_dac", 1024 * 2, NULL, 5, NULL);
  xTaskCreate(&i2c_htu21d_sensor_task,"i2c_htu21d_sensor_task", 4096, NULL, 5, NULL);
  //xTaskCreate(&ht1622_lcd_task,"ht1622_lcd_task", 4096, NULL, 5, NULL);
	//xTaskCreate(&tm7705_spi_task,"tm7705_spi_task", 4096, NULL, 5, NULL);
	gpio_int_task();
	
		/*
    //iot_sc_setup(SC_TYPE_ESPTOUCH_AIRKISS, WIFI_MODE_STA, 0);
    iot_sc_setup(SC_TYPE_ESPTOUCH_AIRKISS, WIFI_MODE_STA, 0);
	printf("Smart config.......");
  
   xTaskCreate(smart_config_task, "smart_config_task", 1024*2, NULL, 5, NULL);
   
    while (1) {
        res = iot_sc_start(20000 / portTICK_PERIOD_MS);
        if (res == ESP_OK) {
  			printf("connected wifi...\n");
  			break;
        } else if (res == ESP_ERR_TIMEOUT) {
            printf("smart config timeout\n");
        } else if (res == ESP_FAIL) {
            printf("smart config stopped\n");
        }
    }
      */
 while (1) {
 	vTaskDelay(10000);
}
	
}
