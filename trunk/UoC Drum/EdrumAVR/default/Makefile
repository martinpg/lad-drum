###############################################################################
# Makefile for the project EdrumAVR
###############################################################################

## General Flags
PROJECT = EdrumAVR
MCU = atmega32
TARGET = EdrumAVR.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99                                                        -DF_CPU=20000000UL -Os -fsigned-char
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=EdrumAVR.map
LDFLAGS += -Wl,-section-start=.profile1=0x7600


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"M:\Robotics\Source\EdrumAVR\..\FuncLib" -I"M:\Robotics\Source\EdrumAVR\..\FuncLib\Peripherals" -I"M:\Robotics\Source\EdrumAVR\..\FuncLib\Other" -I"M:\Robotics\Source\EdrumAVR\..\GenericEdrum" -I"M:\Robotics\Source\EdrumAVR\..\FuncLib\Applications" -I"M:\Robotics\Source\EdrumAVR\..\FuncLib\External" -I"M:\Robotics\Source\EdrumAVR\." 

## Objects that must be built in order to link
OBJECTS = EDrumAVRMega.o hardUart.o ringbuffer.o UI_LCD.o LCDSettings.o spi.o uint8toa.o UI_KP.o sample.o sensor.o adc.o vumeter.o MenuSettings.o Menu.o userFunctions.o softtimer.o midi.o profiles.o SysEx.o hardwareSpecific.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) EdrumAVR.hex EdrumAVR.eep EdrumAVR.lss size

## Compile
EDrumAVRMega.o: ../EDrumAVRMega.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

hardUart.o: ../../FuncLib/Peripherals/hardUART/hardUart.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ringbuffer.o: ../../FuncLib/Other/RingBuffer/ringbuffer.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

UI_LCD.o: ../../FuncLib/External/UI_LCD/UI_LCD.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

LCDSettings.o: ../LCDSettings.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

spi.o: ../../FuncLib/Peripherals/SPI/spi.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uint8toa.o: ../../FuncLib/Other/mmculib/uint8toa.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

UI_KP.o: ../UI_KP.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sample.o: ../../GenericEdrum/Sample/sample.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sensor.o: ../../GenericEdrum/Sensor/sensor.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

adc.o: ../../FuncLib/Peripherals/avrADC/adc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

vumeter.o: ../../FuncLib/External/VUMeter/vumeter.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

MenuSettings.o: ../../GenericEdrum/MenuSettings.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Menu.o: ../../FuncLib/Applications/Menu/Menu.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

userFunctions.o: ../../GenericEdrum/UserFunctions/userFunctions.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

softtimer.o: ../../GenericEdrum/SoftTimer/softtimer.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

midi.o: ../../GenericEdrum/MIDI/midi.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

profiles.o: ../../GenericEdrum/Profiles/profiles.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

SysEx.o: ../../GenericEdrum/MIDI/SysEx/SysEx.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

hardwareSpecific.o: ../hardwareSpecific.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) EdrumAVR.elf dep/* EdrumAVR.hex EdrumAVR.eep EdrumAVR.lss EdrumAVR.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

