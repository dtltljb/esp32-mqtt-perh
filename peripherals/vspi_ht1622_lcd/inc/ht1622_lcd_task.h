// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef _HT1622_LCD_TASK_
#define _HT1622_LCD_TASK_

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t	display_acsii_buf[8];  //rang: 7 bit bcd + 1 dot

void ht1622_lcd_task(void *arg);


#ifdef __cplusplus
}
#endif

#endif // iic_bh1750_sensor_task
