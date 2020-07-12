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
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"		//vTaskDelay
#include "driver/gpio.h"
#include "spi_ad_tm7705.h"

//= = = = = = = = =  /\ = 
#define PIN_NUM_debug_tm7705
#define TM7705_TRANS_BIT		(8)
//= = = =  = = = = = \/ =

spi_device_handle_t spi_ad_tm7705;




/**
 * @brief spi master initialization
 */

void spi_bus_tm7705_init()
{
    esp_err_t ret;
    //Initialize non-SPI GPIOs
    gpio_set_direction(TM7705_PIN_NUM_DRDY, GPIO_MODE_INPUT);
    gpio_set_direction(TM7705_PIN_NUM_REST, GPIO_MODE_OUTPUT);
   
    
    spi_bus_config_t buscfg={
        .miso_io_num=TM7705_PIN_NUM_MISO,
        .mosi_io_num=TM7705_PIN_NUM_MOSI,
        .sclk_io_num=TM7705_PIN_NUM_CLK,
        .quadwp_io_num=-1,
        .quadhd_io_num=-1,
        .max_transfer_sz=8
    };

    spi_device_interface_config_t devcfg={
        .clock_speed_hz=1*1000*1000,           //Clock out at 1 MHz
        .mode	=	2,                       			//SPI mode 2
        .spics_io_num=TM7705_PIN_NUM_CS,      //CS pin
        .queue_size=7,                        //We want to be able to queue 7 transactions at a time
        .pre_cb		=	NULL,  										//Specify pre-transfer callback to handle D/C line
        //.post_cb	=	dac_spi_post_transfer_callback
    };
    //Initialize the SPI bus
    ret=spi_bus_initialize(VSPI_HOST, &buscfg, 0);			//HSPI_HOST
    ESP_ERROR_CHECK(ret);
    //Attach the ad_tm7705 to the SPI bus
    ret=spi_bus_add_device(VSPI_HOST, &devcfg, &spi_ad_tm7705);
    ESP_ERROR_CHECK(ret);
}
/*
*********************************************************************************************************
*	  : TM7705_wait_DRDY
*	:  
*	    : 
*	  : 
*********************************************************************************************************
*/
static int TM7705_wait_DRDY(void)  
{  
    int i = 0;  
    uint32_t time_cnt = 500;  		//1s	
  
    for (i=0; i<time_cnt; i++)  
    {  
        if (0 == gpio_get_level(TM7705_PIN_NUM_DRDY)){
            break;  
        }
        vTaskDelay(2 / portTICK_RATE_MS);
    }

    if (i >= time_cnt){
				printf("[%s][%d]: tm7705_wait_DRDY Time Out ...\r\n", __FUNCTION__, __LINE__);
        return -1;  
    }
    return 0;  
}

/*
*********************************************************************************************************
*	  : TM7705_sync_spi
*	: AD7705SPI
*	    : 
*	  : 
*********************************************************************************************************
*/

static void TM7705_sync_spi(spi_device_handle_t spi_dev)
{
  uint8_t tx_buf[4] = {0xFF,0xFF,0xFF,0xFF};
  uint32_t	ret;
  
	// 32AD7705DIN"1" 
	if( spi_dev == NULL){
		    printf("[%s][%d]\r\n", __FUNCTION__, __LINE__);
		    return	;
	}
	spi_transaction_t t;
    memset(&t, 0, sizeof(t));       				//Zero out the transaction
    t.length=sizeof(tx_buf)*TM7705_TRANS_BIT;        		//Command is 8 bits
    t.tx_buffer=(void*)tx_buf;            			//The data is the cmd itself
    t.user=(void*)1;                   			// set to 1 flag
    ret=spi_device_polling_transmit(spi_dev, &t);  //Transmit!
    assert(ret==ESP_OK);           				 //Should have had no issues.
    printf("[%s][%d]\r\n", __FUNCTION__, __LINE__);
    return ;
}

/*
*********************************************************************************************************
*	  : TM7705_reset
*	: 
*	    : 
*	  : 
*********************************************************************************************************
*/ 

