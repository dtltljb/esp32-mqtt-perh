// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.


#ifndef _led_example_task
#define _led_example_task

#include "driver/ledc.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LEDC_HS_TIMER          LEDC_TIMER_0
#define LEDC_HS_MODE           LEDC_HIGH_SPEED_MODE
#define LEDC_HS_CH0_GPIO       (4)
#define LEDC_HS_CH0_CHANNEL    LEDC_CHANNEL_0

#define LEDC_HS_CH1_GPIO       (19)
#define LEDC_HS_CH1_CHANNEL    LEDC_CHANNEL_1

#define LEDC_LS_TIMER          LEDC_TIMER_1
#define LEDC_LS_MODE           LEDC_LOW_SPEED_MODE
#define LEDC_LS_CH2_GPIO       (21)
#define LEDC_LS_CH2_CHANNEL    LEDC_CHANNEL_2
#define LEDC_LS_CH3_GPIO       (5)
#define LEDC_LS_CH3_CHANNEL    LEDC_CHANNEL_3

#define LEDC_TEST_CH_NUM       (1)
#define LEDC_TEST_DUTY         (120)//(4000)
#define LEDC_TEST_FADE_TIME    (0)//(3000)

void led_example_task(void *arg);

#ifdef __cplusplus
}
#endif

#endif // led_example_task
