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

#include "v_i2c_bus.h"

static const char* I2C_TAG = "v_i2c";
#define I2C_CHECK(a, str, ret)  if(!(a)) {                                             \
        ESP_LOGE(I2C_TAG,"%s:%d (%s):%s", __FILE__, __LINE__, __FUNCTION__, str);      \
        return (ret);                                                                   \
        }

#define I2C_DRIVER_ERR_STR             "i2c driver install error"
#define I2C_DRIVER_MALLOC_ERR_STR      "i2c driver malloc error"
#define I2C_NUM_ERROR_STR              "i2c number error"
#define I2C_TIMEING_VAL_ERR_STR        "i2c timing value error"
#define I2C_ADDR_ERROR_STR             "i2c null address error"
#define I2C_DRIVER_NOT_INSTALL_ERR_STR "i2c driver not installed"
#define I2C_SLAVE_BUFFER_LEN_ERR_STR   "i2c buffer size too small for slave mode"
#define I2C_EVT_QUEUE_ERR_STR          "i2c evt queue error"
#define I2C_SEM_ERR_STR                "i2c semaphore error"
#define I2C_BUF_ERR_STR                "i2c ringbuffer error"
#define I2C_MASTER_MODE_ERR_STR        "Only allowed in master mode"
#define I2C_MODE_SLAVE_ERR_STR         "Only allowed in slave mode"
#define I2C_CMD_MALLOC_ERR_STR         "i2c command link malloc error"
#define I2C_TRANS_MODE_ERR_STR         "i2c trans mode error"
#define I2C_MODE_ERR_STR               "i2c mode error"
#define I2C_SDA_IO_ERR_STR             "sda gpio number error"
#define I2C_SCL_IO_ERR_STR             "scl gpio number error"
#define I2C_CMD_LINK_INIT_ERR_STR      "i2c command link error"
#define I2C_GPIO_PULLUP_ERR_STR        "this i2c pin does not support internal pull-up"
#define I2C_ACK_TYPE_ERR_STR           "i2c ack type error"
#define I2C_DATA_LEN_ERR_STR           "i2c data read length error"
#define I2C_PSRAM_BUFFER_WARN_STR      "Using buffer allocated from psram"


//--- -----
static void	us_sleep(void){
	
	int clks,cnt=0;
	clks=1*100;								// test 1 us 
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
}

void u_sleep(int us)
{
	int cnt = 0;
	for(;cnt < us;cnt++)
		us_sleep();
}

esp_err_t v_i2c_master_start(void)
{
//		gpio_set_pull_mode(V_I2C_MASTER_SCL_IO, GPIO_PULLUP_ONLY);			//must be peripherals pull up 4.7kR
//		gpio_set_pull_mode(V_I2C_MASTER_SDA_IO, GPIO_PULLUP_ONLY);
		gpio_set_level(V_I2C_MASTER_SDA_IO, 0);
		us_sleep();
		us_sleep();
		gpio_set_direction(V_I2C_MASTER_SDA_IO, GPIO_MODE_OUTPUT);		
		gpio_set_level(V_I2C_MASTER_SCL_IO, 1);
		gpio_set_direction(V_I2C_MASTER_SCL_IO, GPIO_MODE_OUTPUT);
		us_sleep();
		us_sleep();
		us_sleep();		
		gpio_set_level(V_I2C_MASTER_SDA_IO, 1);		//stop
		us_sleep();
		us_sleep();
		us_sleep();
		us_sleep();		
		gpio_set_level(V_I2C_MASTER_SDA_IO, 0);		//start
		us_sleep();
		us_sleep();
		us_sleep();
		us_sleep();		
		gpio_set_level(V_I2C_MASTER_SCL_IO, 0);
		us_sleep();		
		us_sleep();
		us_sleep();
		us_sleep();
		return ESP_OK;
}

esp_err_t v_i2c_second_start(void)
{
//		gpio_set_level(V_I2C_MASTER_SCL_IO, 0);
//		u_sleep(1);
		gpio_set_level(V_I2C_MASTER_SDA_IO, 0);  //start
		gpio_set_direction(V_I2C_MASTER_SDA_IO, GPIO_MODE_OUTPUT);
		us_sleep();
		us_sleep();
		us_sleep();
		us_sleep();
		us_sleep();
		us_sleep();
    gpio_set_level(V_I2C_MASTER_SCL_IO, 0);
		us_sleep();
		us_sleep();
		us_sleep();     
		us_sleep();
		us_sleep();        
		us_sleep();
		return ESP_OK;
}

esp_err_t v_i2c_master_stop(void)
{
		gpio_set_level(V_I2C_MASTER_SCL_IO, 0);
		us_sleep();
		gpio_set_level(V_I2C_MASTER_SDA_IO, 0);
		us_sleep();
		gpio_set_level(V_I2C_MASTER_SCL_IO, 1);
		us_sleep();
		us_sleep();
		us_sleep();
		us_sleep();
		us_sleep();
		gpio_set_level(V_I2C_MASTER_SDA_IO, 1);			//stop
		us_sleep();
		us_sleep();
		us_sleep();
		us_sleep();
		us_sleep();
		return ESP_OK;
}

