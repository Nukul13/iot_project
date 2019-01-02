#include <lpc213x.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "lib_funcs.h"        // OCFreaks LPC214x Tutorials Library Header
#include "DHT11.h" 

#define relay1 4
#define relay2 3
#define MULVAL 15
#define DIVADDVAL 1

void pinselint();

/* all delay function */

void SmallDelay(void);
void Delay250(void);
void DelayMs(int n);
void delay (int);
 
//void initClocks(void);
void initUART0(void);
void uartWrite(char c);
char uartRead(void);

void initTimer0(void);
void delayUS(unsigned long microseconds);
void delayMS(unsigned int milliseconds);
void startTimer0(void);
unsigned int stopTimer0(void);

/* for serial communication */

void UART1_Init(void);
void UART1_Write(unsigned char value);
void UART1_Write_Text(unsigned char msg[]);
unsigned char UART1_Read(void);
void UART0_Init(void);
void UART0_Write(unsigned char value);
void UART0_Write_Text(unsigned char msg[]);

uint8_t UART0_RxString(char *ptr_string) ; 
unsigned char UART0_Read(void);

void setup();
void serialnumbersend(int value);
void UART0_ReadStatus(void);

void Display_Number_Lcd0(unsigned char row,unsigned char col,unsigned char places,unsigned long number);

void Display_Number_Lcd1(unsigned char row,unsigned char col,unsigned char places,unsigned long number);
void Display_Number_Lcd2(unsigned char row,unsigned char col,unsigned char places,unsigned long number);
void Display_Number_Lcd3(unsigned char row,unsigned char col,unsigned char places,unsigned long number);




void Display_Number_Fraction_Lcd(unsigned char row,unsigned char col,unsigned char places,unsigned long number);

void ADC0_Init(void);
void ADC1_Init(void);
void ADC2_Init(void);

unsigned int ADC0_Read(void);

void Delay_ms(unsigned long times);

void sendurl1();
void sendurl2();
void sendurl3();
void sendurl4();
void sendurl5();

void sendurl6() ; 



int x;	
unsigned long adc_data1 , adc_data2 , adc_data3,a ;

unsigned int range=0,i = 0;

unsigned char ch, b[10] ,data[7],data1[7], Temp[50] = "Temp :",H[50] = "Humid", A0[20],A1[20],A2[20],A3[20]  ; 
 unsigned int temp = 0 , h = 0;
  char temp_buff[10];
int echoTime=0 ; 
unsigned long distance1[] = {1,2,3,5,7,8,10,12,11,12,13,15,16,3,5,1,2,3,18,19};
float distance = 0 ;
/* main function */
	  char str[50];  
    int len = 0;  
int main()
{

	
			unsigned char msg[] = "WELCOME";
			unsigned char adc_example[] = "IOT BASED AIR POLLUTION METER";
			uint8_t humidity_integer, humidity_decimal, temp_integer, temp_decimal, checksum; 
			int i  = 0, j = 0 ,RPM = 0 ,RPM1 = 0 , PWM_Status = 0 , k  = 0,LDR_Status =  0 ,var = 0,G = 0 ,Flag_1 = 1 ;
	
/*******************************END OF DECLARATIONS ********************************************************/	
	
	
	
	
			initTimer0(); //Init Timer for delay functions	
			PINSEL2 |= 0x000000; //Configure the PORT1 Pins as GPIO;
			PINSEL0 |= 0x000000; //Configure the PORT1 Pins as GPIO;
			PINSEL1 |= 0x000000; //Configure the PORT1 Pins as GPIO;
			UART0_Init(); //BR 9600
			UART1_Init();  //BR 115200 
			UART0_Write_Text(adc_example);
			UART0_Write(10);
			UART0_Write(13);
		  setup();
			UART0_Write_Text(msg);
			UART0_Write(10);
			UART0_Write(13);
			UART0_Write_Text("\r\n Reading taken....\r\n");
			UART0_Write(10);
			UART0_Write(13);

/*******************************END OF INITILIZATIONS ********************************************************/	
	
/*******************************START OF WHILE LOOP ********************************************************/	
	
			while(1)
			{
				
/*************************************** MQ135 GAS sensor **********************************************************************/


										ADC0_Init() ; 
										adc_data1 = ADC0_Read() ;
										adc_data1 = adc_data1 / (float)3.1 ;
										UART0_Write_Text("G =\n\r") ;			
										UART0_Write(G) ;

				
										UART0_Write_Text("\n\r MQ3:-") ;
										Display_Number_Lcd1(2,1,4,adc_data1) ;
										UART0_Write_Text("\n\r") ;			
										//adc_data1*3300 ;
										//adc_data1 = adc_data1/1023 ;			//Value of Voltage in Milli Volts

/*************************************** Humidity sensor **********************************************************************/

										dht11_request();
										dht11_response();
										humidity_integer = dht11_data();
										humidity_decimal = dht11_data();
										temp_integer = dht11_data();
										temp_decimal = dht11_data();
										checksum = dht11_data();
										if((humidity_integer + humidity_decimal + temp_integer + temp_decimal) != checksum)
													UART0_Write_Text("Checksum Error\r\n");
										else
										{			
											
													UART0_Write_Text("\n\rRelative Humidity : ");
													memset(data, 0, 7);
													sprintf(data, "%d", humidity_integer);
													UART0_Write_Text(data);
											    UART0_Write_Text("%RH");
													UART0_Write('\n');
													UART0_Write('\r');
											
													UART0_Write_Text("\n\rTemperature : ");
													memset(data1, 0, 7);
													sprintf(data1,"%d",temp_integer);
													UART0_Write_Text(data1);
											    UART0_Write_Text("degC");
													UART0_Write('\n');
													UART0_Write('\r');
													Delay_ms(1000);
													Delay_ms(1000);
//												  if(G%3 == 0 )
//													{
//														sendurl5();
//													}
													Delay_ms(2000);	
													Delay_ms(2000);	
													
										}
										
								
/*************************************** SENT HUMIDITY READING TO SERVER  **********************************************************************/
		
							if(G%3 == 0 )
							{
								
								sendurl5();
								Flag_1 = 0 ;
							
							}		
							
		

/*************************************** SENT GAS SENSOR READING TO SERVER  **********************************************************************/
								
							if((G%2 == 0 )&&(Flag_1 == 1))
							{
							
									sendurl1();
									
							
							}
							
							
							
/*************************************** SENT TEMPRATURE SENSOR READING TO SERVER  **********************************************************************/
							
							
							if((G%2 == 1 )&&(Flag_1 == 1))
							{
								
										sendurl4();
										Flag_1 = 0 ;

							}

							Flag_1 = 1 ;		
							G = G+1 ;
							UART0_Write_Text("\r\n\n\********************************************************************************************************************************\\r\n\n");

			}// WHILE END .

}// MAIN END.
	



