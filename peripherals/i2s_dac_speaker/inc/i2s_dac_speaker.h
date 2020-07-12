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

#ifndef _I2S_DAC_SPEAKER_H_
#define _I2S_DAC_SPEAKER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "esp_system.h"
#include "sdkconfig.h"



 
/**
 * @brief I2S ADC/DAC mode init.
 */
void example_i2s_init();

/**
 * @brief I2S ADC/DAC example
 *        1. Erase flash
 *        2. Record audio from ADC and save in flash
 *        3. Read flash and replay the sound via DAC
 *        4. Play an example audio file(file format: 8bit/8khz/single channel)
 *        5. Loop back to step 3
 */
void example_i2s_adc_dac(void*arg);


#ifdef __cplusplus
}
#endif

#endif




