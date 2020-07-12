// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef _iic_bh1750_sensor_task
#define _iic_bh1750_sensor_task

#ifdef __cplusplus
extern "C" {
#endif

extern SemaphoreHandle_t print_mux;
void iic_bh1750_sensor_task(void *arg);
 esp_err_t i2c_master_init(void);
 esp_err_t i2c_slave_init(void);
#ifdef __cplusplus
}
#endif

#endif // iic_bh1750_sensor_task
