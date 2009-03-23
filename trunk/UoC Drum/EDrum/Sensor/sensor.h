#ifndef SENSOR_H
#define SENSOR_H


#define SENSOR_OUTPUT	(INCH_A1)

/* Variable Output */
#define SENSOR_OUTPUT2	(INCH_A0)
#define POT_INPUT			(INCH_A3)

#define MAX_CROSSTALK	(1000)
#define DEFAULT_CROSSTALK (12)
#define MIN_CROSSTALK	(0)

typedef struct {

   uint16_t	SensorInputPort;
	int16_t CrosstalkDelay;
} SensorSettings_t;

extern SensorSettings_t* SensorSettings;

void SensorInit(void);
/* Changes the sampling channel to channel, CH0 -> CH15 */
void SensorChannel(uint8_t channel);


/* New Port must be within INCH_A0 -> INCH_Ax */
void SensorInputSelect(uint8_t newPort);
uint8_t GetSensorInput(void);

/* Crosstalk controls the delay between changing the channel */
int16_t GetCrossTalkDelay(void);
void SetCrossTalkDelay(int16_t newCrosstalk);

uint16_t SensorPotValue(void);




#endif