static void TM7705_reset(void)  
{
	printf("[%s][%d]\r\n", __FUNCTION__, __LINE__);

    gpio_set_level(TM7705_PIN_NUM_REST, 1);
    vTaskDelay(20 / portTICK_RATE_MS);
    gpio_set_level(TM7705_PIN_NUM_REST, 0);
    vTaskDelay(10 / portTICK_RATE_MS);
    gpio_set_level(TM7705_PIN_NUM_REST, 1);
    vTaskDelay(2 / portTICK_RATE_MS);
 //   return ;
}
/*
*********************************************************************************************************
*	  : TM7705_config_channel
*	: TM7705  
*	    : channel : ADC12
*	  : 
*********************************************************************************************************
*/  
static esp_err_t TM7705_config_channel(spi_device_handle_t spi_dev, uint8_t channel)  
{  
    uint8_t tx_buf[2] = {0,0}; 
    uint32_t	ret;
    tx_buf[0] = AD_TM7705_REG_CLOCK | AD_TM7705_WRITE | channel;  	// clock reg 0x20(chn0) or 0x21(chn1)
    tx_buf[1] = AD_TM7705_B4_CLKDIS_0 | AD_TM7705_B3_CLKDIV_1 | AD_TM7705_B2_CLK_0 | AD_TM7705_B1_B0_FS_50;  
    	//config (0x08), enbale clk out(4.9152MHz), filter output 20Hz

  	spi_transaction_t t;
    memset(&t, 0, sizeof(t));       										//Zero out the transaction
    t.length=sizeof(tx_buf)*TM7705_TRANS_BIT;          //Command is 8 bits
    t.tx_buffer=(void*)tx_buf;            						//The data is the cmd itself
    t.user=(void*)0;                   								// needs to be set to 0
    ret=spi_device_polling_transmit(spi_dev, &t);  						//Transmit!
    assert(ret==ESP_OK);            									//Should have had no issues.
    printf("[%s][%d]: Write CLOCK Register:MSB:%02X,LSB:%02x\r\n", __FUNCTION__, __LINE__, tx_buf[0], tx_buf[1]); 
    vTaskDelay(2 / portTICK_RATE_MS);
    
    memset(tx_buf,0,sizeof(tx_buf));
		tx_buf[0] = AD_TM7705_REG_SETUP | AD_TM7705_WRITE | channel;  										// write setup reg :0x10(chn0) or 11(chn1)
    tx_buf[1] = AD_TM7705_MD_NORMAL | AD_TM7705_GAIN_1_ | AD_TM7705_UNIPOLAR | AD_TM7705_BUF_EN | AD_TM7705_FSYNC_0;  	
    //config channelgain 1,UNIPOLAR mode,BUF_EN,FSYNC,result = 0x03 
 		memset(&t, 0, sizeof(t));       										//Zero out the transaction
    t.length=sizeof(tx_buf)*TM7705_TRANS_BIT;          //Command is 8 bits
    t.tx_buffer=(void*)tx_buf;            						//The data is the cmd itself
    t.user=(void*)0;                   								// needs to be set to 0
    ret=spi_device_polling_transmit(spi_dev, &t);  						//Transmit!
    assert(ret==ESP_OK);            									//Should have had no issues.
    printf("[%s][%d]: Write SETUP Register:MSB:%02X,LSB:%02x\r\n", __FUNCTION__, __LINE__, tx_buf[0], tx_buf[1]);     
    return ret;
}
/*
*********************************************************************************************************
*	  : TM7705_systemcalib_self
*	: . AIN+ AIN-0Vref  
*			   180ms
*	    :  channel : ADC12
*	  : 
*********************************************************************************************************
*/
void TM7705_systemcalib_self(spi_device_handle_t spi_dev, uint8_t channel)  
{
    int8_t tx_buf[2] = {0,0};   
    int8_t rx_buf[1] = {0};
  	uint32_t	ret; 

  	tx_buf[0] = AD_TM7705_REG_CLOCK | AD_TM7705_WRITE | channel;  	// clock reg 0x20(chn0) or 0x21(chn1)
    tx_buf[1] = AD_TM7705_B4_CLKDIS_0 | AD_TM7705_B3_CLKDIV_1 | AD_TM7705_B2_CLK_0 | AD_TM7705_B1_B0_FS_50;  
    	//config (0x08), enbale clk out(4.9152MHz), filter output 20Hz

  	spi_transaction_t t;
    memset(&t, 0, sizeof(t));       										//Zero out the transaction
    t.length=sizeof(tx_buf)*TM7705_TRANS_BIT;          //Command is 8 bits
    t.tx_buffer=(void*)tx_buf;            						//The data is the cmd itself
    t.user=(void*)0;                   								// needs to be set to 0
    ret=spi_device_polling_transmit(spi_dev, &t);  						//Transmit!
    assert(ret==ESP_OK);            									//Should have had no issues.
    printf("[%s][%d]: Write CLOCK Register:MSB:%02X,LSB:%02x\r\n", __FUNCTION__, __LINE__, tx_buf[0], tx_buf[1]); 
  	
    tx_buf[0] = AD_TM7705_REG_SETUP | AD_TM7705_WRITE | channel;  	//  0x11
    tx_buf[1] = AD_TM7705_CAL_SELF | AD_TM7705_GAIN_1_ | AD_TM7705_UNIPOLAR | AD_TM7705_BUF_EN | AD_TM7705_FSYNC_0; 
		//channel(0x46)10
	
//   	spi_transaction_t t;
    memset(&t, 0, sizeof(t));       													//Zero out the transaction
    t.length=sizeof(tx_buf)*TM7705_TRANS_BIT;                 //Command is 8 bits
    t.tx_buffer=(void*)tx_buf;            										//The data is the cmd itself
    t.user=(void*)0;                   												// set to 0
    ret=spi_device_polling_transmit(spi_dev, &t);  						//Transmit!
    assert(ret==ESP_OK);            													//Should have had no issues.
	 
	  vTaskDelay(250 / portTICK_RATE_MS);												/*  --- 180ms*/  
	  printf("[%s][%d]: Write SETUP Register16: %x,%x\r\n", __FUNCTION__, __LINE__, tx_buf[0], tx_buf[1]); 
/**********************************debug use******************************************/
	TM7705_wait_DRDY(); 
	memset(rx_buf, 0, sizeof(rx_buf));
	
  tx_buf[0] = AD_TM7705_REG_SETUP | AD_TM7705_READ | channel;  	//(0x46)  ,   
  
   //	spi_transaction_t t;
    memset(&t, 0, sizeof(t));       				//Zero out the transaction
    t.length= (1 + 1)*TM7705_TRANS_BIT ;                 //Command is 8 bits
    t.tx_buffer=&tx_buf[0];            			//The data is the cmd itself
    t.rx_buffer=&rx_buf[0];
    t.user=(void*)0;                   			// needs to be set to 0
    ret=spi_device_polling_transmit(spi_dev, &t);  			//Transmit!
    assert(ret==ESP_OK);            				//Should have had no issues.
    
	if(rx_buf[1] < 0)
	{
		printf("[%s][%d]: Read SETUP Register faild.\r\n", __FUNCTION__, __LINE__);
	}else{
		printf("[%s][%d]: Read SETUP Register: 0x%x\r\n", __FUNCTION__, __LINE__, rx_buf[1]); 
	}
	
	TM7705_wait_DRDY();	 
	memset(rx_buf, 0, sizeof(rx_buf));
  tx_buf[0] = AD_TM7705_REG_CLOCK | AD_TM7705_READ | channel;  		//(0x8)  ,   
  
   //	spi_transaction_t t;
    memset(&t, 0, sizeof(t));       								//Zero out the transaction
    t.length= (1 + 1)*TM7705_TRANS_BIT ;    				//Command is 8 bits
    t.tx_buffer=&tx_buf[0];            							//The data is the cmd itself
    t.rx_buffer=&rx_buf[0];
    t.user=(void*)0;                   							// needs to be set to 0
    ret=spi_device_polling_transmit(spi_dev, &t);  			//Transmit!
    assert(ret==ESP_OK);            								//Should have had no issues.
    
	if(rx_buf[1] < 0)
	{
		printf("[%s][%d]: Read SETUP Register faild.\r\n", __FUNCTION__, __LINE__);
	}else{
		printf("[%s][%d]: Read SETUP Register: 0x%x\r\n", __FUNCTION__, __LINE__, rx_buf[1]); 
	}
/***********************************************************************************************************/	 
    return ;  
}


