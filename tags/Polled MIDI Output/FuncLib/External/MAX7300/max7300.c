
#include "max7300.h"
#include "SoftI2C/softi2c.h"

/* Returns the state of the data in 'reg'*/
uint8_t MAX7300_ReadRegister(uint8_t address, uint8_t reg)
{
   uint8_t readByte;
	I2CAddress(address, WRITE);     
 	I2CTransmit(reg);
	
	I2CAddress(address, READ);
   readByte = I2CRead(NACK_BIT);
   I2CStop();
 	return readByte;   
}

/* Places 'data' into register 'reg' */
void MAX7300_SetRegister(uint8_t address, uint8_t reg, uint8_t data)
{
	I2CAddress(address, WRITE);     
 	I2CTransmit(reg);
 	I2CTransmit(data);
	I2CStop();
}

/* Sends a block of bytes to the MAX7300 */
void MAX7300_SetBlock_C(uint8_t address, const uint8_t* data, uint8_t bytes)
{
	I2CAddress(address, WRITE);
 	I2CTransmitBlock_C(data, bytes);
	I2CStop();
}




