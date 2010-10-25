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
CFLAGS += -Wall -gdwarf-2 -std=gnu99       -DVERSION_CODE=VERSION_WITHOUT_PE  -DF_CPU=20000000UL -Os -fsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=EdrumAVR.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"M:\Edrum\EdrumAVR\..\FuncLib" -I"M:\Edrum\EdrumAVR\." -I"M:\Edrum\EdrumAVR\..\FuncLib\Peripherals" -I"M:\Edrum\EdrumAVR\..\FuncLib\Other" -I"M:\Edrum\EdrumAVR\..\FuncLib\Applications" -I"M:\Edrum\EdrumAVR\..\FuncLib\External" -I"M:\Edrum\EdrumAVR\..\GenericEdrum" -I"M:\Edrum\EdrumAVR\..\FuncLib\Util" 

## Objects that must be built in order to link
OBJECTS = main.o hardUart.o ControllerMode.o midi.o SysEx.o profiles.o sample.o sensor.o Softtimer.o UI_KP.o UI_LCD.o userFunctions.o LCDSettings.o MenuSettings.o Menu.o uint16toa.o uint8toa.o vumeter.o ringbuffer.o UI.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) EdrumAVR.hex EdrumAVR.eep EdrumAVR.lss size

## Compile
main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

hardUart.o: ../../FuncLib/Peripherals/hardUART/hardUart.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ControllerMode.o: ../../GenericEdrum/ControllerMode/ControllerMode.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

midi.o: ../../GenericEdrum/MIDI/midi.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

SysEx.o: ../../GenericEdrum/MIDI/SysEx/SysEx.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

profiles.o: ../../GenericEdrum/Profiles/profiles.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sample.o: ../../GenericEdrum/Sample/sample.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sensor.o: ../../GenericEdrum/Sensor/sensor.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Softtimer.o: ../../GenericEdrum/SoftTimer/Softtimer.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

UI_KP.o: ../../GenericEdrum/UI_KP/UI_KP.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

UI_LCD.o: ../../GenericEdrum/UI_LCD/UI_LCD.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

userFunctions.o: ../../GenericEdrum/UserFunctions/userFunctions.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

LCDSettings.o: ../../GenericEdrum/LCDSettings.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

MenuSettings.o: ../../GenericEdrum/MenuSettings.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Menu.o: ../../FuncLib/Applications/Menu/Menu.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uint16toa.o: ../../FuncLib/Other/mmculib/uint16toa.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uint8toa.o: ../../FuncLib/Other/mmculib/uint8toa.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

vumeter.o: ../../FuncLib/External/VUMeter/vumeter.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ringbuffer.o: ../../FuncLib/Other/RingBuffer/ringbuffer.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

UI.o: ../../GenericEdrum/UI/UI.c
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
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