/*
*********************************************************************************************************
*	  : TM7705_calibZero_self,  
*	: . AIN+ AIN-
*			  TM7705_ReadReg(REG_ZERO_CH1)   TM7705_ReadReg(REG_ZERO_CH2) 
*	    : channel : ADC12
*	  : 
*********************************************************************************************************
*/
esp_err_t spi_ad_tm7705_adjust_zero_val(spi_device_handle_t spi_dev, uint8_t channel) 
{
    uint8_t tx_buf[2] = {0,0}; 
    uint32_t	ret;
		printf("[%s][%d],tm7705 adjust zero \r\n", __FUNCTION__, __LINE__);

    tx_buf[0] = AD_TM7705_REG_SETUP | AD_TM7705_WRITE | channel;  	//  
    tx_buf[1] = AD_TM7705_CAL_ZERO | AD_TM7705_GAIN_1_ | AD_TM7705_UNIPOLAR | AD_TM7705_BUF_EN | AD_TM7705_FSYNC_0;  	//channel10
    
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));       				//Zero out the transaction
    t.length=sizeof(tx_buf)*TM7705_TRANS_BIT;                 //Command is 8 bits
    t.tx_buffer=&tx_buf[0];            			//The data is the cmd itself
    t.user=(void*)0;                   			// needs to be set to 0
    ret=spi_device_polling_transmit(spi_dev, &t);  			//Transmit!
    assert(ret==ESP_OK);            				//Should have had no issues.
    vTaskDelay(200 / portTICK_RATE_MS);
    return ret;
}

