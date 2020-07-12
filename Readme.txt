外设文档结构说明,
 1). 外设子文件夹是以外设名称命名的，子文件夹inc是头文件,源文件名称一般是按照设备名称、型号命名,基本上可望文生意;
 
 2). 程序分为三级架构、低层是与外设相关驱动程序、中间层、应用层.
 
 3). 子文件夹内有辅助说明文档,readme.md 。

.
├── vspi_ht1622_lcd
│   ├── vspi_ht1622_lcd.c				// ht1622 驱动程序
│   ├── v_spi_bus.c							// v_spi_bus 驱动程序
│   ├── readme_en.md
│   ├── readme_cn.md
│   ├── lcd_cdc_3077t_p14.c	    // 液晶屏cdc3077字库驱动
│   ├── inc
│   │   ├── vspi_ht1622_lcd.h
│   │   ├── v_spi_bus.h
│   │   ├── lcd_cdc_3077t_p14.h
│   │   └── ht1622_lcd_task.h
│   ├── ht1622_lcd_task.c												// lcd 应用程序
│   └── ht1622_lcd_example.c										// 参考例程
├── v_iic_bus																	//sub forlder 暂时未使用
│   ├── touch.c
│   ├── rtc_module.c
│   ├── inc
│   │   ├── touch.h
│   │   └── rtc_io.h
│   ├── i2c_bus
│   │   ├── README.md
│   │   ├── include
│   │   │   └── iot_i2c_bus.h
│   │   ├── i2c_bus_obj.cpp
│   │   ├── i2c_bus.c
│   │   └── component.mk
│   └── gpio.c
├── v_i2c_htu21d																		// htu21d 温湿度
│   ├── v_i2c_htu21d.c
│   ├── v_i2c_bus.c
│   ├── inc
│   │   ├── v_i2c_htu21d.h
│   │   ├── v_i2c_bus.h
│   │   └── htu21d_i2c_sensor_task.h
│   ├── htu21d_i2c_sensor_task.c
│   └── component.mk
├── ulp_i2c_bitbang																		//sub forlder 暂时未使用
│   ├── sdkconfig.old
│   ├── sdkconfig.defaults
│   ├── sdkconfig
│   ├── README.md
│   ├── readme_cn.md
│   ├── Makefile
│   ├── main
│   │   ├── ulp
│   │   │   ├── stack.S
│   │   │   ├── i2c.S
│   │   │   └── i2c_dev.S
│   │   ├── main.c
│   │   └── component.mk
│   └── inc
├── touch_pad_read																//sub forlder 暂时未使用
│   ├── README.md
│   ├── Makefile
│   ├── main
│   │   ├── tp_read_main.c
│   │   ├── component.mk
│   │   └── CMakeLists.txt
│   └── CMakeLists.txt
├── touch_pad_interrupt														//sub forlder 暂时未使用
│   ├── README.md
│   ├── Makefile
│   ├── main
│   │   ├── tp_interrupt_main.c
│   │   ├── component.mk
│   │   └── CMakeLists.txt
│   └── CMakeLists.txt
├── timer_group																		//sub forlder 暂时未使用
│   ├── timer_example_evt_task.c
│   ├── README.md
│   ├── Makefile
│   ├── main
│   │   ├── timer_group_example_main.c
│   │   ├── component.mk
│   │   └── CMakeLists.txt
│   ├── inc
│   │   └── timer_group_task.h
│   └── CMakeLists.txt
├── spi_master_lcd																//sub forlder 暂时未使用
│   ├── sdkconfig
│   ├── README.md
│   ├── Makefile
│   ├── main
│   │   ├── spi_master_example_main.h
│   │   ├── spi_master_example_main.c
│   │   ├── pretty_effect.h
│   │   ├── pretty_effect.c
│   │   ├── Kconfig.projbuild
│   │   ├── image.jpg
│   │   ├── decode_image.h
│   │   ├── decode_image.c
│   │   ├── component.mk
│   │   └── CMakeLists.txt
│   └── CMakeLists.txt
├── spi_dac121s101																				// dac 电压输出 
│   ├── spi_dac121s101.c
│   ├── readme_en.md
│   ├── readme_cn.md
│   ├── inc
│   │   ├── spi_dac121s101.h
│   │   └── dac121s101_spi_task.h
│   └── dac121s101_spi_task.c
├── spi_ad_tm7705																				//ad 采集 tm7705
│   ├── spi_ad_tm7705.c
│   ├── readme_en.md
│   ├── readme_cn.md
│   ├── inc
│   │   ├── spi_ad_tm7705.h
│   │   └── ad_tm7705_spi_task.h
│   └── ad_tm7705_spi_task.c
├── smart_config																				// wifi 链接 airkiss
│   ├── smart_config_task.c
│   ├── smart_config.c
│   ├── README.md
│   └── include
│       └── iot_smartconfig.h
├── sdio_sdCard																				//sub forlder 暂时未使用
│   ├── slave
│   │   ├── Makefile
│   │   ├── main
│   │   │   ├── Kconfig.projbuild
│   │   │   ├── component.mk
│   │   │   ├── CMakeLists.txt
│   │   │   └── app_main.c
│   │   └── CMakeLists.txt
│   ├── sdio_test.py
│   ├── README.md
│   └── host
│       ├── sdkconfig.defaults
│       ├── Makefile
│       ├── main
│       │   ├── Kconfig.projbuild
│       │   ├── component.mk
│       │   ├── CMakeLists.txt
│       │   └── app_main.c
│       ├── components
│       │   └── esp_slave
│       │       ├── include
│       │       │   └── esp_slave.h
│       │       ├── esp_slave.c
│       │       ├── component.mk
│       │       └── CMakeLists.txt
│       └── CMakeLists.txt
├── motor_stepper															// step motor 暂时未使用
│   └── a4988
│       ├── test
│       │   ├── stepper_a4988_test.cpp
│       │   └── component.mk
│       ├── include
│       │   └── iot_a4988.h
│       ├── component.mk
│       └── a4988.cpp
├── mcpwm																			//BLDC or DC motor 暂时未使用
│   ├── mcpwm_servo_control
│   │   ├── README.md
│   │   ├── Makefile
│   │   ├── main
│   │   │   ├── mcpwm_servo_control_example.c
│   │   │   ├── component.mk
│   │   │   └── CMakeLists.txt
│   │   └── CMakeLists.txt
│   ├── mcpwm_brushed_dc_control
│   │   ├── README.md
│   │   ├── Makefile
│   │   ├── main
│   │   │   ├── mcpwm_brushed_dc_control_example.c
│   │   │   ├── component.mk
│   │   │   └── CMakeLists.txt
│   │   └── CMakeLists.txt
│   ├── mcpwm_bldc_control
│   │   ├── README.md
│   │   ├── Makefile
│   │   ├── main
│   │   │   ├── mcpwm_bldc_control_hall_sensor_example.c
│   │   │   ├── component.mk
│   │   │   └── CMakeLists.txt
│   │   └── CMakeLists.txt
│   └── mcpwm_basic_config
│       ├── README.md
│       ├── Makefile
│       ├── main
│       │   ├── mcpwm_basic_config_example.c
│       │   ├── component.mk
│       │   └── CMakeLists.txt
│       └── CMakeLists.txt
├── ledpwm																						//led 指示灯
│   ├── led_example_task.c
│   ├── include
│   │   └── led_example_task.h
│   ├── component.mk
│   └── CMakeLists.txt
├── i2s_dac_speaker																	//	语言播报		
│   ├── tools
│   │   ├── sample_02.wav
│   │   ├── sample_01.wav
│   │   ├── sample_00.wav
│   │   └── generate_audio_file.py
│   ├── README.md
│   ├── partitions_adc_dac_example.csv
│   ├── Makefile
│   ├── main
│   │   ├── component.mk
│   │   └── CMakeLists.txt
│   ├── inc
│   │   ├── i2s_dac_speaker.h
│   │   └── audio_example_file.h
│   ├── i2s_dac_speaker.c
│   └── CMakeLists.txt
├── i2c_hts221																								//sub forlder 暂时未使用
│   ├── test
│   │   ├── hts221_test.c
│   │   ├── hts221_obj_test.cpp
│   │   └── component.mk
│   ├── README.md
│   ├── include
│   │   └── iot_hts221.h
│   ├── hts221_obj.cpp
│   ├── hts221.c
│   └── component.mk
├── i2c_ht16c21																							//sub forlder 暂时未使用
│   ├── test
│   │   ├── ht16c21_test.c
│   │   └── component.mk
│   ├── include
│   │   └── iot_ht16c21.h
│   ├── ht16c21.c
│   └── component.mk
├── gpio_int																								//key  管理程序
│   ├── README.md
│   ├── Makefile
│   ├── main
│   │   ├── component.mk
│   │   └── CMakeLists.txt
│   ├── inc
│   │   └── gpio_int_task.h
│   ├── gpio_int_task.c
│   └── CMakeLists.txt
├── flash_sava_param																			// 设备参数存储程序
│   ├── test
│   │   ├── param_test.c
│   │   └── component.mk
│   ├── README.md
│   ├── param.c
│   ├── include
│   │   └── iot_param.h
│   └── component.mk
├── bh1750																									//sub forlder 暂时未使用
│   ├── README.md
│   ├── inc
│   │   └── bh1750_i2c_sensor_task.h
│   ├── i2c_tools
│   │   ├── sdkconfig.defaults
│   │   ├── README.md
│   │   ├── partitions_example.csv
│   │   ├── Makefile
│   │   ├── main
│   │   │   ├── Kconfig.projbuild
│   │   │   ├── i2ctools_example_main.c
│   │   │   ├── component.mk
│   │   │   ├── cmd_i2ctools.h
│   │   │   ├── cmd_i2ctools.c
│   │   │   ├── cmd_decl.h
│   │   │   └── CMakeLists.txt
│   │   └── CMakeLists.txt
│   └── bh1750_i2c_sensor_task.c
└── adc																										//sub forlder 暂时未使用
    ├── include
    │   └── adc_example_task.h
    └── adc_example_task.c
