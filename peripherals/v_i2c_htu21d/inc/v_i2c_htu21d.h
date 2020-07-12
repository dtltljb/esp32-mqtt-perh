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
#ifndef _IOT_HTU21D_H_
#define _IOT_HTU21D_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "v_i2c_bus.h"
#include "sdkconfig.h"

#define WRITE_BIT      0  /*!< I2C master write */
#define READ_BIT       1   /*!< I2C master read */
#define ACK_CHECK_EN   0x1               /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS  0x0               /*!< I2C master will not check ack from slave */


/**
* @brief  State enable. 
*/
typedef enum {
    HTU21D_DISABLE    = 0x00, 
    HTU21D_ENABLE     = 0x01
} htu21d_state_t; 

typedef enum {
    HTU21D_CMD_HTTM 	= 0xE3, 		/*!< Trigger Temperature Measurement,Hold master */
    HTU21D_CMD_HTHM 	= 0xE5, 		/*!< Trigger Humidity Measurement ,Hold master */
    
    HTU21D_CMD_NTTM 	= 0xF3, 		/*!< Trigger Temperature Measurement,No Hold master*/
    HTU21D_CMD_NTHM 	= 0xF5, 		/*!< Trigger Humidity Measurement ,No Hold master */
    
    HTU21D_CMD_WUR		= 0xE6, 	/*!< Write user register */
    HTU21D_CMD_RUR   	= 0xE7,   /*!< Read user register*/
    HTU21D_CMD_RST		=	0xFE,		/*!< Soft Reset */
} htu21d_cmd_t;

/**
* @brief  Bitfield positioning.
*/
#define HTU21D_BIT(x)    ((uint8_t)x)


/**
* @brief user register 1.
* \  Read = 0xE7 / write = 0xE6

* |------------------------------------------------------------------------|
*	|	BIT7 BIT0		|		Measurement resolution				  |			Default   			 |
  |	0			0			|		RH 12BIT   TEMP 14BIT						|				00						 |
  |	0			1			|		RH	8BIT	 TEMP 12							|											 |
  |	1 		0			|		RH 10BIT	 TEMP	13							|											 |
  | 1     1     |   RH 11BIT   TEMP 11BIT						|											 |
  --------------------------------------------------------------------------
* BIT6  Status: End of Battery    '0':VDD>2.25V , '1':VDD<2.25V ,Default '0'
* 5:3   Reserved.
* BIT2	Enable on-chip heater , '0' 
* BIT1  Disable OTP reload, '1'
*  This status bit is updated after each measurement,Reserved bits must not be changed.
*  OTP reload active loads default settings after each time a measurement command is issued.
*
* \endcode
*/


#define HTU21D_CTRL_REG1      	((uint8_t)0x01)

#define HTU21D_RESO_BIT7        (HTU21D_BIT(7))
#define HTU21D_RESO_BIT0        (HTU21D_BIT(0))
#define HTU21D_EOB_BIT         	(HTU21D_BIT(6))
#define HTU21D_EOH_BIT        	(HTU21D_BIT(2))
#define HTU21D_DOR_BIT       		(HTU21D_BIT(1))

/**
* @brief  HTU21D Init structure definition. 
*/   
typedef struct
{
  htu21d_state_t        			workStatus;           	
	htu21d_cmd_t								workCmd;
	uint8_t											controlReg;
}htu21d_config_t;


/**
* @brief  I2C address.
*/
#define HTU21D_I2C_ADDRESS    ((uint8_t)0x40)


/**
* @brief  Humidity data (LSB).
* \code
* Read
* Default value: 0x00.
* HOUT7 - HOUT0: Humidity data LSB (2's complement).
* \endcode
*/
#define HTU21D_HR_OUT_L_REG        ((uint8_t)0x01)

#define HTU21D_HR_OUT_MASK        ((uint8_t)0x03)

/**
* @brief  Humidity data (MSB).
* \code
* Read
* Default value: 0x00.
* HOUT15 - HOUT8: Humidity data MSB (2's complement).    
* \endcode
*/ 
#define HTU21D_HR_OUT_H_REG        ((uint8_t)0x01)

/**
* @brief  Temperature data (LSB).
* \code
* Read
* Default value: 0x00.
* TOUT7 - TOUT0: temperature data LSB. 
* \endcode
*/
#define HTU21D_TEMP_OUT_L_REG       ((uint8_t)0x01)

/**
* @brief  Temperature data (MSB).
* \code
* Read
* Default value: 0x00.
* TOUT15 - TOUT8: temperature data MSB. 
* \endcode
*/
#define HTU21D_TEMP_OUT_H_REG       ((uint8_t)0x01)

/*

extern 	i2c_config_t htu21d_i2c_bus ;

extern 	htu21d_config_t	htu21_config;
*/

typedef void* htu21d_handle_t;
extern 	htu21d_handle_t htu21d;


/**
 * @brief init HTU21D
 *
 * @param sensor object handle of htu21d
 * @param 
 * @param 
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
void i2c_sensor_htu21d_init();


/**
 * @brief Set HTU21D as power down mode
 *
 * @param sensor object handle of htu21d
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
esp_err_t iot_htu21d_soft_reset(htu21d_handle_t sensor);

/**
 * @brief Set HTU21D as power down mode
 *
 * @param sensor object handle of htu21d
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
esp_err_t htu21d_read_write_user_register(htu21d_handle_t sensor);


/**
 * @brief Read HTU21D temperature output registers, and calculate temperature
 *
 * @param sensor object handle of htu21d
 * @param temperature pointer to the returned temperature value that must be divided by 10 to get the value in ['C]
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
esp_err_t iot_htu21d_get_measure(htu21d_handle_t sensor,htu21d_cmd_t order,float *value);

/**
 * @brief Create and init sensor object and return a sensor handle
 *
 * @param bus I2C bus object handle
 * @param dev_addr I2C device address of sensor
 *
 * @return
 *     - NULL Fail
 *     - Others Success
 */
htu21d_handle_t iot_htu21d_create(v_i2c_config_t bus, uint16_t dev_addr);

/**
 * @brief Delete and release a sensor object
 *
 * @param sensor object handle of htu21d
 * @param del_bus Whether to delete the I2C bus
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
esp_err_t iot_htu21d_delete(htu21d_handle_t sensor, bool del_bus);

#ifdef __cplusplus
}
#endif


#endif

