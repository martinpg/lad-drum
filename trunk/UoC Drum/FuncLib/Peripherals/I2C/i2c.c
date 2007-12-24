/* MSP430 I2C Routines
 * Adrian Gin
 *
 */

#include <io.h>
#include "i2c.h"

/* Master Operation Only */

void i2cInit(void)
{
   /* Configure I/O Pins */
   P3SEL |= 0x0A; // P3.1 & 3.3 for SDA and SCL
   

   /* Disable UART and I2C Operation */
   UCTL0 = (SWRST | SYNC | I2C);
   UCTL0 &= ~(I2CEN);
   
   /* Use Master Mode */
   UCTL0 |= (MST);
   
   /* Set the I2C Speed Rate, select SMCLK */
   I2CTCTL = I2CRM | I2CSSEL_2;
   I2CPSC = 0x00; // Use 8MHz xtal 
   
   /* For 400kHz operation, L = H = 20 clks = 18 */
   /* For 100kHz operation */
   I2CSCLL = 82;
   I2CSCLH = 82;
     
   /* Activate the I2C Module */
   UCTL0 |= I2CEN;
}



/* SCL f = fclk / clkPeriod , so for 8MHz clk, and wanting a 100kHz SCL,
 * clkPeriod = 82 */
void i2cSetBitRate(uint8_t clkPeriod)
{
   UCTL0 &= ~(I2CEN);
   
   I2CSCLL = clkPeriod;
   I2CSCLH = clkPeriod;
   
   UCTL0 |= (I2CEN);
}


void i2cDisable(void)
{
   UCTL0 &= ~(I2C | SYNC | I2CEN);
}


/** i2cAdrress:
 * Tranmits the passed address and RW bit to the TWI Bus lines
 */
void i2cAddress(uint8_t address, uint8_t rw_bit)
{
   I2CSA = address >> 1;
   I2CTCTL &= ~(I2CTRX);
   I2CTCTL |= (I2CSTT | I2CSTB | (rw_bit << 3)); 
   
   
   
}


void i2cTransmit(uint8_t byte)
{


   
   
}