void Delay_ms(unsigned long times)
{
	unsigned long i,j;
	for(j=0;j<times;j++)
					for(i=0;i<7500;i++);
}

void UART1_Init(void)
{

   
    //and Set 8 bit Character Length with 1 Stop bit and Parity Disabled
    //Access to Divisor Latches is Enabled, in order to write Baud Rate Generator Registers
    
    //Values to be written in Baud Rate Registers U0DLM and U0LL
    /*
    Formula is
    
    Baud_Rate = PCLK*MulVal / [(16*(256*U0DLM+U0DLL)*(MulVal + DivAddVal))]
    
    Example:-
    MulVal = 1;
    DivAddVal = 0;
    Baud_Rate = 9600;
    PCLK = 12MHz
    U0DLM = 0;
    
    Hence,
    U0DLL = 12000000/(9600*16) = 97.65625 = 97
    U0DLL = 97 = 0x62
    */
		
		
		PINSEL0 |= 0x00050000; 
		U1LCR = 0x83;                                //Enable access to Divisor Latches
		U1DLM = 0x00;
		U1DLL = 8; 														 
		U1LCR = 0x03;                                    //Disable Access to Divisor Latches
		
		
}
void UART0_Init(void)
{
			//and Set 8 bit Character Length with 1 Stop bit and Parity Disabled
			//Access to Divisor Latches is Enabled, in order to write Baud Rate Generator Registers

			//Values to be written in Baud Rate Registers U0DLM and U0LL
			/*
			Formula is

			Baud_Rate = PCLK*MulVal / [(16*(256*U0DLM+U0DLL)*(MulVal + DivAddVal))]

			Example:-
			MulVal = 1;
			DivAddVal = 0;
			Baud_Rate = 9600;
			PCLK = 12MHz
			U0DLM = 0;

			Hence,
			U0DLL = 12000000/(9600*16) = 97.65625 = 97
			U0DLL = 97 = 0x62
			*/
			//		U0LCR = 0x83;                                //Enable access to Divisor Latches
			//		U0DLM = 0x00;
			//		U0DLL = 8; 		
			//		U0LCR = 0x03;                                    //Disable Access to Divisor Latches	
			PINSEL0 = 0x00000005;			//P0.0 as TX0 and P0.1 as RX0 UART0 AND UART1 BOTH DEFINED
			U0LCR = 0x83;					//Enable access to Divisor Latches
			//	U1LCR = 0x83;
			//and Set 8 bit Character Length with 1 Stop bit and Parity Disabled
			//Access to Divisor Latches is Enabled, in order to write Baud Rate Generator Registers
			//Values to be written in Baud Rate Registers U0DLM and U0LL
			/*
			Formula for baud rate calculations
			Baud_Rate = PCLK*MulVal / [(16*(256*U0DLM+U0DLL)*(MulVal + DivAddVal))]
			Example:-
			MulVal = 1;
			DivAddVal = 0;
			Baud_Rate = 9600;
			PCLK = 15MHz
			U0DLM = 0;
			Hence,
			U0DLL = 15000000/(9600*16) = 97.65625 = 98
			U0DLL = 98 = 0x62	  */
			U0DLM = 0x0;
			//	U1DLM = 0x00;
			U0DLL = 0x62;									//Baud Rate of 9600
			//U1DLL = 0x62;								//Baud Rate of 9600
			U0LCR = 0x03;									//Disable Access to Divisor Latches
			//U1LCR = 0x03;					

}

