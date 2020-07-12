#
# Component Makefile
#
# Component configuration in preprocessor defines

#user_peripherals_app
USER_PERIPHERALS_APP_PATH = ../../../peripherals
IOT_SOLUTION_PATH	=	../../../peripherals
Component_Json_Path = ../../../../../esp32-idf

#set(COMPONENT_EMBED_FILES image.jpg)
#COMPONENT_EMBED_FILES := image.jpg

COMPONENT_ADD_INCLUDEDIRS :=  \
$(USER_PERIPHERALS_APP_PATH)/adc/include	\
$(USER_PERIPHERALS_APP_PATH)/ledpwm/include	\
$(USER_PERIPHERALS_APP_PATH)/timer_group/inc	\
$(USER_PERIPHERALS_APP_PATH)/vspi_ht1622_lcd/inc	\
$(USER_PERIPHERALS_APP_PATH)/v_i2c_htu21d/inc	\
$(USER_PERIPHERALS_APP_PATH)/spi_dac121s101/inc	\
$(USER_PERIPHERALS_APP_PATH)/spi_ad_tm7705/inc	\
$(USER_PERIPHERALS_APP_PATH)/spi_master_lcd/main	\
$(USER_PERIPHERALS_APP_PATH)/i2s_dac_speaker/inc	\
$(USER_PERIPHERALS_APP_PATH)/gpio_int/inc	\
$(USER_PERIPHERALS_APP_PATH)/smart_config/include	\
$(Component_Json_Path)/components/json/cJSON/tests/unity/src	\

COMPONENT_SRCDIRS :=  \
$(USER_PERIPHERALS_APP_PATH)/adc \
$(USER_PERIPHERALS_APP_PATH)/ledpwm \
$(USER_PERIPHERALS_APP_PATH)/smart_config	\
$(USER_PERIPHERALS_APP_PATH)/timer_group \
$(USER_PERIPHERALS_APP_PATH)/vspi_ht1622_lcd	\
$(USER_PERIPHERALS_APP_PATH)/v_i2c_htu21d \
$(USER_PERIPHERALS_APP_PATH)/spi_dac121s101 \
$(USER_PERIPHERALS_APP_PATH)/spi_ad_tm7705 \
$(USER_PERIPHERALS_APP_PATH)/i2s_dac_speaker	\
$(USER_PERIPHERALS_APP_PATH)/spi_master_lcd/main	\
$(USER_PERIPHERALS_APP_PATH)/gpio_int	\

COMPONENT_OBJS =  \
$(USER_PERIPHERALS_APP_PATH)/adc/adc_example_task.o	\
$(USER_PERIPHERALS_APP_PATH)/ledpwm/led_example_task.o	\
$(USER_PERIPHERALS_APP_PATH)/timer_group/timer_example_evt_task.o	\
$(USER_PERIPHERALS_APP_PATH)/i2s_dac_speaker/i2s_dac_speaker.o	\
$(USER_PERIPHERALS_APP_PATH)/gpio_int/gpio_int_task.o	\
\
$(USER_PERIPHERALS_APP_PATH)/spi_dac121s101/spi_dac121s101.o	\
$(USER_PERIPHERALS_APP_PATH)/spi_dac121s101/dac121s101_spi_task.o	\
\
$(USER_PERIPHERALS_APP_PATH)/spi_ad_tm7705/spi_ad_tm7705.o	\
$(USER_PERIPHERALS_APP_PATH)/spi_ad_tm7705/ad_tm7705_spi_task.o	\
\
$(USER_PERIPHERALS_APP_PATH)/smart_config/smart_config.o	\
$(USER_PERIPHERALS_APP_PATH)/smart_config/smart_config_task.o	\
\
$(USER_PERIPHERALS_APP_PATH)/v_i2c_htu21d/v_i2c_bus.o	\
$(USER_PERIPHERALS_APP_PATH)/v_i2c_htu21d/v_i2c_htu21d.o	\
$(USER_PERIPHERALS_APP_PATH)/v_i2c_htu21d/htu21d_i2c_sensor_task.o	\
\
$(USER_PERIPHERALS_APP_PATH)/vspi_ht1622_lcd/v_spi_bus.o	\
$(USER_PERIPHERALS_APP_PATH)/vspi_ht1622_lcd/vspi_ht1622_lcd.o	\
$(USER_PERIPHERALS_APP_PATH)/vspi_ht1622_lcd/lcd_cdc_3077t_p14.o	\
$(USER_PERIPHERALS_APP_PATH)/vspi_ht1622_lcd/ht1622_lcd_task.o	\
\
#$(USER_PERIPHERALS_APP_PATH)/spi_master_lcd/main/spi_master_example_main.o	\
#$(USER_PERIPHERALS_APP_PATH)/spi_master_lcd/decode_image.o	\
#$(USER_PERIPHERALS_APP_PATH)/spi_master_lcd/pretty_effect.o	\
