[[中文]](./readme_cn.md)

(0) . ht private bus interface, 
			
				 ---------------------------- 
				|NUM|GPIO |  adjust| HT1622  |
				------------------------------
				|1|CMD		|		IO22 |CS  	P1 |
				|2|CLK		|		IO25 |RD    P2 |
				|3|SD0		|		IO23 |WR  	P3 |
				|4|SD1		|		IO19 |DATA  P4 |
				------------------------------
				|5|TXD0   | 	IO18 |OSCI P6  |
				|6|RXD0   |   IO21 |IRQ  P9  |
				------------------------------
		
(1) . Command Format

	The HT1622 can be configured by the software setting.
	There are two mode commands to configure the
	HT1622 resource and to transfer the LCD display data.
	The following are the data mode ID and the command
	mode ID:
 			 —————————————————————————————————————————————————————————
			| 			ID 				|    command           |     note 			|
			-----------------------------------------------------------
			| 1   |  1  |  0  |     READ			       |                |
			| 1   |  0  |  1  |     WRITE            |                |
			| 1   |  0  |  1  | READ-MODIFY-WRITE    |                |
			| 1   |  0  |  0  |    COMMAND           |                |
			-----------------------------------------------------------
	If successive commands have been issued, the com-mand mode ID can be omitted. 
	While the system is operating in a non -successive command or a non-successive 
	address data mode, the CS pin should be set to 212 and the previous operation 
	mode will be re-set also. The CS pin returns to 202, a new operation
	mode ID should be issued first.

	 -----------------------------------------------------------------------
	|  NAME		 | Command Code |																							|
	| TONE OFF | 0000-1000-X  | Turn-off tone output 												|
	| TONE 4K  | 010X-XXXX-X  | Turn-on tone output, tone frequency is 4kHz |
	| TONE 2K  | 0110-XXXX-X  | Turn-on tone output, tone frequency is 2kHz |
	------------------------------------------------------------------------
	
(2) . Timing Diagrams
			READ Mode (Command Code : 1 1 0)          					 |    non					|
			READ Mode (Successive Address Reading)							 |		 support		|
			
			Successive Address reading is gpio-clk-mode ( 16bit clock )
			16bit =: 	8bit = 3bit_id + 5bit_addr, ( 8bit write clock);
								8bit = 4bit_data(com7~4 ) + 4bit_data (com3~0),(8bit read clk);
			
			WRITE Mode (Command Code : 1 0 1)										 |    non					|
			WRITE Mode (Successive Address Writing)							 |    support			|
			
			Successive Address reading is gpio-clk-mode ( 16bit clock )
			16bit =: 	8bit = 3bit_id + 5bit_addr, ( 8bit write clock);
								16bit = 4bit_data (SEG7~4 ) + 4bit_data (com3~0);(16bit read clk);
								
			READ-MODIFY-WRITE Mode (Command Code : 1 0 1)				 |			non				|
			READ-MODIFY-WRITE Mode (Successive Address Accessing)|	non						|
			
			Command Mode (Command Code : 1 0 0)									 |    support			|
			Mode (Data and Command Mode)												 |			non				|
			
			Command is gpio-clk-mode ( 12bit clock )
			12bit =: 3bit_id + 9bit_command_id , ( 12bit write clock).

