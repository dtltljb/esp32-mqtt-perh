#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal_spi.h"
#include "stm32l0xx_hal_tim.h"
#include "touch.h"
#include "main.h"
#define KEY1 1
#define KEY2 2 
#define KEY3 3
#define KEY4 4
#define KEY5 5
#define KEY6  6
#define KEY7  7
#define KEY8  8
#define KEY9  9
#define KEY10  0x0a
#define KEY11  0x0b
#define KEY12  0x0c
#define READ_KEY_ADDR 0x55 


#define SOURCE_CODE

#define RBUFLEN 570
extern unsigned char SenseFlag;
unsigned char outpointer = 0;
unsigned char I2Cdata = 0x00;
unsigned char DeviceFlag = 0;
unsigned char DeviceID=0x18;
extern unsigned char g_Receive_Buf[RBUFLEN];
void delay_us(unsigned int delay)   // 10US
{
	unsigned i;
	for(i=0;i<delay;i++)
	{
		__nop();
		
	}
}


//void I2C_START(void)
//{	SCL=0; SDA=0;								   	//Set SCL=0, SDA=0
//	SCLC=0;SDAC=0;								   	//SCL/SDA as Output Mode
//	_delay(10);
//	SCL=1; SDA=1; _delay(10);						//SCL=1, SDA=1, Delay 10us
//	SDA=0; _delay(10);								//SDA=0, Delay 10us	
//	SCL=0; _delay(10);								//SCL=0, Delay 10us	
//}
//void I2C_STOP(void)
//{	SCL=0; SDA=0;									   //Set SCL=0, SDA=0
//	SDAC=0;										   	//SDA as Output Mode
//	_delay(10);
//	SCL=1; _delay(10);								//SCL=1, Delay 10us
//	SDA=0; _delay(10);								//SDA=0, Delay 10us
//	SDA=1; _delay(10);								//SDA=1, Delay 10us		
//	SCL=0; _delay(10);								//SCL=0, Delay 10us	
//}
////////////////////////////////////////////////////////////////////////////////////////
// IIC功能的起始
////////////////////////////////////////////////////////////////////////////////////////
void IIC_Start(void)				//IIC开始  当SCL为高的时候，SDA高电平跳变到低电平
{	
	#ifdef SOURCE_CODE
	
	IIC_SDA_L();
	IIC_SCL_L();
	delay_us(I2C_PER_10US);
	IIC_SDA_H();
	IIC_SCL_H();
	delay_us(I2C_PER_10US);
	IIC_SDA_L();
	delay_us(I2C_PER_10US);
	IIC_SCL_L();
	delay_us(I2C_PER_10US);
#else
	IIC_SCL_H();
	delay_us(I2C_PER_10US);
	
	IIC_SDA_H(); 
	delay_us(I2C_PER_10US);
  
	IIC_SDA_L(); 
	delay_us(I2C_PER_10US);
#endif
} 

void IIC_Stop(void)					//IIC停止   当SCL为高的时候，SDA低电平跳变到高电平  
{	
	#ifdef SOURCE_CODE
	IIC_SDA_L();
	IIC_SCL_L();
	delay_us(I2C_PER_10US);
	IIC_SCL_H();
	delay_us(I2C_PER_10US);
	IIC_SDA_L();
	delay_us(I2C_PER_10US);
	IIC_SDA_H();
	delay_us(I2C_PER_10US);
	IIC_SCL_L();
	delay_us(I2C_PER_10US);
#else
	IIC_SCL_H();    
	delay_us(I2C_PER_10US);
	IIC_SDA_L();
	delay_us(I2C_PER_10US);
	IIC_SDA_H(); 
  delay_us(I2C_PER_10US);
#endif
}

unsigned char I2C_Write_Byte(unsigned char data)
{
	#ifdef SOURCE_CODE
	unsigned short temp=0x80;
	IIC_SCL_L(); 																			//SCL=0; SDAC=0;									   //SCL=0, SDAC as Output
	do
	{  (data&temp)?IIC_SDA_H():IIC_SDA_L(); 					//	SDA=I2CData & temp;
		delay_us(I2C_PER_10US);													//_delay(10);	      			//Set SDA State and delay 10us
		IIC_SCL_H(); 																		//SCL=1; 
		 delay_us(I2C_PER_10US);												//_delay(10); 
		IIC_SCL_L(); 																			//SCL=0; 
		delay_us(I2C_PER_10US);														//_delay(10);  //Generate SCL Clock
		temp>>=1;									   												//Next Bit
	} while (temp!=0);
	return HAL_OK;
	
	#else
	unsigned char i;
	for(i=0;i<8;i++)
	{
		IIC_SCL_L();
		delay_us(I2C_PER_10US);
  	(data&0x80)?IIC_SDA_H():IIC_SDA_L();
		delay_us(I2C_PER_10US);
		IIC_SCL_H(); 
    delay_us(I2C_PER_10US);		
		data = data<<1;
	}
	IIC_SCL_L();
	delay_us(I2C_PER_10US);
	IIC_SDA_H();   //输出高电平，读取外设的输出
	delay_us(I2C_PER_10US);
	if(IIC_SDA_IN()==1)
	{
		return HAL_ERROR;  //等待应答
	}
	IIC_SCL_H();
	delay_us(I2C_PER_10US);
	IIC_SCL_L();
	IIC_SDA_H(); 
	return HAL_OK;
	#endif
}