void setup()
{

	
							UART1_Write_Text("AT\r\n");
							delay(2500);
							UART1_Write_Text("AT+GMR\r\n");

							delay(2500);
							UART1_Write_Text("AT+CWMODE=3\r\n");

							delay(2500);
							UART1_Write_Text("AT+CWLAP\r\n");

							delay(5000);
							//UART1_Write_Text("AT+CWJAP=\"TCP\",\"12345678\"\r\n");
	
						//	UART1_Write_Text("AT+CWJAP=\"OpulentWifi\",\"newPasswordHard\"\r\n");//New : - newPasswordHard  Old :- w|f|Pa55	
	
							UART1_Write_Text("AT+CWJAP=\"Sandip\",\"sandip8888\"\r\n");

							delay(5000);
							UART1_Write_Text("AT+CIFSR\r\n");

							delay(2500);
							
							
}




void sendurl1()
{

	  unsigned char url[]="GET /update?api_key=RGKL1QNZ5490FMY5&field1=0",i,j;	
		//unsigned char url[]="GET /wheatherMonitoring/getlight?light=",i,j;

	  UART1_Write_Text("AT+CIPMUX=1\r\n");
		delay(500);
		
	  UART1_Write_Text("AT+CIPSTART=4,\"TCP\",\"api.thingspeak.com\",80\r\n");
		//UART1_Write_Text("AT+CIPSTART=4,\"TCP\",\"192.168.43.46\",8080\r\n");
		delay(500);
	
		for(i = 0; url[i] != '\0'; ++i);
			{
					for(j = 0; A1[j] != '\0'; ++j, ++i)
					{

								url[i] = A1[j];
					}

			}
			
			
					url[i] = '\0';
	
		UART1_Write_Text("AT+CIPSEND=4,");
	
		for(i=0;url[i]!='\0';i++)
		{
			i++;
		}
		
		serialnumbersend(i+4);
		UART1_Write_Text("\r\n");
		delay(500);
	
		

			//printf("After concatenation: %s", s1);

			UART0_Write_Text(url);
			UART1_Write_Text(url);

			//UART1_Write_Text(url);
			//UART1_Write_Text(a);

			UART1_Write_Text("\r\n");
			delay(500);

			UART1_Write_Text("");
			delay(500);
			
}



void sendurl2()
{

	  //unsigned char url[]="GET /update?api_key=O0W3ZM2L61OIQX3D&field2=0",i,j;	
		unsigned char url[]="GET /wheatherMonitoring/getgases?gases=" , i , j;

	  UART1_Write_Text("AT+CIPMUX=1\r\n");
		delay(1500);
		
	  //UART1_Write_Text("AT+CIPSTART=4,\"TCP\",\"api.thingspeak.com\",80\r\n");
		UART1_Write_Text("AT+CIPSTART=4,\"TCP\",\"192.168.43.46\",8080\r\n");
		delay(1500);
	
		for(i = 0; url[i] != '\0'; ++i);
			{
					for(j = 0; A2[j] != '\0'; ++j, ++i)
					{

								url[i] = A2[j];
					}

			}
			
			
					url[i] = '\0';
	
		UART1_Write_Text("AT+CIPSEND=4,");
	
		for(i=0;url[i]!='\0';i++)
		{
			i++;
		}
		
		serialnumbersend(i+4);
		UART1_Write_Text("\r\n");
		delay(1500);

			//printf("After concatenation: %s", s1);
		
			UART0_Write_Text(url);
			UART1_Write_Text(url);

			//UART1_Write_Text(url);
			//UART1_Write_Text(a);

			UART1_Write_Text("\r\n");
			delay(1500);

			UART1_Write_Text("");
			delay(1500);
			
}



