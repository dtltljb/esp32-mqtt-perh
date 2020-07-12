

/*========= define	io ====

1?HT1(PA4) ----PA0             切换
2?IIC-SCL  ----PB0              SCL
3?IIC-SDA  ----PB1             SDA
4?TOUCH-INT --- PA1            按键中断
5?TOUCH-PRO ---PC13            接近感应中断
*/


#define SW_RFID_TOUCH_L()     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0,GPIO_PIN_RESET)
#define SW_RFID_TOUCH_H()    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0,GPIO_PIN_SET)


#define IIC_SCL_L()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0,GPIO_PIN_RESET)
#define IIC_SCL_H()    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0,GPIO_PIN_SET)

#define IIC_SDA_L()        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET)
#define IIC_SDA_H()        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_SET)
#define IIC_SDA_IN()         HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)	

#define I2C_PER_10US         30
#define I2C_PER_100US         300
#define PASSWORD_KEY_LEN      8





void delay_us(unsigned int delay);
void IIC_Start(void);
void IIC_Stop(void);
unsigned char I2C_Write_Byte(unsigned char data);
unsigned char I2C_Read_Byte(unsigned char flag);

//uint8_t zyjk_check_cipher(uint8_t cipherlen,uint8_t* cipherbuf,unsigned char *temp_buf);
//uint8_t GetKeyValue(unsigned char *valuebuf);
//uint8_t zyjk_check_cipher(uint8_t cipherlen,uint8_t* cipherbuf);
//uint8_t GetKeyValue(unsigned char *valuebuf);


/********************************************************************/

void i2c_init(void)
{
PACNT_init;
PADDR_init;
PADAT_init;

SCL_high;
SDA_high;
}

/********************************************************************/

uint8 i2c_write(uint8 slave_address, uint8 *buffer, int byte_count, int freq)
{
    uint8 out_mask = 0x80;
    uint8 value = 0x00;
	  uint8 send_byte = 0x00;
	  uint8 status = 0x81;
	  int count = 8;
	  int clk_count = 0;
		int i = 0;

/* Set delay value based on frequency. */
	int D = (int) ((4000/freq) - 14);

	slave_address = (slave_address & 0xFE);
         
  i2c_start();
  delay(500);
  
   send_byte = slave_address;
        
   for(i = 0; i <= byte_count; i++)
   {    
    count = 8;  
    out_mask = 0x80;
         
    /* Send data bytes one bit at a time. */   
    while(count > 0)
    {
     value = ((send_byte & out_mask) ? 1 : 0);
      if (value == 1){
       	PADAT_init;
        SDA_high;
      }else{
       PADAT_init;
       SDA_low;
       }
       
      delay(D);     
                   
        PADAT_init;
      SCL_high;
      
      /* Clock stretching wait statement.  Wait until clock is released
      by slave.  Only effects program on first iteration.  */
   while (((GPIO_PADAT & 0x0200) ? 1 : 0) == 0){;}

      delay(2*D);  
       
      PADAT_init;
      SCL_low;
      delay(D); 
       
      out_mask >>= 1; 
      count--;   
     }
     
     PADAT_init;
     SDA_high;  /* Let go of data pin. */
     delay(D); 
      
     if (((GPIO_PADAT & 0x0400) ? 1 : 0) == 1)
     { 
      status = 0xA1; /* Transfer complete, bus busy, acknowledge not received. */
      break; } /* If not acknowledged, exit loop. */
     
    PADAT_init;
     SCL_high;
     delay(2*D);  
    
     PADAT_init;
     SCL_low;
     status = 0xA0;  /* Transfer complete, bus busy, acknowledge received. */
     delay(D); 
     
     send_byte = buffer[i];      
    }
    
    PADAT_init;
    SDA_high;
    SCL_low;   
delay(100);
return(status);
}

/********************************************************************/

