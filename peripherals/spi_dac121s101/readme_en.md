[[中文]](./readme_cn.md)

(0) . SPI 接口
_____________________________
|NUM|GPIO | SPI   |DAC121S101|
------------------------------
|1|GPIO_05|SPI_CS  |SYNC  P6 |
|2|GPIO_23|SPI_MOSI|DI    P4 |
|3|GPIO_19|SPI_SCLK|SCLK  P5 |
------------------------------

(1) . Input Shift Register
The input shift register, , has sixteen bits. The first two bits are don't cares and are followed by two bits that
determine the mode of operation (normal mode or one of three power-down modes). The contents of the serial
input register are transferred to the DAC register on the sixteenth falling edge of SCLK.

	DB15 (MSB)																															DB0 (LSB)
	---------------------------------------------------------------------------------
  |x | x | PD1 | PD0 | D11 | D10 | D9 | D8 | D7 | D6 | D5 | D4 | D3 | D2 | D1 | D0|
  ---------------------------------------------------------------------------------
  _____________________________________
 |  PD1     |   PD0  |                 |
 --------------------------------------
 |   0      |    0   |Normal Operation |
 ---------------------------------------
 |   0      |    1   | 1 kr GND				 |
 ---------------------------------------
 |   1      |    0   | 100kr GND       |
 ---------------------------------------
 |   1      |    1   | High Impedance  |
 ---------------------------------------
 
Normally, the SYNC line is kept low for at least 16 falling edges of SCLK and the DAC is updated on the 16th
SCLK falling edge. However, if SYNC is brought high before the 16th falling edge, the shift register is reset and
the write sequence is invalid. The DAC register is not updated and there is no change in the mode of operation
or in the output voltage.

(2) . Typical Application

The output voltage:	Vout = (Vref × D / 4096)