unsigned char I2C_Read_Byte(unsigned char flag)
{
	#ifdef SOURCE_CODE
	unsigned char I2CData=0, temp=0x80;
	do
	{	
		IIC_SCL_H(); 														//SCL=1; 
		delay_us(I2C_PER_10US);			   					//_delay(10);							//Set SCL=1, Delay 10us
		if(IIC_SDA_IN())I2CData|=temp; 					//if(SDA)I2CData|=temp;						//Received Data
		IIC_SCL_L();														//SCL=0; 
		delay_us(I2C_PER_10US);									//_delay(10);							//Set SCL=0, Delay 10us	
		temp>>=1;									   						//Next Bit
	} while (temp!=0);
	 IIC_SDA_H(); 														//SDA=1; 
	delay_us(I2C_PER_10US);										//_delay(10);																//Send NoACK Signal
	IIC_SCL_H();															//SCL=1; 
	delay_us(I2C_PER_10US);										//_delay(10); 
	IIC_SCL_L(); 															//SCL=0;
	return I2CData;
	#else
	unsigned char i;
	IIC_SCL_L();
	IIC_SDA_H();       //输出高电平，读取外设的输出
	delay_us(I2C_PER_10US);
	for(i=0;i<8;i++)
	{
    I2Cdata = I2Cdata<<1; 
		if(IIC_SDA_IN())                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
		{
			I2Cdata |= 0x01;
		}
		else
		{
			I2Cdata &= ~0x01;
		}
		delay_us(I2C_PER_10US);
		IIC_SCL_H();  
    delay_us(I2C_PER_10US);		
		IIC_SCL_L();
		delay_us(I2C_PER_10US);
	}
  (flag)?IIC_SDA_L():IIC_SDA_H();//应答，不应答
	delay_us(I2C_PER_10US);
	IIC_SCL_H();
	delay_us(I2C_PER_10US);
	IIC_SCL_L();
	IIC_SDA_H();
	return I2Cdata;
	#endif
}

void CheckACK(void)
{	unsigned short i=200;
	//SDAC=1;											   //Config SDAC as Input
	
	IIC_SCL_H();	//SCL=1;
	delay_us(I2C_PER_10US);														//_delay(10);								//Set SCL=1, Delay 10us
	while(i--)
	{	
		if(IIC_SDA_IN()==0){
     DeviceFlag=1; break;}		//if(!SDA) {DeviceFlag=1; break;}		   //Break if Device Respounding
		delay_us(I2C_PER_100US);                    //_delay(100);								   //200*100uS=2ms Waiting Time
	} 
	IIC_SCL_L();//SCL=0; 
	delay_us(I2C_PER_10US);  											//_delay(10);								//Set SCL=0, Delay 10us
}

////////////////////////////////////////////////////
//7位地址为0x48，低第四位是键值，高4位是键值取反
//////////////////////////////////////////////////////
void   TransferValue(unsigned char value,unsigned char *tvalue)
{
	  switch(value)
		{
			case KEY1:
			  *tvalue = KEY11;
			break;
			case KEY2:
				*tvalue = KEY7;
			break;
			case KEY3:
				*tvalue = KEY4;
				//*tvalue = 0x0;
			break;
			case KEY4:
				//*tvalue = KEY8;
			*tvalue = KEY1;
			break;
			case KEY5:
				//*tvalue = KEY4;
				*tvalue = 0x0;
			break;
			case KEY6:
				//*tvalue = KEY1;
			*tvalue = KEY8;
			break;
			case KEY7:
				*tvalue = KEY5;
			break;
			case KEY8:
				*tvalue = KEY2;
			break;
			case KEY9:
				*tvalue = KEY3;
			break;
			case KEY10:
					*tvalue = KEY6;
			break;
			case KEY11:
				*tvalue = KEY9;
			break;
			case KEY12:
				*tvalue = KEY12;
			break;
			default:
			break;
		}
}
uint8_t GetKeyValue(unsigned char *valuebuf)
{
	unsigned char TempValue = 0;
	#ifdef SOURCE_CODE
	IIC_Start();
	I2C_Write_Byte(DeviceID|0);
	CheckACK();
	if(DeviceFlag) 						//Device Respounding?
	{	
			I2C_Write_Byte(READ_KEY_ADDR);				//Yes, Transmit 7-SEG Code										  
	}
		CheckACK(); 
		IIC_Stop();
		delay_us(I2C_PER_10US);                                //写从机地址
		
		  IIC_Start(); 
		  I2C_Write_Byte(DeviceID|1); //LSB=1 for Read   读数据
			CheckACK();
			valuebuf[0]=I2C_Read_Byte(1); 
		  IIC_Stop();
			delay_us(I2C_PER_10US);
		  if((valuebuf[0]!=KEY1)&&(valuebuf[0]!=KEY2)&&(valuebuf[0]!=KEY3)&&(valuebuf[0]!=KEY4)&&(valuebuf[0]!=KEY5)&&(valuebuf[0]!=KEY6)
			   &&(valuebuf[0]!=KEY7)&&(valuebuf[0]!=KEY8)&&(valuebuf[0]!=KEY9)&&(valuebuf[0]!=KEY10)&&(valuebuf[0]!=KEY11)&&(valuebuf[0]!=KEY12))
			{
				return HAL_ERROR;
			}
		 else
		 {	
			TransferValue(valuebuf[0],&TempValue);
			 valuebuf[0] = TempValue;
			 return HAL_OK;
		 }

	#else
	 
#endif			
}




