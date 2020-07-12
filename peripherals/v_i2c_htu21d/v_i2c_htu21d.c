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
#include <stdio.h>
#include "esp_log.h"
#include "v_i2c_htu21d.h"

typedef struct {
    v_i2c_config_t bus;
    uint16_t dev_addr;
} htu21d_dev_t;

#define I2C_MASTER_SCL_IO           18          /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO           27          /*!< gpio number for I2C master data  */
#define I2C_MASTER_FREQ_HZ          100000      /*!< I2C master clock frequency */

htu21d_handle_t htu21d = NULL;
v_i2c_config_t htu21d_i2c_bus;

//htu21d_config_t	htu21_config = NULL;

htu21d_handle_t iot_htu21d_create(v_i2c_config_t bus, uint16_t dev_addr)
{
    htu21d_dev_t* sensor = (htu21d_dev_t*) calloc(1, sizeof(htu21d_dev_t));
    sensor->bus = bus;
    sensor->dev_addr = dev_addr;
    return (htu21d_handle_t) sensor;
}

esp_err_t iot_htu21d_delete(htu21d_handle_t sensor, bool del_bus)
{
    htu21d_dev_t* sens = (htu21d_dev_t*) sensor;
    free(sens);
    return ESP_OK;
}



esp_err_t iot_htu21d_soft_reset(htu21d_handle_t sensor)
{
	  esp_err_t ret;
	  htu21d_dev_t* sens = (htu21d_dev_t*) sensor;
restart:
    v_i2c_master_start();
    ret = v_i2c_master_write_byte( (uint8_t)((sens->dev_addr << 1) | WRITE_BIT ), ACK_CHECK_EN);
    while(ret == ESP_FAIL){
    	ESP_LOGI("htu21d sensor", "%s,line:%d non ack,reset i2c bus..\n",__FILE__,__LINE__);
    	v_i2c_master_stop();
    	vTaskDelay(500);
   		goto restart;
  	}
    ret = v_i2c_master_write_byte( HTU21D_CMD_RST, ACK_CHECK_EN);
    while(ret == ESP_FAIL){
    	ESP_LOGI("htu21d sensor", "%s,line:%d non ack,reset i2c bus..\n",__FILE__,__LINE__);
    	v_i2c_master_stop();
    	vTaskDelay(500);    	
    	goto restart;
  	}
    v_i2c_master_stop();
    ESP_LOGI("htu21d sensor", "%s,line:%d reset sensor okay..\n",__FILE__,__LINE__);
    return ESP_OK;
}

esp_err_t htu21d_read_write_user_register(htu21d_handle_t sensor)
{
		uint8_t rx_buf[1]={0};
		htu21d_dev_t* sens = (htu21d_dev_t*) sensor;
    v_i2c_second_start();		//v_i2c_master_start();
    v_i2c_master_write_byte( (uint8_t)((sens->dev_addr << 1) | WRITE_BIT ), ACK_CHECK_EN);
    v_i2c_master_write_byte( HTU21D_CMD_RUR, ACK_CHECK_EN);
    v_i2c_second_start();
    v_i2c_master_write_byte( (sens->dev_addr << 1) | READ_BIT, ACK_CHECK_EN);
    v_i2c_master_read_byte( &rx_buf[0], ACK_CHECK_DIS);
    v_i2c_second_start();
    v_i2c_master_write_byte( (uint8_t)((sens->dev_addr << 1) | WRITE_BIT ), ACK_CHECK_EN);
    v_i2c_master_write_byte( HTU21D_CMD_WUR, ACK_CHECK_EN);
    rx_buf[0]	=	HTU21D_DOR_BIT | HTU21D_EOH_BIT;
    v_i2c_master_write_byte( rx_buf[0], ACK_CHECK_EN);
    v_i2c_master_stop();
    ESP_LOGI("htu21d sensor", "%s,line:%d config sensor okay..\n",__FILE__,__LINE__);
    return ESP_OK;
}

/**
 * @brief v_i2c_bus htd21d initialization
 */
void i2c_sensor_htu21d_init()
{
    v_i2c_config_t conf;
    conf.mode = 0;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    
		gpio_set_direction(I2C_MASTER_SDA_IO, GPIO_MODE_INPUT);
    gpio_set_direction(I2C_MASTER_SCL_IO, GPIO_MODE_OUTPUT);
    htu21d = iot_htu21d_create(conf, HTU21D_I2C_ADDRESS);
    iot_htu21d_soft_reset(htu21d);
    vTaskDelay(15);															//less than 15ms
   // htu21d_read_write_user_register(htu21d);
}

esp_err_t iot_htu21d_get_measure(htu21d_handle_t sensor,htu21d_cmd_t order,float *value)
{
    float  f_out = 0 ;
   	uint8_t buffer[3],Cnt = 0 ;
    int32_t tmp_32;
    htu21d_dev_t* sens = (htu21d_dev_t*) sensor;
    esp_err_t ret;

    v_i2c_master_start();
    ret	=	v_i2c_master_write_byte( (sens->dev_addr << 1) | WRITE_BIT, ACK_CHECK_EN);
  	if ( ret != ESP_OK ){
  		 v_i2c_master_stop();
  		 *value	= 0 ;
  		 return ESP_FAIL;
  	}    
    ret	=	v_i2c_master_write_byte( order, ACK_CHECK_EN);		//HTU21D_CMD_HTTM
  	if ( ret != ESP_OK ){
  		 v_i2c_master_stop();
  		 *value	= 0 ;
  		 return ESP_FAIL;
  	}
  	    
    do{
    	vTaskDelay(10);					//less than 10ms ,because rtos sys ticks min value
	    v_i2c_second_start();
	    ret	=	v_i2c_master_write_byte( (sens->dev_addr << 1) | READ_BIT, ACK_CHECK_EN);
	    Cnt = Cnt + 1 ;
		}while( (ret != ESP_OK)&&(Cnt < 20 ));
		
  	ret = v_i2c_master_read_byte( &buffer[0], ACK_CHECK_EN);
  	if ( ret != ESP_OK ){
  		 v_i2c_master_stop();
  		 *value	= 0 ;
  		 return ESP_FAIL;
  	}
  	
    ret	=	v_i2c_master_read_byte( &buffer[1], ACK_CHECK_EN);   
		if ( ret != ESP_OK ){
  		 v_i2c_master_stop();
  		 *value	= 0 ;
  		 return ESP_FAIL;
  	}
  	
    v_i2c_master_read_byte( &buffer[2], ACK_CHECK_DIS);      
		v_i2c_master_stop();
		
		buffer[0] &=0xfc;														//LSB BIT0 BIT1 is 0,
		tmp_32	=	buffer[1]*256 + buffer[0];
		
		if ( order == HTU21D_CMD_HTTM)
		{
			f_out	=	tmp_32*175.72 / 65536 - 46.85 ;		//T= -46.85 + 175.72*ST/2^16
			printf("temperature = %01f \n", f_out);
		}	else if( order == HTU21D_CMD_HTHM) {
			f_out	=	tmp_32*125.0 / 65536 - 6 ;				//RH % = -6 + 125 * SRH/2^16
			printf("RH = %01f %% \n", f_out);
		} else {
			printf("order error = 0x%02x \n", order);
		}
		*value	=	f_out;
    return ESP_OK;
}



