工程文件工作区,在此文件路径下make、make menuconfig、make flash monitor等操作。

	1).ubuntu install serial port driver pl2301, debug monitor 
	2).

├── sdkconfig.old
├── sdkconfig																		//make menuconfig context
├── readme.md																		//文档说明
├── partitions_adc_dac_example.csv
├── Makefile																		//cmake create makefile file
├── main
│   ├── main.c																	//main function input
│   ├── Kconfig.projbuild
│   └── component.mk
└── components																	//workspace cmake components
    ├── peripherals															//peripherals part 
    │   ├── Kconfig.projbuild
    │   └── component.mk
    └── baidu																		//baidu mqtt client
        └── component.mk

