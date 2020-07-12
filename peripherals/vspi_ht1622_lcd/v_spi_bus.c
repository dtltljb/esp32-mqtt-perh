// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "esp_types.h"
#include "esp_attr.h"
#include "esp_intr.h"
#include "esp_log.h"
#include "malloc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/xtensa_api.h"
#include "freertos/task.h"
#include "freertos/ringbuf.h"
#include "soc/dport_reg.h"
#include "soc/i2c_struct.h"
#include "soc/i2c_reg.h"
#include "driver/gpio.h"
#include "driver/periph_ctrl.h"
#include "v_spi_bus.h"

static const char* V_SPI_TAG = "v_spi_bus";
#define V_SPI_CHECK(a, str, ret)  if(!(a)) {                                             \
        ESP_LOGE(V_SPI_TAG,"%s:%d (%s):%s", __FILE__, __LINE__, __FUNCTION__, str);      \
        return (ret);                                                                   \
        }

#define VSPI_ADDR_ERROR_STR             "v_spi null address error"
#define VSPI_DATA_LEN_ERR_STR           "v_spi data  length error"
#define VSPI_COMMAND_ERR_STR           "v_spi command error"


static void	us_sleep(int us){  //10us
	
	int clks,cnt=0;
	clks=us*100;								// test 2 us 
	//printf("clks=%d",clks);
	while( cnt < clks ){
			cnt = cnt+1;
			cnt = cnt+1;
			cnt = cnt+1;
			cnt = cnt+1;
			cnt = cnt+1;
			cnt = cnt+1;
			cnt = cnt+1;
			cnt = cnt+1;
		}
	//printf("cnt=%d,clks=%d\n",cnt,clks);
}

esp_err_t v_spi_reset(void)
{
	gpio_set_level(V_SPI_CS_CON_IO, 1);
	vTaskDelay(10 / portTICK_RATE_MS);
	ESP_LOGE(V_SPI_TAG,"%s:%d (%s):%s ", __FILE__, __LINE__, __FUNCTION__, "v_spi_reset..."); 
	gpio_set_level(V_SPI_CS_CON_IO, 0);
	vTaskDelay(30 / portTICK_RATE_MS);		//vTaskDelay prm less than 10 / portTICK_RATE_MS, if not delay failure.!!!
	gpio_set_level(V_SPI_CS_CON_IO, 1);
	vTaskDelay(10 / portTICK_RATE_MS);
	return ESP_OK;
}

esp_err_t v_spi_commad_mode( uint8_t cmd_id, uint16_t code)
{
	gpio_set_direction(V_SPI_DATA_IO, GPIO_MODE_OUTPUT);
	gpio_set_direction(V_SPI_WR_SCL_IO, GPIO_MODE_OUTPUT);
	gpio_set_direction(V_SPI_CS_CON_IO, GPIO_MODE_OUTPUT);
	gpio_set_level(V_SPI_CS_CON_IO, 1);
	us_sleep(1);
	gpio_set_level(V_SPI_DATA_IO, 1);
	gpio_set_level(V_SPI_WR_SCL_IO, 1);
	gpio_set_level(V_SPI_RD_SCL_IO, 1);
	uint8_t	cnt = 0;
	uint16_t	value = cmd_id;
	value	=	value << 9;
	value	|=	( code	& 0x01ff);
//	ESP_LOGE(V_SPI_TAG,"%s:%d (%s):%s 0x%02x", __FILE__, __LINE__, __FUNCTION__, "v_spi_commad_mode context:",value); 
	gpio_set_level(V_SPI_CS_CON_IO, 0);
	for( ; cnt < 12; cnt++)
	{
		gpio_set_level(V_SPI_WR_SCL_IO, 0);
		us_sleep(1);
		(value&0x0800)?gpio_set_level(V_SPI_DATA_IO, 1):gpio_set_level(V_SPI_DATA_IO, 0);
		us_sleep(1);
		gpio_set_level(V_SPI_WR_SCL_IO, 1);
		us_sleep(1);
		us_sleep(1);
		value = value << 1;
	}
	gpio_set_level(V_SPI_CS_CON_IO, 1);
	us_sleep(1);	
	gpio_set_level(V_SPI_DATA_IO, 1);
	us_sleep(1);
  return ESP_OK;
}

