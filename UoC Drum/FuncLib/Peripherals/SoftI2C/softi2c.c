

#include <io.h>
#include "softi2c.h"
#include "UART/uart.h"



void I2CInit(void)
{
   /* Setup ports as general I/O */
   I2C_SEL &= ~((I2C_SCL ) | (I2C_SDA) );
   I2C_DIR  |= ((I2C_SCL) | (I2C_SDA));
   I2C_OUT |= ((I2C_SCL) | ( I2C_SDA));
}

/* SDA Falling while SCL stays high */
void I2CStart(void)
{
   /* Turn back to an output after input */
   I2C_DIR |= (I2C_SDA);       
   
   I2C_OUT |= (I2C_SDA);
   I2CDelay(QDELAY);   
   I2C_OUT |= (I2C_SCL);
   I2CDelay(QDELAY);   
   I2C_OUT &= ~(I2C_SDA);
   I2CDelay(QDELAY);
   I2C_OUT &= ~(I2C_SCL);
}


/* SDA Rising while SCL stays high */
void I2CStop(void)
{
   /* Turn back to an output after input */
   I2C_DIR |= (I2C_SDA);      
   
   I2C_OUT &= ~(I2C_SDA);
   I2CDelay(HDELAY);   
   I2C_OUT |= (I2C_SCL);
   I2CDelay(QDELAY);
   I2C_OUT |= (I2C_SDA);
   I2CDelay(HDELAY);
}

void I2CSCL_Clock(void)
{
   I2CDelay(QDELAY);
   I2C_OUT |= (I2C_SCL);
   I2CDelay(HDELAY);
   I2C_OUT &= ~(I2C_SCL);
}

/* Clock one byte out of the I2C port acks are generally LOWs*/
uint8_t I2CTransmit(uint8_t byte)
{
   int8_t i;
   uint8_t ack;
   
   /* Turn back to an output after input */
   I2C_DIR |= (I2C_SDA);    
   
   for( i = 7; i >= 0 ; i-- )
   {
      if(byte & (1 << i))
      {
         I2C_OUT |= (I2C_SDA);  
      }
      else
      {
         I2C_OUT &= ~(I2C_SDA);
      }
      I2CSCL_Clock();
   }
   
   /* Clock one ACKnowledge */
   I2C_DIR &= ~(I2C_SDA);   
   I2C_OUT |= (I2C_SDA);
   
   I2CDelay(HDELAY);   
   I2C_OUT |= (I2C_SCL);
   I2CDelay(HDELAY);
   /* Get the ack bit */
   ack = I2C_IN & (I2C_SDA);

   I2C_OUT &= ~(I2C_SCL); 
   
       
   I2CDelay(HDELAY);
   
   return (ack == 0);
}


void I2CTransmitBlock(uint8_t* block, uint8_t bytes)
{
   while( bytes-- )
   {
      I2CTransmit( *block++ );    
   }
}

void I2CTransmitBlock_C(const uint8_t* block, uint8_t bytes)
{
   while( bytes-- )
   {
      I2CTransmit( *block++ );    
   }   
}


void I2CAddress(uint8_t address, uint8_t rwbit)
{
   I2CStart();
   if( I2CTransmit( address | rwbit ) != ACK_BIT )
   {
      UART_TxString("OMG NO ACK!\r\n");
      /* Error ! */  
   }   
}

uint8_t I2CRead(uint8_t ackBit)
{
   int8_t i;
   uint8_t inByte = 0;
   /* Turn pin into an input */
   I2C_DIR &= ~(I2C_SDA);
      
   for( i = 7; i >= 0 ; i-- )
   {
      I2CDelay(HDELAY);
      I2C_OUT |= (I2C_SCL);
      inByte = inByte << 1;
      if(I2C_IN & (I2C_SDA) )
      {
         inByte = inByte | 1;
      }
      I2CDelay(HDELAY);
      I2C_OUT &= ~(I2C_SCL);
   }
  
   /* Make SDA an output again */
   I2C_DIR |= (I2C_SDA);
   
   if( ackBit )
   {
      I2C_OUT &= ~(I2C_SDA);  
   }
   else
   {
      I2C_OUT |= (I2C_SDA);       
   }

   /* Clock the ACK bit */
   I2CSCL_Clock();
   /* Turn pin into an input */
   I2C_DIR &= ~(I2C_SDA);    
   I2CDelay(QDELAY);
   
   return inByte;
}


/* Reads x bytes into buffer , the user must ensure that there is sufficient
 * space */
void I2CReadBlock(uint8_t* buffer, uint8_t bytes)
{
   /* Send a NACK bit instead of an ACK on the last byte */
	while(bytes--)
	{
		*buffer++ = I2CRead(bytes != 0);
	}   
}



/* For 100kHz use a delay time of 40, 400kHz use 10, short delays use 1 */
/*void I2CDelay(uint8_t delay)
{
   while(delay--)
   {
      nop();  
   }
}*/



