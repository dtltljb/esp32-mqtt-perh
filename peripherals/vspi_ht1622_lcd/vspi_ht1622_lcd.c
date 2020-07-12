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
#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "vspi_ht1622_lcd.h"

ht1622_handle_t	ht1622	=	NULL;

typedef struct {
    v_spi_config_t bus;
    uint16_t dev_addr;
} ht1622_dev_t;

ht1622_handle_t vspi_ht1622_create( v_spi_config_t bus, uint8_t dev_addr)
{
    ht1622_dev_t* seg = (ht1622_dev_t*) calloc(1, sizeof(ht1622_dev_t));
    seg->bus = bus;
    seg->dev_addr = dev_addr;
    return (ht1622_handle_t) seg;
}

esp_err_t vspi_ht1622_delete(ht1622_handle_t dev, bool del_bus)
{
    ht1622_dev_t* seg = (ht1622_dev_t*) dev;
    free(seg);
    return ESP_OK;
}

esp_err_t vspi_ht1622_normal_cmd(void)
{
    ht1622_dev_t* seg = (ht1622_dev_t*) ht1622;
    if (seg == NULL)
    	printf("%s,%d  seg is null",__FILE__,__LINE__);
		uint8_t id		=	HT1622_CMD_CONTROL; 
		uint16_t cmd	=	HT1622_MD_NORMAL;
    v_spi_commad_mode(id,cmd);
    return ESP_OK;
}

esp_err_t vspi_ht1622_disable_cmd(void)
{
    ht1622_dev_t* seg = (ht1622_dev_t*) ht1622;
    if (seg == NULL)
    	printf("%s,%d  seg is null",__FILE__,__LINE__);
		uint8_t id	=	HT1622_CMD_CONTROL; 
		uint16_t cmd	=	HT1622_SYS_DIS;
    v_spi_commad_mode(id,cmd);
    printf("%s,%d, disable id = 0x%02x,cmd=0x%02x \n",__FILE__,__LINE__,id,cmd);
    return ESP_OK;
}

esp_err_t vspi_ht1622_enable_cmd(void)
{
    ht1622_dev_t* seg = (ht1622_dev_t*) ht1622;
    if (seg == NULL)
    	printf("%s,%d  seg is null",__FILE__,__LINE__);
		uint8_t id	=	HT1622_CMD_CONTROL; 
		uint16_t cmd	=	HT1622_SYS_EN;
    v_spi_commad_mode(id,cmd);
    printf("%s,%d, enable id = 0x%02x,cmd=0x%02x \n",__FILE__,__LINE__,id,cmd);
    return ESP_OK;
}


esp_err_t vspi_ht1622_lcd_off_cmd(void)
{
    ht1622_dev_t* seg = (ht1622_dev_t*) ht1622;
    if (seg == NULL)
    	printf("%s,%d  seg is null",__FILE__,__LINE__);
		uint8_t id	=	HT1622_CMD_CONTROL; 
		uint16_t cmd	=	HT1622_LCD_OFF;
    v_spi_commad_mode(id,cmd);
    printf("%s,%d, lcd off id = 0x%02x,cmd=0x%02x \n",__FILE__,__LINE__,id,cmd);
    return ESP_OK;
}

esp_err_t vspi_ht1622_lcd_on_cmd(void)
{
    ht1622_dev_t* seg = (ht1622_dev_t*) ht1622;
    if (seg == NULL)
    	printf("%s,%d  seg is null",__FILE__,__LINE__);
		uint8_t id	=	HT1622_CMD_CONTROL; 
		uint16_t cmd	=	HT1622_LCD_ON;
    v_spi_commad_mode(id,cmd);
    printf("%s,%d, lcd on id = 0x%02x,cmd=0x%02x \n",__FILE__,__LINE__,id,cmd);
    return ESP_OK;
}

esp_err_t vspi_ht1622_rc_32k_cmd(void)
{
    ht1622_dev_t* seg = (ht1622_dev_t*) ht1622;
    if (seg == NULL)
    	printf("%s,%d  seg is null",__FILE__,__LINE__);
		uint8_t id	=	HT1622_CMD_CONTROL; 
		uint16_t cmd	=	HT1622_RC_32K;
    v_spi_commad_mode(id,cmd);
    printf("%s,%d, rc 32k id = 0x%02x,cmd=0x%02x \n",__FILE__,__LINE__,id,cmd);
    return ESP_OK;
}

esp_err_t vspi_ht1622_tone_off_cmd(void)
{
    ht1622_dev_t* seg = (ht1622_dev_t*) ht1622;
    if (seg == NULL)
    	printf("%s,%d  seg is null",__FILE__,__LINE__);
		uint8_t id	=	HT1622_CMD_CONTROL; 
		uint16_t cmd	=	HT1622_TONE_OFF;
    v_spi_commad_mode(id,cmd);
    printf("%s,%d, tone off id = 0x%02x,cmd=0x%02x \n",__FILE__,__LINE__,id,cmd);
    return ESP_OK;
}

