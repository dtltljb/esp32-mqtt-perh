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

#include "vspi_ht1622_lcd.h"
#include "lcd_cdc_3077t_p14.h"

#define		d_com			14
#define		d_h			14
//- - - - - public - - - - 
uint8_t		assic_buf[]={1,2,3,4,5,6,7,8,9,0};

//- - - - - private prm - - - - - 

uint8_t	display_lcd_on[]={0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

uint8_t	display_lcd_off[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};

#define		MAX_DIS_SIZE     18
//= = = = = = = = = = = = = = > > > >	  0   1     2     3   4   5     6     7   8   9   A     b    c    d    E     F    -   _    NULL
						unsigned char DIS_CODE[]={	0,	1,		2,		3,	4,	5,		6,		7,	8,	9,	10,		11,	 12,	13,	 14,	15,	 16,  17,		18};
const unsigned char LCD_MIRROR_RAM[]={0xAF,0x06,0x6D,0x4F,0xC6,0xCB,0xEB,0x0E,0xEF,0xCF,0xEE,0xE3,0x61,0x67,0xE9,0xE8,0x40,0x01,0x00};
																			/*{0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0xFE,0xDE,0xEE,0xF2,0xB8,0x76,0xF8,0xE8,0x40,0x10,0x00};*/

void init_lcd_cdc3077(void)
{
		v_spi_ht1622_init(); 
		vspi_ht1622_disable_cmd();
		vspi_ht1622_enable_cmd();
		vspi_ht1622_lcd_off_cmd();    
    vspi_ht1622_lcd_on_cmd();
    //vspi_ht1622_rc_32k_cmd();  
}

/**
 * @brief lcd reset check
 */
esp_err_t	lcd_power_on_disp(){
	esp_err_t ret = ESP_OK;
	uint8_t u8 = 0x0f;
	for(uint8_t i =0 ; i < 63; i ++ )
		ret = vspi_ht1622_display_4bit(i,&u8);
	return ret;
};

/**
 * @brief lcd_power_off_clear
 */
esp_err_t	lcd_power_off_clear(){
	esp_err_t ret = ESP_OK;
	uint8_t u8 = 0;
	for(uint8_t i =0 ; i < 63; i ++ )
		ret = vspi_ht1622_display_4bit(i,&u8);
	return ret;
};

esp_err_t	lcd_power_display(uint8_t rate)
{
	esp_err_t ret = ESP_OK;
	uint8_t u8 = 0;
	if (rate > 75)
		u8 = 0xf;
	else if( (rate <=75)&&(rate > 45) )
		u8 = 0x7;	
	else if( (rate <= 45)&&(rate > 15) )
		u8 = 0x3;	
	else
		u8 = 0x1;	
	ret = vspi_ht1622_display_4bit(PLACE_POWER,&u8);
	return ret;
}

/**
 * @brief lcd_bcd_code_display
 */
 
esp_err_t	lcd_bcd_display(uint8_t *buf,uint8_t Nb)
{
	 esp_err_t ret= ESP_OK;
	 uint8_t u8 = 0,index = 0,dot = 0;
	 for(uint8_t i = 0; i < Nb; i++)
	 {
	 	
	 	if(buf[i] >= '0' && buf[i] <= '9'){
        u8 = buf[i] - '0';
        u8 = LCD_MIRROR_RAM[u8];
        index = index + 1;
        if( dot == 1 ){
        	 u8 = u8 | DISPLAY_DOT;
        	 dot = 0;
        	}
    }
	 	else if ( buf[i] == '.' ){
	 			dot	=	1;
	 		 	continue;
		}else{
			index = index + 1;
			u8 = LCD_MIRROR_RAM[MAX_DIS_SIZE];
		}

	 	switch(index)
	 	{
	 		case 0:
	 			ret	=	vspi_ht1622_display_buf(PLACE_ADD_1,&u8);
	 			break;
	 		case 1:
	 			ret	=	vspi_ht1622_display_buf(PLACE_ADD_2,&u8);
	 			break;
	 		case 2:
	 			ret	=	vspi_ht1622_display_buf(PLACE_ADD_3,&u8);
	 			break;
	 		case 3:
	 			ret	=	vspi_ht1622_display_buf(PLACE_ADD_4,&u8);
	 			break;
	 		case 4:
	 			ret	=	vspi_ht1622_display_buf(PLACE_ADD_5,&u8);
	 			break;
	 		case 5:
	 			ret	=	vspi_ht1622_display_buf(PLACE_ADD_6,&u8);
	 			break;
	 		case 6:
	 			ret	=	vspi_ht1622_display_buf(PLACE_ADD_7,&u8);
	 			break;
			default:
				printf("%s,%d lcd display seg %d is over range..",__FILE__,__LINE__,i);
				ret = ESP_FAIL;
				break;
	 	}
	 	//		printf("LCD_MIRROR_RAM[%x] = 0x%02x \n",i,u8);
	}
	 return ret;
};