void sendurl3()
{

		//unsigned char url[]="GET /update?api_key=O0W3ZM2L61OIQX3D&field3=0",i,j;	
		unsigned char url[]="GET /wheatherMonitoring/getwindspeed?windspeed=",i,j;	
		UART1_Write_Text("AT+CIPMUX=1\r\n");
		delay(500);
		//UART1_Write_Text("AT+CIPSTART=4,\"TCP\",\"api.thingspeak.com\",80\r\n");
		UART1_Write_Text("AT+CIPSTART=4,\"TCP\",\"192.168.43.46\",8080\r\n");	
		delay(500);
	
		for(i = 0; url[i] != '\0'; ++i);
			{
					for(j = 0; A3[j] != '\0'; ++j, ++i)
					{

								url[i] = A3[j];
					}

			}
			
			
					url[i] = '\0';
	
		UART1_Write_Text("AT+CIPSEND=4,");
	
		for(i=0;url[i]!='\0';i++)
		{
			i++;
		}
		
		serialnumbersend(i+4);
		UART1_Write_Text("\r\n");
		delay(500);
	
		

					//printf("After concatenation: %s", s1);

					UART0_Write_Text(url);
					UART1_Write_Text(url);

					//UART1_Write_Text(url);
					//UART1_Write_Text(a);

					UART1_Write_Text("\r\n");
					delay(500);

					UART1_Write_Text("");
					delay(500);
			
}





void sendurl4()
{
//	unsigned char url[]="GET /update?api_key=O0W3ZM2L61OIQX3D&field4=0",i,j;	
	
	//localhost:8080/IOTBasedPucCarMonitoring/GetH?tempvalue=
	
unsigned char url[]="GET /update?api_key=RGKL1QNZ5490FMY5&field2=0",i,j;	
	
		UART1_Write_Text("AT+CIPMUX=1\r\n");
		delay(500);
		UART1_Write_Text("AT+CIPSTART=4,\"TCP\",\"api.thingspeak.com\",80\r\n");
		//UART1_Write_Text("AT+CIPSTART=4,\"TCP\",\"192.168.43.46\",8080\r\n");	
		delay(500);	
		for(i = 0; url[i] != '\0'; ++i);
			{
					for(j = 0; data[j] != '\0'; ++j, ++i)
					{

								url[i] = data[j];
						
					}

			}
		url[i] = '\0';
			
		UART1_Write_Text("AT+CIPSEND=4,");
	
		for(i=0;url[i]!='\0';i++)
		{
				i++;
		}
		
					serialnumbersend(i+4);
					UART1_Write_Text("\r\n");
					delay(500);

					//printf("After concatenation: %s", s1);

				  UART0_Write_Text(url);
					UART1_Write_Text(url);

					//UART1_Write_Text(url);
					//UART1_Write_Text(a);

					UART1_Write_Text("\r\n");
					delay(500);

					UART1_Write_Text("");
					delay(500);
			
}



void sendurl5()
{
	
//	unsigned char url[]="GET /update?api_key=O0W3ZM2L61OIQX3D&field4=0",i,j;	
	
	//localhost:8080/IOTBasedPucCarMonitoring/GetH?tempvalue=
	
		unsigned char url[]="GET /update?api_key=RGKL1QNZ5490FMY5&field3=0",i,j;	

		UART1_Write_Text("AT+CIPMUX=1\r\n");
		delay(500);
		UART1_Write_Text("AT+CIPSTART=4,\"TCP\",\"api.thingspeak.com\",80\r\n");
	//	UART1_Write_Text("AT+CIPSTART=4,\"TCP\",\"192.168.43.46\",8080\r\n");	
		
		delay(500);
	
		for(i = 0; url[i] != '\0'; ++i);
			{
					for(j = 0; data1[j] != '\0'; ++j, ++i)
					{
								url[i] = data1[j];
					}

			}
		url[i] = '\0';
			
		UART1_Write_Text("AT+CIPSEND=4,");
	
		for(i=0;url[i]!='\0';i++)
		{
			i++;
		}
					serialnumbersend(i+4);
					UART1_Write_Text("\r\n");
					delay(500);
					//printf("After concatenation: %s", s1);
				  UART0_Write_Text(url);
					UART1_Write_Text(url);
					//UART1_Write_Text(url);
					//UART1_Write_Text(a);
					UART1_Write_Text("\r\n");
					delay(500);
					UART1_Write_Text("");
					delay(500);
			
		
}





