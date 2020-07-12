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
#ifndef _VSPI_HT1622_LCD_H_
#define _VSPI_HT1622_LCD_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "esp_system.h"
#include "sdkconfig.h"
#include "v_spi_bus.h"

/* - - - - user configure para - - - */


/*
*	HT1622_COMMAND_ID_CODE
*/

typedef enum{

    HT1622_CMD_WRITE            = (5 << 6),
    HT1622_CMD_READ             = (6 << 6), 
    HT1622_RD_MO_WR							=	(5 << 6),
    HT1622_CMD_CONTROL					=	(4 << 0)

}ht1622_command_id_s;

/*
*	ht1622_control_command 
*/

typedef enum{
	
	HT1622_MD_NORMAL					= ( 0xe3 << 1 )|1,
	HT1622_SYS_DIS						=	( 0 << 1 )|1,
	HT1622_SYS_EN							=	( 1 << 1 )|1,
	HT1622_LCD_OFF						=	( 2 << 1 )|1,
	HT1622_LCD_ON							=	( 3 << 1 )|1,
	HT1622_WDT_DIS						=	( 5 << 1)|1,
	HT1622_TIMER_EN						=	( 6 << 1) | 1,
	HT1622_WDT_EN							=	( 7 << 1)|1,
	HT1622_RC_32K							=	( 0x18 << 1 )|1,
	HT1622_TONE_OFF						=	( 0x08 << 1 )|1,
	HT1622_IRQ_EN							=	( 0x88 << 1 )|1,
}ht1622_control_command_s;

//- - - - public component - - - 

typedef void* ht1622_handle_t;
extern	ht1622_handle_t	ht1622;

/**
 * @brief   init ht1622
 *
 * @param   dev object handle of ht1622
 * @param   ht16c21_conf ht16c21 configuration info
 *
 * @return
 *     - non
 *     - 
 */

void v_spi_ht1622_init(void);

/**
 * @brief   Write command or data to ht1622
 *
 * @param   dev object handle of ht1622
 * @param   ht1622  will write command
 * @param   ht1622_handle_t command dev
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
esp_err_t vspi_ht1622_normal_cmd(void);
esp_err_t vspi_ht1622_disable_cmd(void);
esp_err_t vspi_ht1622_enable_cmd(void);
esp_err_t vspi_ht1622_lcd_off_cmd(void);
esp_err_t vspi_ht1622_lcd_on_cmd(void);
esp_err_t vspi_ht1622_rc_32k_cmd(void);
esp_err_t vspi_ht1622_tone_off_cmd(void);
esp_err_t vspi_ht1622_wdt_en_cmd(void);
esp_err_t vspi_ht1622_wdt_dis_cmd(void);

/**
 * @brief   Write RAM or data to ht1622
 *
 * @param   dev object handle of ht1622
 * @param   address RAM address
 * @param   *buf data value pointer
 * @param   *buf data value len
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
esp_err_t vspi_ht1622_display_buf( uint16_t address, uint8_t *buf);

esp_err_t vspi_ht1622_display_4bit( uint16_t address, uint8_t *buf);

/**
 * @brief   Read RAM or data to ht1622
 *
 * @param   dev object handle of ht1622
 * @param   address RAM address
 * @param   *buf data value pointer
 * @param   *buf data value len
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
esp_err_t vspi_ht1622_ram_read( uint16_t address, uint8_t *buf);




#ifdef __cplusplus
}
#endif


#endif

