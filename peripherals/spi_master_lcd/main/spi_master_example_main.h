#pragma once
#include <stdint.h>
#include "esp_err.h"


/**
 * @brief Initialize the effect
 *
 * @return ESP_OK on success, an error from the jpeg decoder otherwise.
 */
void spi_master_lcd_task(void *arg);