/*
*********************************************************************************************************
*	  : TM7705_calibFull_self,  
*	: . AIN+ AIN-
*			  TM7705_ReadReg(REG_FULL_CH1)   TM7705_ReadReg(REG_FULL_CH2) 
*	    :  channel : ADC12
*	  : 
*********************************************************************************************************
*/
esp_err_t spi_ad_tm7705_adjust_full_val(spi_device_handle_t spi_dev, uint8_t channel)  
{  
    uint8_t tx_buf[2] = {0};  
    uint32_t	ret;
		printf("[%s][%d]\r\n", __FUNCTION__, __LINE__);
	 
    tx_buf[0] = AD_TM7705_REG_SETUP | AD_TM7705_WRITE | channel;  	//  
    tx_buf[1] = AD_TM7705_CAL_FULL | AD_TM7705_GAIN_1_ | AD_TM7705_UNIPOLAR | AD_TM7705_BUF_EN | AD_TM7705_FSYNC_0;  //channel(0x46)10
    
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));       				//Zero out the transaction
    t.length=sizeof(tx_buf)*TM7705_TRANS_BIT;                 //Command is 8 bits
    t.tx_buffer=&tx_buf[0];            			//The data is the cmd itself
    t.user=(void*)0;                   			// needs to be set to 0
    ret=spi_device_polling_transmit(spi_dev, &t);  			//Transmit!
    assert(ret==ESP_OK);            				//Should have had no issues.
    vTaskDelay(200 / portTICK_RATE_MS);
    return ret;
}


/*
*********************************************************************************************************
*	  : AD7705_read_channel
*	: AD7705  
*	    : 
*	  : 
*********************************************************************************************************
*/  

esp_err_t TM7705_read_channel(spi_device_handle_t spi_dev, uint8_t channel, uint16_t *ad)  
{
    int ret = -1;   
    uint16_t value = 0xffff;  
    uint8_t tx_buf[1] = {0};   
    uint8_t rx_buf[2] = {0,0};  
   
//	  ret = TM7705_wait_DRDY();  									// 
//	  if ( ret != 0 )
//	  	return ESP_FAIL;
	 
    tx_buf[0] = AD_TM7705_REG_DATA | AD_TM7705_READ | channel;  	//(0x38) 
    
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));       																							//Zero out the transaction
    t.length= (sizeof(tx_buf) + sizeof(rx_buf))*TM7705_TRANS_BIT;                 			//Command is 8 bits
    t.tx_buffer=&tx_buf[0];            																						//The data is the cmd itself
    t.rx_buffer=&rx_buf[0];
    t.user=(void*)0;                   																						// needs to be set to 0
    ret=spi_device_polling_transmit(spi_dev, &t);  																//Transmit! 
    assert(ret==ESP_OK);            																							//Should have had no issues.
    //value = (rx_buf[0]<<8) | rx_buf[1];  note:read value postion!!!!
		value = (rx_buf[1]<<8) | rx_buf[2];
    if (ret < 0)    
    {
        printf( "[%s][%d]: AD7705_read_byte() faild. ret=%d \n", __FUNCTION__, __LINE__, ret);  
        goto fail;  
    }
	 
	if (0xffff == value)  															//, // AD77050xfff  
	{      
    printf("Error: [%s][%d]: value = 0xffff \n", __FUNCTION__, __LINE__);
		ret = ESP_FAIL; 
		goto fail;
	}
    *ad = value;   
fail:
		//printf("[%s][%d]: Read value Register: msb:%x,lsb:%x,other:%x\r\n", __FUNCTION__, __LINE__, rx_buf[0],rx_buf[1],rx_buf[2]); 
    return ret;
}

