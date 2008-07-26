#include <io.h>
#include <stdint.h>
#include "cardreader.h"
#include "MSB2LSB/MSB2LSB.h"

#define CARD_INPUT_PORT    (P5IN)
#define CARD_INPUT_DDR     (P5DIR)

#define CARD_DETECT        (1<<4)
#define CARD_TR1CLOCK      (1<<3)
#define CARD_TR1DATA       (1<<2)

#define CARD_TR2CLOCK      (1<<1)
#define CARD_TR2DATA       (1<<0)

const uint8_t cardCharOffset[] = {' ', '0', ' '};
const uint8_t cardBitLength[] = {7, 5, 7};

const uint8_t cardFieldSize[] = {19,26,4};

uint8_t Card_IsPresent(cardReader_t* cr)
{
   return !((*cr->inputPort) & (cr->detectInput));
}

uint8_t Card_DataState(cardReader_t* cr)
{
   return !((*cr->inputPort) & (cr->dataInput));  
}

uint8_t Card_ClockState(cardReader_t* cr)
{
   return !((*cr->inputPort) & (cr->clockInput));  
}


void Card_Populate(cardInfo_t* card, uint8_t dataIn)
{
   static uint8_t byteCount = 0;
   static uint8_t fieldIndex = 0;
   
   static uint8_t elementIndex = 0;
   
   if( dataIn == 0 )
   {
      byteCount = 0;
      fieldIndex = 0;
      elementIndex = 0;
      return;  
   }
   
   if( dataIn == '^' )
   {
      fieldIndex++;  
      elementIndex = 0;
      byteCount++;
      return;
   }
   
   if( dataIn == ' ' )
   {
      byteCount++;
      return;
   }
   
   switch (fieldIndex )
   {
   
      case CARD_NUMBER:
         if( (byteCount >= 2) && (elementIndex < cardFieldSize[CARD_NUMBER]) )
         {
            card->number[elementIndex++] = dataIn;
            card->number[elementIndex] = 0;
         }
      break;
   
      case CARD_NAME:
         if( (elementIndex < cardFieldSize[CARD_NAME]) )
         {
            card->name[elementIndex++] = dataIn;
            card->name[elementIndex] = 0;
         }
      break;
     
      case CARD_EXPIRY:
         if( (elementIndex < cardFieldSize[CARD_EXPIRY]) )
         {         
            card->expiry[elementIndex++] = dataIn;
            card->expiry[elementIndex] = 0;
         }
      break;
   }
   
   



   byteCount++;
   
   
  
}


/* Basically the Card Init function */
void Card_Init(cardReader_t* cr, uint8_t newTrack)
{
   cr->track = newTrack;
   if( newTrack == TR1 || newTrack == TR3 )
   {
      cr->inputDir = (uint8_t*)(&CARD_INPUT_DDR);
      cr->inputPort = (uint8_t*)(&CARD_INPUT_PORT);
      cr->clockInput = CARD_TR1CLOCK;
      cr->dataInput = CARD_TR1DATA;
   }
   else
   {
      cr->inputDir = (uint8_t*)(&CARD_INPUT_DDR);
      cr->inputPort = (uint8_t*)(&CARD_INPUT_PORT);
      cr->clockInput = CARD_TR2CLOCK;
      cr->dataInput = CARD_TR2DATA;
   }
   
   cr->detectInput = CARD_DETECT;
   /* Set pins as inputs */
   (*cr->inputDir) &= ~( cr->clockInput | cr->dataInput | cr->detectInput);
}

uint8_t CardReadByte(cardReader_t* cr)
{
   uint8_t i = 0;
   uint8_t inByte = 0;
   
   /* Ouch blocking! */
   while( (i < cardBitLength[cr->track]) && Card_IsPresent(cr) )
   {
      if( !Card_ClockState(cr) && cr->clockState  )
      {
         cr->clockState = 0;
      }  
      if( Card_ClockState(cr) && !cr->clockState )
      {
         cr->clockState = 1;
         inByte <<= 1;
         if( Card_DataState(cr) )
         {
            inByte |= 1;
         }
         i++;
      }  
   } 
   return inByte;
}


uint8_t Card_ParseData(cardReader_t* cr, uint8_t data)
{
   return   MSB2LSB( (data & ~(1<<0)) << ( 8 - cardBitLength[ cr->track ] ) ) + cardCharOffset[cr->track] ;
}