void sendurl6()
{
//	unsigned char url[]="GET /update?api_key=O0W3ZM2L61OIQX3D&field4=0",i,j;	
	
	//localhost:8080/IOTBasedPucCarMonitoring/GetH?tempvalue=
	
unsigned char url[]="GET /wheatherMonitoring/getrainfall",i,j;	
	
		UART1_Write_Text("AT+CIPMUX=1\r\n");
		delay(500);
		//UART1_Write_Text("AT+CIPSTART=4,\"TCP\",\"api.thingspeak.com\",80\r\n");
		UART1_Write_Text("AT+CIPSTART=4,\"TCP\",\"192.168.43.46\",8080\r\n");	
		delay(500);	
//		for(i = 0; url[i] != '\0'; ++i);
//			{
//					for(j = 0; data1[j] != '\0'; ++j, ++i)
//					{
//								url[i] = data1[j];
//					}

//			}
//		url[i] = '\0';
			
		UART1_Write_Text("AT+CIPSEND=4,");
	
		for(i=0;url[i]!='\0';i++)
		{
			i++;
		}
		
					serialnumbersend(i+4);
					UART1_Write_Text("\r\n");
					delay(500);

					//printf("After concatenation: %s", s1);

				  UART0_Write_Text(url);
					UART1_Write_Text(url);

					//UART1_Write_Text(url);
					//UART1_Write_Text(a);

					UART1_Write_Text("\r\n");
					delay(500);

					UART1_Write_Text("");
					delay(500);
			
}






uint8_t UART0_RxString(char *ptr_string)
{
    char ch;
    uint8_t len = 0;
    while(1)
    {
        ch=UART0_Read();    //Receive a char
        UART0_Write(ch);     //Echo back the received char

        if((ch=='\r') || (ch=='\n')) //read till enter key is pressed
        {   
          if(len!=0)         /* Wait till atleast 1 char is received */              
           { 
               ptr_string[len]=0;           //once enter key is pressed null terminate the string and break the loop             
               break;  
            }                   
        }
        else if((ch == '\b') && (len != 0))
        {
					
            len-- ;    //If backspace is pressed then decrement the index to remove the old char
        
				}
        else
        {
					
            ptr_string[len]=ch; //copy the char into string and increment the index
            len++;
					
        }
    }
		
  return len ;
		
}


void UART1_Write(unsigned char value)
{
	
    /*    
    THRE bit can be extracted by this U0LSR & 0x20
    THRE = 0 means data is present.
    THRE = 1 means register is empty.
    In order to transmit data, we have to wait will the THRE = 1,
    then only we can transmit data.
    */
    
    while(!(U1LSR&0x20));                        //THRE = 0 stay here
    U1THR = value;
	
}

void UART0_Write(unsigned char value)
{
	
    /*    
    THRE bit can be extracted by this U0LSR & 0x20
    THRE = 0 means data is present.
    THRE = 1 means register is empty.
    In order to transmit data, we have to wait will the THRE = 1,
    then only we can transmit data.
    */
    
    while(!(U0LSR&0x20));                        //THRE = 0 stay here
    U0THR = value;

}

void UART1_Write_Text(unsigned char msg[])
{
	
    while(*msg)
    {
			
        UART1_Write(*msg);
        msg++;
			
    }

}
void UART0_Write_Text(unsigned char msg[])
{
    while(*msg)
    {
        UART0_Write(*msg);
        msg++;
    }
}

unsigned char UART0_Read(void)
{
    /*
    Receiver Data Ready = U0LSR.0 bit
    RDR bit can be extracted by this U0LSR & 0x01
    RDR = 0 means no Data is Received in U0RBR
    RDR = 1 means that Data is present in U0RBR
    */
    while(!(U0LSR & 0x01));                        //RDR = 0 stay here..............
    return (U0RBR);
}

unsigned char UART1_Read(void)
{
    /*
    Receiver Data Ready = U0LSR.0 bit
    RDR bit can be extracted by this U0LSR & 0x01
    RDR = 0 means no Data is Received in U0RBR
    RDR = 1 means that Data is present in U0RBR
    */
    while(!(U1LSR & 0x01));                        //RDR = 0 stay here
    return (U1RBR);
} 

void pinselint()
{
	
	
	PINSEL0 = 0x00050005;                //P0.0 as TX0 and P0.1 as RX0 and select pin p0.4 and p0.5 as a adc and TXD1 and TXD2
	PINSEL1=0X00000000;
	PINSEL2=0X00000000;
	IO1DIR = 0x03030000;		// Configure P1.24(EN) and P1.25(RS) as Output
	IO1CLR = 0x03000000;		// CLEAR(0) P1.24(EN) and P1.25(RS)
	IO0DIR = 0x40428018;		// Configure P0.15(D4), P0.17(D5), P0.22(D6) and P0.30(D7) as Output
	IO0CLR = 0x40428000;		// CLEAR(0) P0.15(D4), P0.17(D5), P0.22(D6) and P0.30(D7)
	
	
	
}

void DelayMs(int n)
{
	
	
	int k ;
	for(k = 0 ; k < n ; k ++)
	{
		Delay250() ;
		Delay250() ;
	}
	
	
}

void Delay250(void)
{
	int k,j ;
	j =200 ;
	for(k = 0 ; k < 100 ; k ++)
	{
		j-- ;
	}
}
void delay (int d)

