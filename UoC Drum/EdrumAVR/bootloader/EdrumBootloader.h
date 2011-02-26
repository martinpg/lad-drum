#ifndef EDRUM_BOOTLOADER_H
#define EDRUM_BOOTLOADER_H



#define BOOTLOADER_CONDITION (PIND & (1 << 3))

#define	PARITY_MASK		(0x30)
#define	NOPARITY			(0x00)
#define	EVEN				(0x02)
#define	ODD				(0x03)

#define	CHARSIZE_MASK	(0x06)
#define 	BIT8				(0x03)
#define	BIT7				(0x02)
#define	BIT6				(0x01)
#define	BIT5				(0x00)

#define	UCSRCMASK		(0x7F)

void uartInit(void);
void bootloader_Init(void);
void bootloader_leave(void);


#endif
