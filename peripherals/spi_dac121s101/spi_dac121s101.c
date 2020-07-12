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
#include <string.h>
//#include "soc/gpio_struct.h"
#include "driver/gpio.h"
#include "spi_dac121s101.h"

//= = = = = = = = =  /\ = 
#define PIN_NUM_debug		
//= = = =  = = = = = \/ =


 spi_device_handle_t spi_dac121s101;


//This function is called (in irq context!) just before a transmission starts. It will
//set io status indicated flag.
//void dac_spi_pre_transfer_callback(spi_transaction_t *t)
//{
//    int dc=(int)t->user;
//    gpio_set_direction(PIN_NUM_debug, GPIO_MODE_OUTPUT);
//    gpio_set_level(PIN_NUM_debug, 0);
//}

//This function is called (in irq context!) just before a transmission starts. It will
//set io status indicated flag.
//void dac_spi_post_transfer_callback(spi_transaction_t *t)
//{
//    int dc=(int)t->user;
//    gpio_set_direction(PIN_NUM_debug, GPIO_MODE_OUTPUT);
//    gpio_set_level(PIN_NUM_debug, 1);
//}

/**
 * @brief spi master initialization
 */
#define		nBits			8
void spi_dac_121s101_init()
{
    esp_err_t ret;
   
    spi_bus_config_t buscfg={
        .miso_io_num=-1,
        .mosi_io_num=PIN_NUM_MOSI,
        .sclk_io_num=PIN_NUM_CLK,
        .quadwp_io_num=-1,
        .quadhd_io_num=-1,
        .max_transfer_sz=8
    };
    
    spi_device_interface_config_t devcfg={

        .clock_speed_hz=4*1000*1000,           	//Clock out at 10 MHz
        .mode	=	2,                       				//SPI mode 2
        .spics_io_num=PIN_NUM_CS,               //CS pin
        .queue_size=7,                          //We want to be able to queue 7 transactions at a time
        .pre_cb		=	NULL,  													//Specify pre-transfer callback to handle D/C line
        //.post_cb	=	dac_spi_post_transfer_callback
    };
    //Initialize the SPI bus,third para = 0 ,non user dma
    ret=spi_bus_initialize(HSPI_HOST, &buscfg, 0);									//HSPI_HOST
    ESP_ERROR_CHECK(ret);
    //Attach the dac121s101 to the SPI bus
    ret=spi_bus_add_device(HSPI_HOST, &devcfg, &spi_dac121s101);
    ESP_ERROR_CHECK(ret);
}

#define		DAC121S101_CODE		(0x1fff)
#define		DAC121S101_PDx_11		(1<<12)

esp_err_t spi_dac121s101_set_out_voltage(spi_device_handle_t spi, uint16_t val)
{
	  esp_err_t ret;
	  uint8_t buf[8]={0,0};
	  spi_transaction_t t;
    if( spi == NULL){
    	return	ESP_FAIL;
    }
		uint16_t	value = val;
		value		=	value & DAC121S101_CODE;
		buf[0] 	= (uint8_t)(value >> 8);	//MSB
		buf[1]	= (uint8_t)value;					//LSB
		
		
		memset(&t, 0, sizeof(t));       //Zero out the transaction
    t.length=nBits*sizeof(val);       //Command is 8 bits * send data bytes length
    t.tx_buffer=(void*)buf;      			//send data context
    t.user=(void*)1;                						// < User-defined variable. Can be used to store eg transaction ID.
    ret=spi_device_polling_transmit(spi, &t);  //poll Transmit!
    //ret=spi_device_transmit(spi, &t);  				//event Transmit!
    assert(ret==ESP_OK);            //Should have had no issues.
    printf("%s line:%d value:%03x \n",__FILE__, __LINE__,value);
    return ret;
}