esp_err_t v_i2c_master_write_byte( uint8_t data, bool ack_en)
{
		 uint8_t	cnt = 0;
		 for( ;cnt < 8; cnt++ ){
		 	gpio_set_level(V_I2C_MASTER_SCL_IO, 0);
		us_sleep();
		 	(data&0x80)?gpio_set_level(V_I2C_MASTER_SDA_IO, 1):gpio_set_level(V_I2C_MASTER_SDA_IO, 0);
		us_sleep();
		 	gpio_set_level(V_I2C_MASTER_SCL_IO, 1);
		us_sleep();
			data = data<<1 ;
		us_sleep();
		}
		gpio_set_level(V_I2C_MASTER_SCL_IO, 0);
		us_sleep();
		if ( ack_en != 1 ){
			//gpio_set_direction(V_I2C_MASTER_SDA_IO, GPIO_MODE_OUTPUT);
			ESP_LOGE("V_I2C_BUS","%s:%d,%s", __FILE__, __LINE__, "write command check ack disabled"); 
		 	return ESP_OK;
		 	}else{
				gpio_set_direction(V_I2C_MASTER_SDA_IO, GPIO_MODE_INPUT);
		    gpio_set_level(V_I2C_MASTER_SDA_IO, 1);
		 		gpio_set_level(V_I2C_MASTER_SCL_IO, 1);
		us_sleep();
		 		cnt = 0;
		 		while( (cnt < 20 )&&(gpio_get_level(V_I2C_MASTER_SDA_IO)!= ACK_VAL) ){
		 			cnt = cnt + 1;
		 		}
				gpio_set_level(V_I2C_MASTER_SCL_IO, 0);
		us_sleep();
		 		gpio_set_level(V_I2C_MASTER_SDA_IO, 0);
		 		gpio_set_direction(V_I2C_MASTER_SDA_IO, GPIO_MODE_OUTPUT);
		 		ESP_LOGE("V_I2C_BUS","%s:%d,%s %d times", __FILE__, __LINE__, "v_i2c_write check ack: ",cnt); 
		 		if( cnt >= 20 )
		 			return ESP_FAIL;
		 		else
		 			return ESP_OK;
		}
}

esp_err_t v_i2c_master_read_byte( uint8_t* data, bool ack)
{
	 uint8_t	cnt = 0;uint8_t	val8 = 0;
	 gpio_set_level(V_I2C_MASTER_SCL_IO, 0);
	us_sleep();
	 gpio_set_level(V_I2C_MASTER_SDA_IO, 1);
	 gpio_set_direction(V_I2C_MASTER_SDA_IO, GPIO_MODE_INPUT);
	us_sleep();
		 for( ;cnt < 8; cnt++ )
		 {
		 	gpio_set_level(V_I2C_MASTER_SCL_IO, 1);
		us_sleep();
		 	val8 = val8>>1 ;
		 	if(gpio_get_level(V_I2C_MASTER_SDA_IO))
		 		val8 |=0x01;
		 	else
		 		val8=val8&0xfe;
		us_sleep();
		 	gpio_set_level(V_I2C_MASTER_SCL_IO, 0);
		us_sleep();
		us_sleep();
		}
		*data	=	val8;
		if ( ack != 1 ){
				gpio_set_level(V_I2C_MASTER_SDA_IO, 0);
				gpio_set_direction(V_I2C_MASTER_SDA_IO, GPIO_MODE_OUTPUT);
	 			//gpio_set_level(V_I2C_MASTER_SCL_IO, 1);
	 			//u_sleep(1);
	 			ESP_LOGE("V_I2C_BUS","%s:%d,%s", __FILE__, __LINE__, "read command check ack disabled"); 
		 		return ESP_OK;
		 	}
		else{
		 		gpio_set_level(V_I2C_MASTER_SCL_IO, 1);
		us_sleep();
		 		gpio_set_level(V_I2C_MASTER_SCL_IO, 0);
		us_sleep();	 		
		 		cnt = 0;
		 		while( (cnt < 20 )&&(gpio_get_level(V_I2C_MASTER_SDA_IO)!=ACK_VAL) ){
		 			cnt = cnt + 1;
		 		}
		 		gpio_set_level(V_I2C_MASTER_SDA_IO, 0);
		 		gpio_set_direction(V_I2C_MASTER_SDA_IO, GPIO_MODE_OUTPUT);
		 		ESP_LOGE("V_I2C_BUS","%s:%d,%s %d times", __FILE__, __LINE__, "v_i2c_read check ack: ",cnt); 
		 		if( cnt >= 20 )
		 			return ESP_FAIL;
		 		else
		 			return ESP_OK;
		}
}

esp_err_t v_i2c_master_write(uint8_t* data, size_t data_len, bool ack_en)
{
    I2C_CHECK((data != NULL), I2C_ADDR_ERROR_STR, ESP_ERR_INVALID_ARG);
    
    I2C_CHECK(data_len > 0, I2C_DATA_LEN_ERR_STR, ESP_ERR_INVALID_ARG);
    ESP_LOGE(I2C_TAG,"%s:%d (%s):%s", __FILE__, __LINE__, __FUNCTION__, "non rewrite do not use"); 
    return ESP_FAIL;
}

esp_err_t v_i2c_master_read(uint8_t* data, size_t data_len, bool ack)
{
    I2C_CHECK((data != NULL), I2C_ADDR_ERROR_STR, ESP_ERR_INVALID_ARG);
    
    I2C_CHECK(data_len > 0, I2C_DATA_LEN_ERR_STR, ESP_ERR_INVALID_ARG);
		ESP_LOGE(I2C_TAG,"%s:%d (%s):%s", __FILE__, __LINE__, __FUNCTION__, "non rewrite do not use"); 
    return ESP_FAIL;
}


esp_err_t v_i2c_set_pin(int sda_io_num, int scl_io_num, gpio_pullup_t sda_pullup_en, gpio_pullup_t scl_pullup_en)
{
   ESP_LOGE(I2C_TAG,"%s:%d (%s):%s", __FILE__, __LINE__, __FUNCTION__, "non rewrite do not use"); 
    return ESP_FAIL;
}

