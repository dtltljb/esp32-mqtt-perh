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

#ifndef _V_I2C_BUS_H_
#define _V_I2C_BUS_H_


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


#define V_I2C_SCL_IO           18          /*!< gpio number for I2C master clock */
#define V_I2C_SDA_IO           27          /*!< gpio number for I2C master data  */

#define		V_I2C_MASTER_SDA_IO			V_I2C_SDA_IO
#define		V_I2C_MASTER_SCL_IO			V_I2C_SCL_IO

#define ACK_VAL        0x0               /*!< I2C ack value */
#define NACK_VAL       0x1               /*!< I2C nack value */

typedef enum{
    V_I2C_MODE_SLAVE = 0,   /*!< I2C slave mode */
    V_I2C_MODE_MASTER,      /*!< I2C master mode */
    V_I2C_MODE_MAX,
}v_i2c_mode_t;



/**
 * @brief I2C initialization parameters
 */
typedef struct{
    v_i2c_mode_t mode;       /*!< I2C mode */
    gpio_num_t sda_io_num;        /*!< GPIO number for I2C sda signal */
    gpio_pullup_t sda_pullup_en;  /*!< Internal GPIO pull mode for I2C sda signal*/
    gpio_num_t scl_io_num;        /*!< GPIO number for I2C scl signal */
    gpio_pullup_t scl_pullup_en;  /*!< Internal GPIO pull mode for I2C scl signal*/

}v_i2c_config_t;

typedef void* i2c_cmd_handle_t;    /*!< I2C command handle  */


void u_sleep(int us);

/**
 * @brief Configure GPIO signal for I2C sck and sda
 *
 * @param i2c_num I2C port number
 * @param sda_io_num GPIO number for I2C sda signal
 * @param scl_io_num GPIO number for I2C scl signal
 * @param sda_pullup_en Whether to enable the internal pullup for sda pin
 * @param scl_pullup_en Whether to enable the internal pullup for scl pin
 * @param mode I2C mode
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_ARG Parameter error
 */
 
 /*
esp_err_t i2c_set_pin(i2c_port_t i2c_num, int sda_io_num, int scl_io_num,
                      gpio_pullup_t sda_pullup_en, gpio_pullup_t scl_pullup_en, v_i2c_mode_t mode);
*/

/**
 * @brief command for I2C master to generate a start signal
 *        @note
 */
esp_err_t v_i2c_master_start(void);  //first stop single,second start single

esp_err_t v_i2c_second_start(void); //only start single

/**
 * @brief Queue command for I2C master to write one byte to I2C bus
 *        @note
 *        Only call this function in I2C master mode
 *        Call i2c_master_cmd_begin() to send all queued commands
 *
 * @param cmd_handle I2C cmd link
 * @param data I2C one byte command to write to bus
 * @param ack_en enable ack check for master
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_ARG Parameter error
 */
esp_err_t v_i2c_master_write_byte( uint8_t data, bool ack_en);

/**
 * @brief Queue command for I2C master to read one byte from I2C bus
 *        @note
 *        Only call this function in I2C master mode
 *        Call i2c_master_cmd_begin() to send all queued commands
 *
 * @param cmd_handle I2C cmd link
 * @param data pointer accept the data byte
 *        @note
 *        If the psram is enabled and intr_flag is `ESP_INTR_FLAG_IRAM`, please use the memory allocated from internal RAM.
 * @param ack ack value for read command
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_ARG Parameter error
 */
esp_err_t v_i2c_master_read_byte( uint8_t* data, bool ack);


/**
 * @brief Queue command for I2C master to generate a stop signal
 *        @note
 *        Only call this function in I2C master mode
 *        Call i2c_master_cmd_begin() to send all queued commands
 *
 * @param cmd_handle I2C cmd link
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_ARG Parameter error
 */
esp_err_t v_i2c_master_stop(void);



#ifdef __cplusplus
}
#endif

#endif /*_DRIVER_I2C_H_*/
