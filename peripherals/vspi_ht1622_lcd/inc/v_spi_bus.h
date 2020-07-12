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

#ifndef _V_SPI_BUS_H_
#define _V_SPI_BUS_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <esp_types.h>
#include "esp_err.h"
#include "esp_intr_alloc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/xtensa_api.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/ringbuf.h"
#include "driver/gpio.h"


#define 		HT1622_PIN_NUM_RD 			22
#define 		HT1622_PIN_NUM_WR 			32
#define 		HT1622_PIN_NUM_DAT  		33
#define 		HT1622_PIN_NUM_CS   		21

#define		V_SPI_DATA_IO				HT1622_PIN_NUM_DAT
#define		V_SPI_WR_SCL_IO			HT1622_PIN_NUM_WR
#define		V_SPI_RD_SCL_IO			HT1622_PIN_NUM_RD
#define		V_SPI_CS_CON_IO			HT1622_PIN_NUM_CS

typedef enum{
    VSPI_CMD_RESTART = 0,   /*!< restart command */
    VSPI_CMD_WRITE	 = (5<<6),         /*!< write command */
    VSPI_CMD_READ,          /*!< read command */
    VSPI_CMD_COMMAND,       /*!< command mode */
}vspi_opmode_t;


typedef enum {
    I2C_ADDR_BIT_7 = 0,    /*!< I2C 7bit address for slave mode */
    I2C_ADDR_BIT_10,       /*!< I2C 10bit address for slave mode */
    I2C_ADDR_BIT_MAX,
} vspi_addr_mode_t;

/**
 * @brief I2C initialization parameters
 */
 
typedef struct{
    vspi_opmode_t mode;       		/*!< mode */
    gpio_num_t da_io_num;        /*!< GPIO number for data signal */
    gpio_pullup_t da_pullup_en;  
    gpio_num_t wr_io_num;        /*!< GPIO number for write signal */
    gpio_pullup_t wr_pullup_en;  
		gpio_num_t rd_io_num;        /*!< GPIO number for read signal */
    gpio_pullup_t rd_pullup_en;  
    gpio_num_t cs_io_num;        /*!< GPIO number for cs signal */
    gpio_pullup_t cs_pullup_en; 
}v_spi_config_t;

//typedef void* vspi_cmd_handle_t;    /*!< I2C command handle  */

//typedef void* vspi_bus_handle_t;

esp_err_t v_spi_reset(void);

/**
 * @brief Queue command for v spi master to write one byte to  bus
 *        @note
 *        Only call this function in I2C master mode
 *        
 *
 * @param cmd_handle I2C cmd link
 * @param data I2C one byte command to write to bus
 * @param ack_en enable ack check for master
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_ARG Parameter error
 */
esp_err_t v_spi_commad_mode( uint8_t cmd_id, uint16_t code);

/**
 * @brief Queue command for v spi master to write buffer to  bus
 *        @note
 *        Only call this function in ht1622 write mode
  * @return
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_ARG Parameter error
 */
esp_err_t v_spi_write_mode( uint16_t addr, uint8_t *data);

/**
 * @brief Queue command for v spi master to read data from bus
 *        @note
 *        Only call this function in I2C master mode
 *
 * @param cmd_handle I2C cmd link
 * @param data data buffer to accept the data from bus
 *        @note
 * @param data_len read data length
 * @param ack ack value for read command
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_ARG Parameter error
 */
esp_err_t v_spi_read_mode(uint16_t addr, uint8_t *data);

#ifdef __cplusplus
}
#endif

#endif /*_DRIVER_I2C_H_*/