esp_err_t v_spi_write_mode( uint16_t addr, uint8_t *data)
{
    V_SPI_CHECK((data != NULL), VSPI_ADDR_ERROR_STR, ESP_ERR_INVALID_ARG);
		gpio_set_direction(V_SPI_DATA_IO, GPIO_MODE_OUTPUT);
		gpio_set_direction(V_SPI_WR_SCL_IO, GPIO_MODE_OUTPUT);
		gpio_set_direction(V_SPI_CS_CON_IO, GPIO_MODE_OUTPUT);
		gpio_set_level(V_SPI_CS_CON_IO, 1);
		gpio_set_level(V_SPI_WR_SCL_IO, 1);
		gpio_set_level(V_SPI_DATA_IO, 1);
		
		uint8_t	cnt = 0;
		uint32_t	send_buf;
		uint16_t	address	= 0 ;
		
		address	=	( addr & 0x01ff );
		uint8_t	value = *data&0x0f;
		send_buf	=	address << 4 ;
		send_buf	=	send_buf | value ;
		//printf("ht1622 write address = 0x%02x,data=0x%02x \n",addr,value);
		
		gpio_set_level(V_SPI_CS_CON_IO, 0);
		for( cnt=0; cnt < 13; cnt++){
			gpio_set_level(V_SPI_WR_SCL_IO, 0);
			us_sleep(1);
			(send_buf&0x1000)?gpio_set_level(V_SPI_DATA_IO, 1):gpio_set_level(V_SPI_DATA_IO, 0);
			us_sleep(1);
			gpio_set_level(V_SPI_WR_SCL_IO, 1);
			us_sleep(1);
			us_sleep(1);
			send_buf = send_buf << 1;
		}
		gpio_set_level(V_SPI_CS_CON_IO, 1);
		us_sleep(1);
		gpio_set_level(V_SPI_DATA_IO, 1);
    us_sleep(1);
    
    return ESP_OK;
}

esp_err_t v_spi_read_mode(uint16_t addr, uint8_t *data)
{
    V_SPI_CHECK((data != NULL), VSPI_ADDR_ERROR_STR, ESP_ERR_INVALID_ARG);
    gpio_set_direction(V_SPI_DATA_IO, GPIO_MODE_OUTPUT);
    gpio_set_direction(V_SPI_RD_SCL_IO, GPIO_MODE_OUTPUT);
		gpio_set_direction(V_SPI_WR_SCL_IO, GPIO_MODE_OUTPUT);
		gpio_set_direction(V_SPI_CS_CON_IO, GPIO_MODE_OUTPUT);
		gpio_set_level(V_SPI_CS_CON_IO, 1);
		gpio_set_level(V_SPI_RD_SCL_IO, 1);
		gpio_set_level(V_SPI_WR_SCL_IO, 1);
		gpio_set_level(V_SPI_DATA_IO, 1);
		
		uint8_t	cnt = 0;
		uint16_t	address	= 0 ;
		address	=	( addr & 0x01ff);
		uint8_t	value = 0;
		
		gpio_set_level(V_SPI_CS_CON_IO, 0);
		for( cnt=0; cnt < 9; cnt++)
		{
			gpio_set_level(V_SPI_WR_SCL_IO, 0);
			us_sleep(1);
			(address&0x0100)?gpio_set_level(V_SPI_DATA_IO, 1):gpio_set_level(V_SPI_DATA_IO, 0);
			us_sleep(1);
			gpio_set_level(V_SPI_WR_SCL_IO, 1);
			us_sleep(1);
			us_sleep(1);
			address = address << 1;
		}
		gpio_set_level(V_SPI_DATA_IO, 1);
		gpio_set_direction(V_SPI_DATA_IO, GPIO_MODE_INPUT);
		gpio_set_level(V_SPI_RD_SCL_IO, 0);
		us_sleep(1);
		for(cnt=0;cnt < 4; cnt++ )
		 {
		 	gpio_set_level(V_SPI_RD_SCL_IO, 1);
		 	us_sleep(1);
		 	value = value<<1 ;
		 	if(gpio_get_level(V_SPI_DATA_IO))
		 		value |=0x01;
		 	else	
		 		value=value&0xfe;
		 	gpio_set_level(V_SPI_RD_SCL_IO, 0);
		 	us_sleep(1);
		 	us_sleep(1);
		}
		*data	=	value;
		gpio_set_level(V_SPI_CS_CON_IO, 1);
		us_sleep(1);		
		gpio_set_level(V_SPI_RD_SCL_IO, 1);
		us_sleep(1);		
		gpio_set_level(V_SPI_WR_SCL_IO, 1);
		us_sleep(1);
		//ESP_LOGE(V_SPI_TAG,"%s:%d (%s):%s 0x02x", __FILE__, __LINE__, __FUNCTION__, "v_spi_successive_address_read=",value); 
    return ESP_OK;
}