/*
*********************************************************************************************************
*	  : AD7705_read_
*	: AD7705  
*	    : 
*	  : 
*********************************************************************************************************
*/  

esp_err_t TM7705_read(spi_device_handle_t spi_dev, ad_value	*val)  
{
    int ret = -1;   
    uint16_t value = 0xffff;  
    uint8_t tx_buf[1] = {0};   
    uint8_t rx_buf[2] = {0,0};  
   
//	  ret = TM7705_wait_DRDY();  									// 
//	  if ( ret != 0 )
//	  	return ESP_FAIL;
	 
    tx_buf[0] = AD_TM7705_REG_DATA | AD_TM7705_READ | AD_TM7705_CH_1;  	//(0x38) 
    
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));       																							//Zero out the transaction
    t.length= (sizeof(tx_buf) + sizeof(rx_buf))*TM7705_TRANS_BIT;                 			//Command is 8 bits
    t.tx_buffer=&tx_buf[0];            																						//The data is the cmd itself
    t.rx_buffer=&rx_buf[0];
    t.user=(void*)0;                   																						// needs to be set to 0
    ret=spi_device_polling_transmit(spi_dev, &t);  																//Transmit! 
    assert(ret==ESP_OK);            																							//Should have had no issues.
    //value = (rx_buf[0]<<8) | rx_buf[1];  note:read value postion!!!!
    value = (rx_buf[1]<<8) | rx_buf[2];
    if (ret < 0)    
    {
        printf( "[%s][%d]: AD7705_read_byte() faild. ret=%d \n", __FUNCTION__, __LINE__, ret);  
        goto fail;  
    }
	 
	if (0xffff == value)  															//, // AD77050xfff  
	{      
    printf("Error: [%s][%d]: value = 0xffff \n", __FUNCTION__, __LINE__);
		ret = ESP_FAIL; 
		goto fail;
	}
  val->channel_1_value = value;   
  
     tx_buf[0] = AD_TM7705_REG_DATA | AD_TM7705_READ | AD_TM7705_CH_2;  	//(0x38) 
    
   //spi_transaction_t t;
    memset(&t, 0, sizeof(t));       																							//Zero out the transaction
    t.length= (sizeof(tx_buf) + sizeof(rx_buf))*TM7705_TRANS_BIT;                 			//Command is 8 bits
    t.tx_buffer=&tx_buf[0];            																						//The data is the cmd itself
    t.rx_buffer=&rx_buf[0];
    t.user=(void*)0;                   																						// needs to be set to 0
    ret=spi_device_polling_transmit(spi_dev, &t);  																//Transmit! 
    assert(ret==ESP_OK);            																							//Should have had no issues.
    //value = (rx_buf[0]<<8) | rx_buf[1];  note:read value postion!!!!
    value = (rx_buf[1]<<8) | rx_buf[2];
    if (ret < 0)    
    {
        printf( "[%s][%d]: AD7705_read_byte() faild. ret=%d \n", __FUNCTION__, __LINE__, ret);  
        goto fail;  
    }
	 
	if (0xffff == value)  															//, // AD77050xfff  
	{      
    printf("Error: [%s][%d]: value = 0xffff \n", __FUNCTION__, __LINE__);
		ret = ESP_FAIL; 
		goto fail;
	}
  val->channel_2_value = value;   

fail:
		//printf("[%s][%d]: Read value Register: msb:%x,lsb:%x,other:%x\r\n", __FUNCTION__, __LINE__, rx_buf[0],rx_buf[1],rx_buf[2]); 
    return ret;
}

/*
*********************************************************************************************************
*	  : AD7705_config_channel
*	: AD7705, AD7705  
*	    : 
*	  : 
*********************************************************************************************************
*/  
 void TM7705_reset_and_reconfig(spi_device_handle_t spi_dev)  
{
		TM7705_reset();
		TM7705_sync_spi(spi_dev);
  	TM7705_systemcalib_self(spi_dev, AD_TM7705_CH_1);		
    vTaskDelay(10 / portTICK_RATE_MS);
  	TM7705_systemcalib_self(spi_dev, AD_TM7705_CH_2);		
    vTaskDelay(10 / portTICK_RATE_MS);
    TM7705_config_channel(spi_dev, AD_TM7705_CH_1);   
    vTaskDelay(100 / portTICK_RATE_MS);
    TM7705_config_channel(spi_dev, AD_TM7705_CH_2);      
    vTaskDelay(100 / portTICK_RATE_MS);
    return ;
}
 