{
/*Initialize the Timer*/

T0CTCR=0x00;

T0PR=14999;  //WITH OUT PLL

T0TCR=0x02;

T0TCR=0x01;

while (T0TC < d);
T0TC=0X00;
T0TCR=0x00;

}

 
void serialnumbersend(int value)
 {
		int length = 5;              // five digits 
		int div = 10000;    // divisor

		while( length )
		{
			
				UART1_Write( (value / div) + 48 );   // write highest digit first
				value = value % div;
				length--;
				div /= 10;
		}

 }

/*****************ADC Functions***************/
void ADC0_Init(void)
{
	
	/*************Initialize ADC AD0.0*************/
	
	AD0CR = 1<<21;								//A/D is Operational
	AD0CR = 0<<21;								//A/D is in Power Down Mode
	//PCONP = (PCONP &0x001817BE) | (1UL<<12);
	PINSEL0 |= 0x00;
	PINSEL1 |= 0x004000000;					//P0.27 is Configured as Analog to Digital Converter Pin AD0.0	
	AD0CR |= 0x00200404;						//CLKDIV=4,Channel-0.0 Selected,A/D is Operational
	
	/*
	
	A/D Clock = PCLK /(CLKDIV+1);
	
	*/
	
	/**********************************************/
	
}


void ADC1_Init(void)
{
	
	/*************Initialize ADC AD0.0*************/
	
	AD0CR = 1<<21;								//A/D is Operational
	AD0CR = 0<<21;								//A/D is in Power Down Mode
	//PCONP = (PCONP &0x001817BE) | (1UL<<12);
	PINSEL0 |= 0x00;
	PINSEL1 |= 0x001000000;					//P0.27 is Configured as Analog to Digital Converter Pin AD0.0	
	AD0CR |= 0x00200402;						//CLKDIV=4,Channel-0.0 Selected,A/D is Operational
	
	/*
	
	A/D Clock = PCLK /(CLKDIV+1);
	
	*/
	
	/**********************************************/
	
}


void ADC2_Init(void)
{
	
	/*************Initialize ADC AD0.0*************/
	
	AD0CR = 1<<21;								//A/D is Operational
	AD0CR = 0<<21;								//A/D is in Power Down Mode
	//PCONP = (PCONP &0x001817BE) | (1UL<<12);
	PINSEL0 |= 0x00;
	PINSEL1 |= 0x0010000000;					//P0.27 is Configured as Analog to Digital Converter Pin AD0.0	
	AD0CR |= 0x00200408;						//CLKDIV=4,Channel-0.0 Selected,A/D is Operational
	
	/*
	
	A/D Clock = PCLK /(CLKDIV+1);
	
	*/
	
	/**********************************************/
	
}




unsigned int ADC0_Read(void)
{
	
	unsigned long adc_data;
	AD0CR |= 1UL<<24;							//Start Conversion
	
	do
	{
		
		adc_data = AD0GDR;
		
	}while(!(adc_data & 0x80000000));
	//Wait untill the DONE bits Sets
	
	AD0CR &= ~0x01000000;					//Stops the A/D Conversion   
	adc_data = adc_data >> 6;
	adc_data = adc_data & 0x3FF;    //Clearing all other Bits
  return(adc_data);
	
}

/*********************************************/


/******Functions to Display Text on LCD*******/
void Display_Number_Lcd0(unsigned char row,unsigned char col,unsigned char places,unsigned long number)
{
	/*
	This is a Function which converts the Number into ASCII Format and Display that on LCD.
	Will Try to make this function Genric
	This function will also take row and column as argument to display text on lcd
	*/
	
	unsigned char thousands,thousands2,thousands3 , hundreds, tens, ones;
	unsigned long temp;
	
	temp = number ;
	ones = temp % 10;
	temp = temp / 10;
	tens = temp % 10;
	temp = temp / 10;
	hundreds = temp % 10;
	temp = temp / 10;
	thousands = temp % 10;
	temp = temp / 10;
	thousands2 = temp % 10;
	temp = temp / 10;
	thousands3 = temp % 10;
	
	
//		temp = temp / 10;
//	thousands2 = temp % 10;
//	
//	if(row == 1)
//	{
//		Lcd_Cmd(0X80+col-1);
//	}
//	if(row == 2)
//	{
//		Lcd_Cmd(0XC0+col-1);
//	}
//	
//			switch(places)
//			{

//			case 4:
//			//Lcd_Write(thousands|0x30);
//			UART0_Write(thousands|0x30);

//			case 3:
//			//Lcd_Write(hundreds|0x30);
//			UART0_Write(hundreds|0x30);

//			case 2:
//			//	Lcd_Write(tens|0x30);
//			UART0_Write(tens|0x30);

//			case 1:
//			//	Lcd_Write(ones|0x30);
//			UART0_Write(ones|0x30);		
//			break;

//			default:
//			break;

//			}
	

		A0[0] = thousands3|0x30 ; 
		A0[1] = thousands2|0x30 ; 
		A0[2] = thousands|0x30 ; 
		A0[3] = hundreds|0x30 ;
		A0[4] = tens|0x30 ;
		A0[5] = ones|0x30 ;
		A0[6] = '\0' ;	
	
	UART0_Write_Text(A0); 

}





