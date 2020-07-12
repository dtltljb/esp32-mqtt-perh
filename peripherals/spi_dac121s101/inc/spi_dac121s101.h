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
#ifndef _SPI_DAC121S101_H_
#define _SPI_DAC121S101_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include "esp_system.h"
#include "driver/spi_master.h"
#include "sdkconfig.h"


/* - - - - user configure para - - - */
#define PIN_NUM_MISO 25
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  19
#define PIN_NUM_CS   5

//#define 		PIN_NUM_MOSI 		23
//#define 		PIN_NUM_CLK  		19
//#define 		PIN_NUM_CS   		5

#define			VOLTAGE_REF			3288   //3288mV
 
//- - - - public parament - - - 
extern spi_device_handle_t spi_dac121s101;

/**
 * @brief init 
 *
 * @param sensor object handle of dac121s101
 * @param 
 * @param 
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
void spi_dac_121s101_init();


/**
 * @brief set dac output registers
 *
 * @param sensor object handle of dac121s101
 * @param voltage value 
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
esp_err_t spi_dac121s101_set_out_voltage(spi_device_handle_t spi, uint16_t val);


#ifdef __cplusplus
}
#endif


#endif

