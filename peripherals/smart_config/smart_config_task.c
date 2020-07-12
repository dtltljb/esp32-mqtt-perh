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

#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include "unity.h"

#include "iot_smartconfig.h"

#define SMART_CONFIG_TEST_EN 1


void smart_config_task(void* arg)
{
    while(1) {
        printf("sc status: %d\n", iot_sc_get_status());
        vTaskDelay(300/portTICK_PERIOD_MS);
        if(iot_sc_get_status() == SC_STATUS_LINK_OVER) {
            printf("sc status: %d\n", iot_sc_get_status());
            break;
        }
    }
    vTaskDelete(NULL);
}