uint8 i2c_read(uint8 slave_address, uint8 *buffer, int byte_count, int freq)
{
uint8 input_byte = 0x00;
uint8 value = 0x00;
uint8 out_mask = 0x80;
uint8 status = 0x81;
int count = 8;
int clk_count = 0;
int i = 0;

/* Set delay value based on frequency. */
int D = (int) ((4000/freq) - 14);

slave_address = (slave_address | 0x01);
   
i2c_start();
delay(500);
               
/**********  Write Address Procedure **********/
  
   while(count > 0)
   {
    value = ((slave_address & out_mask) ? 1 : 0);
     if (value == 1)
     {
      PADAT_init;
       SDA_high;}
     else
     {
      PADAT_init;
      SDA_low;}
      delay(D);      
                   
      PADAT_init;
     SCL_high;
      
     /* Clock stretching wait.  Wait until clock is released
     by slave.  */
     while (((GPIO_PADAT & 0x0200) ? 1 : 0) == 0){;}

     delay(2*D); 
       
     PADAT_init;
     SCL_low;
     delay(D);  
       
     out_mask >>= 1; 
     count--;         
  }
     
    PADAT_init;
  SDA_high;  /* Let go of data pin. */
    delay(D);
    SCL_high;
    delay(2*D);  
    
    /* If not acknowleged, set status accordingly and exit read process. */
    if (((GPIO_PADAT & 0x0400) ? 1 : 0) == 1)
    {
     status = 0xA1;
     return(status);}
    
    PADAT_init;
    SCL_low;
    delay(D);

/**********  Begin Read Procedure **********/

/* Release SDA and SCL to initiate transfer. */
PADAT_init;    
SDA_high;
SCL_high;

for(i = 0; i < byte_count; i++)
{
  count = 8;
  input_byte = 0x00;
   
  PADAT_init;
  SCL_high;
   
  /* Clock stretching wait.  Wait until clock is released
     by slave.  */
  while (((GPIO_PADAT & 0x0200) ? 1 : 0) == 0){;}
           
  /* Loop for bit-by-bit read of data. */
  while(count > 0) 
  {
    PADAT_init;
    SCL_high;
    delay(D);  
    delay(4); /* Required to make read and write clocks the same freq. */
    
   if ((GPIO_PADAT & 0x0600) == 0x0600)
    input_byte++;
  
   delay(D);  
   
   PADAT_init;
   SCL_low;
   delay(2*D); 
      
   if (count == 1)
    break;
   else  
    input_byte <<= 1;
  
   count--;
  }

  /* Write input byte to "read_buffer". */
  buffer[i] = input_byte;
    
  if(i == (byte_count - 1))
   break;
     
     /* Below is the acknowledge procedure. */     
     PADAT_init;
     SDA_low;
     delay(D);  
     SCL_high;     
  delay(2*D);  
    
     PADAT_init;
     SCL_low;
     delay(D);  
     SDA_high; 
     status = 0xA0; 
}     

/* Standard protocol calls for the last read byte to
    not receive an acknowledge from the master. */     
PADAT_init;
    SDA_high;
    SCL_high;     
delay(2*D); 
     
   PADAT_init;
   SCL_low;
   delay(D);  
   SDA_high;
   status = 0xA1;
   return(status);
}

/********************************************************************/

void i2c_start(void)
{
int clk_count = 0;
uint8 compare = 0x00;

PADAT_init;  
SDA_high;
delay(100);

PADAT_init;  
SCL_high;
delay(100);

/* Clock stretching wait.  Wait until clock is released
    by slave.  */
while (((GPIO_PADAT & 0x0200) ? 1 : 0) == 0){;}    
  
PADAT_init;
SDA_low;
delay(100);
     
    PADAT_init;  
SCL_low;
delay(100);
}

/********************************************************************/

uint8 i2c_stop(void)
{
uint8 status = 0x00;
int clk_count = 0;
  
PADAT_init;
SCL_low;
delay(100);

PADAT_init;
SDA_low;
delay(100);
    
    PADAT_init; 
SCL_high;

/* Clock stretching wait statement.  Wait until clock is released
    by slave.  */
while (((GPIO_PADAT & 0x0200) ? 1 : 0) == 0){;}
      
delay(100);
    
    PADAT_init; 
SDA_high;
status = 0x81; /* Set bus idle. */
return(status);
}

/********************************************************************/

void delay(int value)
{
int clk_count = 0;
while (clk_count < value)
{clk_count++;}
}

/********************************************************************/