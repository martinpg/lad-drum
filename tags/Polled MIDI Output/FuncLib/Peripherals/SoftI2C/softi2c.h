#ifndef SOFTI2C_H
#define SOFTI2C_H

#include <io.h>

#define	ACK_BIT		1
#define	NACK_BIT 	0

#define WRITE  0
#define READ   1

#define I2C_IN     P3IN
#define I2C_OUT    P3OUT
#define I2C_DIR    P3DIR
#define I2C_SEL    P3SEL

#define I2C_SCL    (1<<3)
#define I2C_SDA    (1<<1)

#define HDELAY     1
#define QDELAY     1

#define I2C_SCL_HI()   I2C_OUT |= (I2C_SCL)
#define I2C_SCL_LO()   I2C_OUT &= ~(I2C_SCL)
#define I2C_SDA_HI()   I2C_OUT |= (I2C_SDA)
#define I2C_SDA_LO()   I2C_OUT &= ~(I2C_SDA)

#define I2CDelay(x)  nop();
//void I2CDelay(uint8_t delay);

void I2CStart(void);
void I2CStop(void);
void I2CInit(void);



void I2CReadBlock(uint8_t* buffer, uint8_t bytes);
uint8_t I2CRead(uint8_t ackBit);
void I2CAddress(uint8_t address, uint8_t rwbit);
void I2CTransmitBlock(uint8_t* block, uint8_t bytes);
void I2CTransmitBlock_C(const uint8_t* block, uint8_t bytes);

uint8_t I2CTransmit(uint8_t byte);
void I2CSCL_Clock(void);




#endif
