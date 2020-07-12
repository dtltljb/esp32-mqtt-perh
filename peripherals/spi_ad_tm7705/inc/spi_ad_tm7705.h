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

#ifndef _SPI_TM7705_SPI_H_
#define _SPI_TM7705_SPI_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include "esp_system.h"
#include "driver/spi_master.h"
#include "sdkconfig.h"

//#define PIN_NUM_MISO 25
//#define PIN_NUM_MOSI 23
//#define PIN_NUM_CLK  19
//#define PIN_NUM_CS   22

/* - - - - user configure para - - - */



#define 		TM7705_PIN_NUM_MISO 		25
#define 		TM7705_PIN_NUM_MOSI 		23
#define 		TM7705_PIN_NUM_CLK  		19
#define 		TM7705_PIN_NUM_CS   		16

#define 		TM7705_PIN_NUM_DRDY   		15
#define 		TM7705_PIN_NUM_REST   		17

#define			AD_VOLTAGE_REF			3000   //3000mV


/*
*	tm7705_communication_reg 
*/

typedef enum{
    AD_TM7705_REG_COMM         = (0 << 4),  		//8bits
    AD_TM7705_REG_SETUP        = (1 << 4),  		//8bits
    AD_TM7705_REG_CLOCK        = (2 << 4), 			//8bits
    AD_TM7705_REG_DATA         = (3 << 4), 			//16bits
    AD_TM7705_REG_TEST         = (4 << 4), 			//8bits
    AD_TM7705_REG_OFFSET       = (6 << 4),  		//24bits
    AD_TM7705_REG_GAIN         = (7 << 4),  		//24bits
    
    AD_TM7705_WRITE            = (0 << 3),
    AD_TM7705_READ             = (1 << 3), 
    
    AD_TM7705_CH_1             = 0,   		 		// AIN1+  AIN1-  
    AD_TM7705_CH_2             = 1,    			// AIN2+  AIN2-  
    AD_TM7705_CH_3             = 2,    			// AIN1-  AIN1-  
    AD_TM7705_CH_4             = 3     			// AIN1-  AIN2-  

}tm7705_communication_reg;

/*
*	tm7705_control_reg 
*/
typedef enum{
	AD_TM7705_MD_NORMAL				= (0<<6),
	AD_TM7705_CAL_SELF				= (1<<6),	
	AD_TM7705_CAL_ZERO				= (2<<6),
	AD_TM7705_CAL_FULL				= (3<<6),
	
	AD_TM7705_GAIN_1_					=	(0<<3),
	AD_TM7705_GAIN_2_					=	(1<<3),
	AD_TM7705_GAIN_4_					=	(2<<3),
	AD_TM7705_GAIN_8_					=	(3<<3),
	AD_TM7705_GAIN_16_					=	(4<<3),
	AD_TM7705_GAIN_32_					=	(5<<3),
	AD_TM7705_GAIN_64_					=	(6<<3),
	AD_TM7705_GAIN_128_					=	(7<<3),	
	
	  AD_TM7705_BIPOLAR          = (0 << 2),    		//double
    AD_TM7705_UNIPOLAR         = (1 << 2),   		 //single
  
    AD_TM7705_BUF_NO           = (0 << 1),   		//
    AD_TM7705_BUF_EN           = (1 << 1),   
  
    AD_TM7705_FSYNC_0          = 0,     			//
    AD_TM7705_FSYNC_1          = 1    
	
}tm7705_control_reg;

/*
*	tm7705_clock_select_reg 
*/
typedef enum{
	AD_TM7705_B7_B5_ZERO			= (7<<5),
	
	AD_TM7705_B4_CLKDIS_1			= (1<<4),	
	AD_TM7705_B4_CLKDIS_0			= (0<<4),	
	
	AD_TM7705_B3_CLKDIV_1				= (1<<3),	
	AD_TM7705_B3_CLKDIV_0				= (0<<3),	
	
	AD_TM7705_B2_CLK_1					= (1<<2),
	AD_TM7705_B2_CLK_0					= (0<<2),

	AD_TM7705_B1_B0_FS_20			=	(0<<0),
	AD_TM7705_B1_B0_FS_25			=	(1<<0),
	AD_TM7705_B1_B0_FS_100		=	(2<<0),
	AD_TM7705_B1_B0_FS_200		=	(3<<0),
	
	AD_TM7705_B1_B0_FS_50			=	(0<<0),
	AD_TM7705_B1_B0_FS_60			=	(1<<0),
	AD_TM7705_B1_B0_FS_250		=	(2<<0),
	AD_TM7705_B1_B0_FS_500		=	(3<<0),
		
}tm7705_clock_select_reg;

/*
*	tm7705_zero_adjust_reg 
*/
typedef enum{
	AD_TM7705_ZERO_ADJUST			= 0X1F4000,
}tm7705_zero_adjust_reg;

/*
*	tm7705_full_adjust_reg 
*/
typedef enum{
	AD_TM7705_FULL_ADJUST			= 0X5761AB,
}tm7705_full_adjust_reg;

/*
*	tm7705_device_work_struct
*/
	enum{
	TM7705_MEASURE_BUZYING	=	0X01,
	TM7705_STATUS_IDLE	,
	TM7705_MEASURE_OKAY ,
	TM7705_ADJUST_ZERO_ING,
	TM7705_ADJUST_FULL_ING,
};

typedef struct {
	uint8_t		init_indication;
	uint8_t 	communication_reg;
	uint8_t 	control_reg;
	uint8_t 	clock_reg;
	uint8_t		channel_indication;
	uint8_t		work_status_indication;
	uint16_t	measure_value;
	uint16_t	ch0_value[32];
	uint16_t	ch1_value[32];
}tm7705_device_work;

typedef	struct	{
	uint16_t	channel_1_value;
	uint16_t	channel_2_value;
}ad_value;
//- - - - public parament - - - 
extern spi_device_handle_t spi_ad_tm7705;
 

/**
 * @brief init 
 *
 * @param 
 * @param 
 * @param 
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
void 	spi_bus_tm7705_init();

void TM7705_reset_and_reconfig(spi_device_handle_t spi_dev); 

/**
 * @brief get ad measure result
 *
 * @param sensor object handle of spi_ad_tm7705
 * @param voltage value 
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
esp_err_t TM7705_read_channel(spi_device_handle_t spi_dev, uint8_t channel, uint16_t *ad);


esp_err_t TM7705_read(spi_device_handle_t spi_dev, ad_value	*val);//double channel value

/**
 * @brief adjust ad zero
 *
 * @param sensor object handle of spi_ad_tm7705
 * @param output TM7705_STATUS_IDLE
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
esp_err_t spi_ad_tm7705_adjust_zero_val(spi_device_handle_t spi_dev, uint8_t channel); 

/**
 * @brief adjust ad full
 *
 * @param sensor object handle of spi_ad_tm7705
 * @param output TM7705_STATUS_IDLE
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
esp_err_t spi_ad_tm7705_adjust_full_val(spi_device_handle_t spi_dev, uint8_t channel);

/*
*	DEBUG TEST
*/

void TM7705_systemcalib_self(spi_device_handle_t spi_dev, uint8_t channel);

#ifdef __cplusplus
}
#endif


#endif