/**************************************  Functions to Display Text on LCD  ****************************************/

void Display_Number_Lcd1(unsigned char row,unsigned char col,unsigned char places,unsigned long number)
{
	/*
	This is a Function which converts the Number into ASCII Format and Display that on LCD.
	Will Try to make this function Genric
	This function will also take row and column as argument to display text on lcd
	*/
	
	unsigned char thousands,thousands2,thousands3 , hundreds, tens, ones;
	unsigned long temp;
	
	temp = number ;
	ones = temp % 10;
	temp = temp / 10;
	tens = temp % 10;
	temp = temp / 10;
	hundreds = temp % 10;
	temp = temp / 10;
	thousands = temp % 10;
	temp = temp / 10;
	thousands2 = temp % 10;
	temp = temp / 10;
	thousands3 = temp % 10;
	
	
//		temp = temp / 10;
//	thousands2 = temp % 10;
	
//	
//	if(row == 1)
//	{
//		Lcd_Cmd(0X80+col-1);
//	}
//	if(row == 2)
//	{
//		Lcd_Cmd(0XC0+col-1);
//	}
//	
//			switch(places)
//			{

//			case 4:
//			//Lcd_Write(thousands|0x30);
//			UART0_Write(thousands|0x30);

//			case 3:
//			//Lcd_Write(hundreds|0x30);
//			UART0_Write(hundreds|0x30);

//			case 2:
//			//	Lcd_Write(tens|0x30);
//			UART0_Write(tens|0x30);

//			case 1:
//			//	Lcd_Write(ones|0x30);
//			UART0_Write(ones|0x30);		
//			break;

//			default:
//			break;

//			}
	
	
	A1[0] = thousands3|0x30 ; 
	A1[1] = thousands2|0x30 ; 
	A1[2] = thousands|0x30 ; 
	A1[3] = hundreds|0x30 ;
	A1[4] = tens|0x30 ;
	A1[5] = ones|0x30 ;
A1[6] = '\0' ;	
	
	UART0_Write_Text(A1); 

}






/******Functions to Display Text on LCD*******/
void Display_Number_Lcd2(unsigned char row,unsigned char col,unsigned char places,unsigned long number)
{
	/*
	This is a Function which converts the Number into ASCII Format and Display that on LCD.
	Will Try to make this function Genric
	This function will also take row and column as argument to display text on lcd
	*/
	
	unsigned char thousands,thousands2,thousands3 , hundreds, tens, ones;
	unsigned long temp;
	
	temp = number ;
	ones = temp % 10;
	temp = temp / 10;
	tens = temp % 10;
	temp = temp / 10;
	hundreds = temp % 10;
	temp = temp / 10;
	thousands = temp % 10;
	temp = temp / 10;
	thousands2 = temp % 10;
	temp = temp / 10;
	thousands3 = temp % 10;
	
	
//		temp = temp / 10;
//	thousands2 = temp % 10;
	
//	
//	if(row == 1)
//	{
//		Lcd_Cmd(0X80+col-1);
//	}
//	if(row == 2)
//	{
//		Lcd_Cmd(0XC0+col-1);
//	}
//	
//			switch(places)
//			{

//			case 4:
//			//Lcd_Write(thousands|0x30);
//			UART0_Write(thousands|0x30);

//			case 3:
//			//Lcd_Write(hundreds|0x30);
//			UART0_Write(hundreds|0x30);

//			case 2:
//			//	Lcd_Write(tens|0x30);
//			UART0_Write(tens|0x30);

//			case 1:
//			//	Lcd_Write(ones|0x30);
//			UART0_Write(ones|0x30);		
//			break;

//			default:
//			break;

//			}
	
	
	A2[0] = thousands3|0x30 ; 
	A2[1] = thousands2|0x30 ; 
	A2[2] = thousands|0x30 ; 
	A2[3] = hundreds|0x30 ;
	A2[4] = tens|0x30 ;
	A2[5] = ones|0x30 ;
	A2[6] = '\0' ;	
	
	UART0_Write_Text(A2); 

}




