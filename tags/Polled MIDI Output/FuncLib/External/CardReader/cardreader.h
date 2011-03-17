#ifndef _CARD_READER_H
#define _CARD_READER_H



typedef enum {
   TR1 = 0, 
   TR2, 
   TR3
} cardTrack;

typedef enum {
   CARD_NUMBER = 0,
   CARD_NAME,
   CARD_EXPIRY
} fieldIndex_t;

typedef struct cardReader
{
   uint8_t cardPresent;
   cardTrack track;
   
   /* Clock and Data input Pins */
   uint8_t* inputPort;
   uint8_t* inputDir;
   
   uint8_t clockInput;
   uint8_t dataInput;
   uint8_t detectInput;
   
   uint8_t clockState;
   
} cardReader_t;


typedef struct cardInformation
{
   /* Remember to include Nulls */
   uint8_t number[20];
   uint8_t name[27];
   uint8_t expiry[5];

} cardInfo_t;

void Card_Populate(cardInfo_t* card, uint8_t dataIn);
void Card_reset(cardInfo_t* card);

void Card_Init(cardReader_t* cr, uint8_t newTrack);

uint8_t Card_IsPresent(cardReader_t* cr);
uint8_t Card_DataState(cardReader_t* cr);
uint8_t Card_ClockState(cardReader_t* cr);

/* Retrieves one 'byte' from the reader */
uint8_t CardReadByte(cardReader_t* cr);

uint8_t Card_ParseData(cardReader_t* cr, uint8_t data);


#endif