(3) Command Summary
		----------------------------------------------------------------------------------------------
	 | NAME						|		ID		|			COMMAND CODE			| D / C |				FUNCTION										|
	  ----------------------------------------------------------------------------------------------
	 | READ						| 1 1 0 	| A5A4A3A2A1A0 D0D1D2D3 |   D   | Read data from the RAM						|
	 | WRITE					| 1 0 1   | A5A4A3A2A1A0 D0D1D2D3 |   D   | Write data to the RAM							|
	 | SYS DIS				| 1 0 0   | 0000-0000-X           |   C   | Turn off both system 							|
	 |                |         |                       |       | oscillator and LCD bias generator	|
	 | SYS EN         | 1 0 0   | 0000-0001-X           |   C   | Turn on system oscillator					|
	 | LCD OFF        | 1 0 0   | 0000-0010-X           |   C   | Turn off system LCD display				|
	 | LCD ON         | 1 0 0   | 0000-0011-X           |   C   | Turn on system LCD display				|
	 | RC 32K         | 1 0 0   | 0001-10XX-X           |   C   | System clock source, on-chip RC   |
	 | TONE OFF       | 1 0 0   | 0000-1000-X           |   C   | System clock source, on-chip RC   |
	 | NORMAL         | 1 0 0   | 1110-0011-X           |   C   | Normal mode   										|
	  ----------------------------------------------------------------------------------------------
	 other command non support.

(4) Display Memory - RAM Structure

		The static display RAM is organized into 64 ́4 bits and stores the display data.
		The contents of the RAM are directly mapped to the contents of the LCD driver.
		Data in the RAM can be accessed by the READ, WRITE and READ-MODIFY-WRITE commands.
		The following is a mapping from the RAM to the LCD patterns.
		
					 COM7 COM6  COM5  COM4        COM3  COM2  COM1  COM0
					 ----------------------------------------------------------
			SEG0|		 |		 |		 |		 |	1	 |		 |		 |		 |		 |	0	 |
					-----------------------------------------------------------
			SEG1|		 |		 |		 |		 |	3	 |		 |		 |		 |		 |	2	 |
					-----------------------------------------------------------
			SEG2|		 |		 |		 |		 |	5	 |		 |		 |		 |		 |	4	 |
					-----------------------------------------------------------
			SEG3|		 |		 |		 |		 |	7	 |		 |		 |		 |		 |	6	 |
					-----------------------------------------------------------
					|		  		...		  		 | ... |		      ...        	 | ... |
					-----------------------------------------------------------
		 SEG32|		 |		 |		 |		 |	63 |		 |		 |		 |		 | 62	 |
					-----------------------------------------------------------
					|	D3 |	D2 |	D1 |	D0 | D\A |	D3 |	D2 |	D1 | D0	 | D\A |
					 ----------------------------------------------------------
					
						Addr: A5A4A3A2A1A0  Data: D0D1D2D3
			
(5)  lcd encode table  (sch: carehealth-v0.1 ,lcd-model:  )
      
       _      _      _       _         |
      |	|    | |    |	|     |	|       ||
       -      -      -       -       |||
      | |    | |    | |     | |       ||
       -      -      -       -         |
       
(6)  驱动程序按照SEG0～SEG14为逐线扫描的方式,刷新HT1622的显示缓冲区。       
		 每次发送 8 位内容，15ms定时刷新。
		 
eg-cdc-3077-encode:
	//编码表==============>>>	0 1 2 3 4 5 6 7 8 9  A  b  C  d  E  F  - _ NULL
	
	unsigned char DIS_CODE[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18};
	const unsigned char LCD_MIRROR_RAM[]={0x5F,0x06,0x6B,0x2F,0x36,0x3D,0x7D,0x07,0x7F,0x3F,0x77,0x7C,0x71,0x6E,0x79,0x71,0x20,0x08,0x00};

breif:
	address=0x01,val=5;address=0x03,val=f;
			display:>  _
								| |
								| |
								 -	
	address=0x01,val=0;address=0x03,val=6;							
			display:>  
								 |
								 |
	address=0x01,val=6;address=0x03,val=b;							
			display:> _ 
								 |
								-
							 |_							 	
							 
(7)		CDC3077编码表的com1～com8与ht1622映像关系
			_____________    ______________
		| COM4  |  D0  |  | COM8  |  D5  |
		| COM3  |  D1  |  | COM7  |  D6  |
		| COM2  |  D2  |  | COM6  |  D7  |
		| COM1  |  D3  |  | COM5  |  D8  |
		 --------------    --------------