esp_err_t vspi_ht1622_wdt_en_cmd(void)
{
    ht1622_dev_t* seg = (ht1622_dev_t*) ht1622;
    if (seg == NULL)
    	printf("%s,%d  seg is null",__FILE__,__LINE__);    
		uint8_t id	=	HT1622_CMD_CONTROL; 
		uint16_t cmd	=	HT1622_WDT_EN;
    v_spi_commad_mode(id,cmd);
    printf("%s,%d, wdt enable id = 0x%02x,cmd=0x%02x \n",__FILE__,__LINE__,id,cmd);
    return ESP_OK;
}

esp_err_t vspi_ht1622_wdt_dis_cmd(void)
{
    ht1622_dev_t* seg = (ht1622_dev_t*) ht1622;
    if (seg == NULL)
    	printf("%s,%d  seg is null",__FILE__,__LINE__);    
		uint8_t id		=	HT1622_CMD_CONTROL; 
		uint16_t cmd	=	HT1622_WDT_DIS;
    v_spi_commad_mode(id,cmd);
    printf("%s,%d, wdt disable id = 0x%02x,cmd=0x%02x \n",__FILE__,__LINE__,id,cmd);
    return ESP_OK;
}

/**
 * @brief v_spi_bus ht1622 initialization
 */
void v_spi_ht1622_init(void)
{
    v_spi_config_t conf;
    conf.mode = 0;
    conf.da_io_num = HT1622_PIN_NUM_DAT;
    conf.da_pullup_en = GPIO_PULLUP_ENABLE;
    
    conf.wr_io_num = HT1622_PIN_NUM_WR;
    conf.wr_pullup_en = GPIO_PULLUP_ENABLE;
    
    conf.rd_io_num = HT1622_PIN_NUM_RD;
    conf.rd_pullup_en = GPIO_PULLUP_ENABLE;
    
    conf.cs_io_num = HT1622_PIN_NUM_CS;
    conf.cs_pullup_en = GPIO_PULLUP_ENABLE;
    
		gpio_set_direction(conf.da_io_num, GPIO_MODE_OUTPUT);
    gpio_set_direction(conf.wr_io_num, GPIO_MODE_OUTPUT);
		gpio_set_direction(conf.rd_io_num, GPIO_MODE_OUTPUT);
		gpio_set_direction(conf.cs_io_num, GPIO_MODE_OUTPUT);
		gpio_set_level(V_SPI_DATA_IO, 1);
		gpio_set_level(V_SPI_WR_SCL_IO, 1);
		gpio_set_level(V_SPI_RD_SCL_IO, 1);
		v_spi_reset();														//hal reset ht1622
		ht1622 = vspi_ht1622_create(conf, 0);
  
}

esp_err_t vspi_ht1622_display_buf( uint16_t address, uint8_t *buf)
{
    ht1622_dev_t* seg = (ht1622_dev_t*) ht1622;
    if(seg == NULL)
			printf("%s,%d vspi_ht1622_ram_write seg is null",__FILE__,__LINE__);

		uint8_t	*u8 = buf;
		uint16_t addr=address;
		addr	=	addr | HT1622_CMD_WRITE;
		//printf("ht1622 write addr = 0x%02x,data=0x%02x \n",addr,*u8);
	  v_spi_write_mode(addr+2,u8);
	  *u8 = *u8 >>4;
	  v_spi_write_mode(addr,u8);
    return ESP_OK;
}



esp_err_t vspi_ht1622_display_4bit( uint16_t address, uint8_t *buf)
{
    ht1622_dev_t* seg = (ht1622_dev_t*) ht1622;
    if(seg == NULL)
			printf("%s,%d vspi_ht1622_ram_write seg is null",__FILE__,__LINE__);

		uint8_t	*u8 = buf;
		uint16_t addr=address;
		addr	=	addr | HT1622_CMD_WRITE;
		// printf("%s,%d,write addr = 0x%02x,data=0x%02x \n",__FILE__,__LINE__,addr,*u8);
	  *u8 = *u8 &0x0f;
	  v_spi_write_mode(addr,u8);
    return ESP_OK;
}


esp_err_t vspi_ht1622_ram_read( uint16_t address, uint8_t *buf)
{
    ht1622_dev_t* seg = (ht1622_dev_t*) ht1622;
    if(seg == NULL)
			printf("%s,%d vspi_ht1622_ram_read seg is null",__FILE__,__LINE__);

		uint16_t addr = address;
		addr	= (addr | HT1622_CMD_READ);
		v_spi_read_mode(addr,buf);
		/*
		for(uint8_t i=0; i < ilen; i++)
		{
	    v_spi_read_mode(addr,u8);
	   // printf("%s,%d, read addr = 0x%02x,data=0x%02x \n",__FILE__,__LINE__,addr,*u8);
	    addr	+=	2;		//even address
	    u8 	+= 	1;
	  }
	  */
    return ESP_OK;
}
