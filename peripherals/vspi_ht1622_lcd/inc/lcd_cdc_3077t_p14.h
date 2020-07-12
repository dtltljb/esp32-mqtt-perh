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
#ifndef LCD_CDC_3077T_P14_H_
#define LCD_CDC_3077T_P14_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include "esp_system.h"
#include "driver/spi_master.h"
#include "sdkconfig.h"

#define			PLACE_ADD_1			(uint8_t)(1)
#define			PLACE_ADD_2			(uint8_t)(5)
#define			PLACE_ADD_3			(uint8_t)(9)
#define			PLACE_ADD_4			(uint8_t)(13)
#define			PLACE_ADD_5			(uint8_t)(0)
#define			PLACE_ADD_6			(uint8_t)(4)
#define			PLACE_ADD_7			(uint8_t)(8)

#define			PLACE_ADD_8			(uint8_t)(24)
#define			PLACE_ADD_9			(uint8_t)(20)
#define			PLACE_ADD_10		(uint8_t)(18)

#define			PLACE_POWER				(uint8_t)(12)
#define			PLACE_SINGLE			(uint8_t)(14)
#define			PLACE_UNIT				(uint8_t)(27)

#define			DISPLAY_DOT				(uint8_t)(0x10)


esp_err_t	lcd_bcd_display(uint8_t *buf,uint8_t Nb);

esp_err_t	lcd_power_display(uint8_t rate);

esp_err_t	lcd_power_off_clear();
esp_err_t	lcd_power_on_disp();

void init_lcd_cdc3077(void);

#ifdef __cplusplus
}
#endif


#endif