/******Functions to Display Text on LCD*******/
void Display_Number_Lcd3(unsigned char row,unsigned char col,unsigned char places,unsigned long number)
{
	/*
	This is a Function which converts the Number into ASCII Format and Display that on LCD.
	Will Try to make this function Genric
	This function will also take row and column as argument to display text on lcd
	*/
	
	unsigned char thousands,thousands2,thousands3 , hundreds, tens, ones;
	unsigned long temp;
	
	temp = number ;
	ones = temp % 10;
	temp = temp / 10;
	tens = temp % 10;
	temp = temp / 10;
	hundreds = temp % 10;
	temp = temp / 10;
	thousands = temp % 10;
	temp = temp / 10;
	thousands2 = temp % 10;
	temp = temp / 10;
	thousands3 = temp % 10;
	
	
//		temp = temp / 10;
//	thousands2 = temp % 10;
	
//	
//	if(row == 1)
//	{
//		Lcd_Cmd(0X80+col-1);
//	}
//	if(row == 2)
//	{
//		Lcd_Cmd(0XC0+col-1);
//	}
//	
//			switch(places)
//			{

//			case 4:
//			//Lcd_Write(thousands|0x30);
//			UART0_Write(thousands|0x30);

//			case 3:
//			//Lcd_Write(hundreds|0x30);
//			UART0_Write(hundreds|0x30);

//			case 2:
//			//	Lcd_Write(tens|0x30);
//			UART0_Write(tens|0x30);

//			case 1:
//			//	Lcd_Write(ones|0x30);
//			UART0_Write(ones|0x30);		
//			break;

//			default:
//			break;

//			}
	
	
	A3[0] = thousands3|0x30 ; 
	A3[1] = thousands2|0x30 ; 
	A3[2] = thousands|0x30 ; 
	A3[3] = hundreds|0x30 ;
	A3[4] = tens|0x30 ;
	A3[5] = ones|0x30 ;
	A3[6] = '\0' ;	
	
	UART0_Write_Text(A3); 

}





void Display_Number_Fraction_Lcd(unsigned char row,unsigned char col,unsigned char places,unsigned long number)
{
	
	
	/*
	This is a Function which converts the Number into ASCII Format and Display that on LCD.
	Will Try to make this function Genric
	This function will also take row and column as argument to display text on lcd
	It is the Modification of above Lcd Function.
	The chane is that it will divide the number by 100 and display the decimal and fractional part.
	This function is extremely usefull for displaying temperature and humidity on lcd.
	*/
	
	unsigned char hundreds, tens, ones;
	unsigned long temp;
	temp = number/100;
	ones = temp % 10;
	temp = temp / 10;
	tens = temp % 10;
	temp = temp / 10;
	hundreds = temp % 10;
	
	
	//	if(row == 1)
	//	{
	//		Lcd_Cmd(0X80+col-1);
	//	
	//	}
	//	if(row == 2)
	//	{
	//		Lcd_Cmd(0XC0+col-1);
	//	}
	//	

	switch(places)
	{
		case 3:
			//	Lcd_Write(hundreds|0x30);
		    UART0_Write(hundreds|0x30);
		
		case 2:
			//	Lcd_Write(tens|0x30);
        UART0_Write(tens|0x30);
		
		case 1:
				//Lcd_Write(ones|0x30);
		    UART0_Write(ones|0x30);
		
		    break;
		
		default:
				break;
		
	}
	
	temp = number ;
	ones = temp % 10;
	temp = temp / 10;
	tens = temp % 10;
	//Lcd_Write('.');
	//Lcd_Write(tens|0x30);
	//Lcd_Write(ones|0x30);
	
	
}
/********************************************************/





void initTimer0(void)
{
	//Assuming PCLK = 60Mhz
	T0CTCR = 0x0;
	T0PR = 60-1; //60 clock cycles @60Mhz = 1 us
	T0TCR = 0x02; //Reset Timer
}

void delayUS(unsigned long microseconds) //Using Timer0
{
	T0TCR = 0x02; //Reset Timer
	T0TCR = 0x01; //Enable timer
	while(T0TC < microseconds); //wait until timer counter reaches the desired delay
	T0TCR = 0x00; //Disable timer
}

void delayMS(unsigned int milliseconds)
{
	delayUS(milliseconds * 1000);
}

void startTimer0(void)
{
	T0TCR = 0x02; //Reset Timer
	T0TCR = 0x01; //Enable timer
}

unsigned int stopTimer0(void)
{
	T0TCR = 0x00; //Disable timer
	return T0TC;
}

void initUART0(void)
{
	//Assuming PCLK = 60Mhz
	PINSEL0 |= 0x5;  /* Select TxD for P0.0 and RxD for P0.1 */
	U0LCR = 3 | (1<<7) ; /* 8 bits, no Parity, 1 Stop bit | DLAB set to 1  */
	U0DLL = 110;
	U0DLM = 1;   
	U0FDR = (MULVAL<<4) | DIVADDVAL; /* MULVAL=15(bits - 7:4) , DIVADDVAL=0(bits - 3:0)  */
	U0LCR &= 0x0F; // Set DLAB=0 to lock MULVAL and DIVADDVAL
	//BaudRate is now ~9600 and we are ready for UART communication!
}



























		