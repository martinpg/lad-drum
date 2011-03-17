#Makefile template for..!
#

# Get Time header file 
# Set GETTIME = 1 if you wish for the gettime.h header file to be updated
GETTIME 			= 0

GETTIME_PATH 	= D:/WinAVR/dev/Time/
GETTIME_EXEC	= gettime.exe 
GETTIME.H 		= gettime.h 

# EEPROM Settings, set HAS_EEPROM to 1 if you want
# the $(PROGRAM).eep to be uploaded too
HAS_EEPROM = 0

# AVR-GCC Options
CC = avr-gcc
FLAGS = $(WARNINGS) $(DEBUG) $(OPTIMISE)

WARNINGS = -Wall
DEBUG = -gstabs
OPTIMISE = -Os

COMPILE = -c
OUTPUT = -o
MMCU = atmega8
MMCUid = m8
# End of AVR-GCC Options



# AVR Elf to Hex conversion options
FORMAT = ihex

OBJCOPY = avr-objcopy
OBJCOPY_FLAGS = -O $(FORMAT)
# End of AVR Elf to Hex conversion options

# AVR-Linker options
LD = avr-gcc
LD_FLAGS = $(WARNINGS) $(DEBUG) $(OPTIMISE)
# End of AVR-Linker options

# AVR-Archiver options
AR = avr-ar
AR_FLAGS = rc
AR_INDEX = avr-ranlib

LIBRARY_PATH = D:/WinAVR/dev/
LIBRARY_NAME = libmylib.a

LIB_PATH = D:/WinAVR/dev/libmylib.a

# End of AVR-Archiver options

# AVR-Flash Programmer Options


PRG = avrdude
sp12_FLASH_FLAGS = -wpf $(PROGRAM).hex -T1
sp12_EEPROM_FLAGS = -wef $(PROGRAM).eep
sp12_FLAGS = 
# sp12 Fuses only used for new devices needed to have their
# fuses set
sp12_FUSES = -wF10100100 -T1
sp12_ERASE = -E -T1


#set AVRdude programmer, port, and micro controller */
avrdude_FLAGS = -p $(MMCUid) -c sp12 -P lpt1 -E noreset
# Write and Verify data written to flash/eeprom 
# (Disable verification using -V) flag
avrdude_FLASH_FLAGS = -U flash:w:$(PROGRAM).hex:i 
avrdude_EEPROM_FLAGS = -U eeprom:w:$(PROGRAM).eep:i
avrdude_ERASE = -e

# AVR-Flash Programmer Options 


#Source File paths
#Add the full path name to each source under SOURCE_FILES followed by a '\'#

MAIN_FILE = main.c
MAIN_OBJECT = $(MAIN_FILE:.c=.o)

SOURCE_FILES = ../Pause/pause.c \
					../Uart/Tx.c \
					../Uart/Rx.c \
					../pwm/pwm.c\
					../ADC/adc.c \
					../hardUart/hardUart.c \
					../I2C/i2c.c \
					../DS1624/DS1624.c \
					../DS1307/DS1307.c \
					../serial_ee/serial_ee.c \
					
					

#Source File paths

#Header and Object File paths
HEADER_FILES = $(SOURCE_FILES:.c=.h)
OBJECTS = $(SOURCE_FILES:.c=.o) 
#Header and Object File paths

#Project name:
PROGRAM = main
#Project name
	
#Remove file command#	
RM = rm
#Remove file command#


# Compiles all source files, links them, then uploads to micro#
# Note that linking depends on the library files produced
# So ensure that $(LIB_PATH) is upto date by invoking >'make lib'
all: upload
	@echo "Make complete!"



# Compile all source files to produce object files #
compile: $(LIB_PATH) $(MAIN_OBJECT) 

#Compiling the source files	
%.o : %.h %.c
	@echo "Now compiling" $?
	$(CC) $(FLAGS) $(COMPILE) -mmcu=$(MMCU) $*.c $(OUTPUT) $@

#Evaluate the current time and output a gettime.h file
GETTIME: 
   ifeq ($(GETTIME) , 1)
	@echo "Now updating gettime.h (Time and Date definitions)"
	
#Update the current time by invoking gettime.exe	
	$(GETTIME_PATH)$(GETTIME_EXEC)
   endif

	
	
#Compiling the main() file
$(MAIN_OBJECT): GETTIME $(MAIN_FILE) 
	@echo "Compiling the source file with main()"
	$(CC) $(FLAGS) $(COMPILE) -mmcu=$(MMCU) $*.c $(OUTPUT) $@


#Create a library for the .o files
lib: $(LIB_PATH)
$(LIB_PATH): $(OBJECTS)
	@echo ""
	@echo ""
	@echo "Creating a library for source files excluding the file containing"
	@echo "main() function..."
	@echo ""
	$(AR) $(AR_FLAGS) $(LIB_PATH) $(OBJECTS)
	@echo ""
	@echo "Indexing library...."
	$(AR_INDEX) $(LIB_PATH)


# Linking the source files to generate an executable
link: $(PROGRAM).elf
$(PROGRAM).elf: $(LIB_PATH) $(MAIN_OBJECT)
	@echo "Linking the object and library files..."
	$(LD) $(LD_FLAGS) -mmcu=$(MMCU) $(MAIN_OBJECT) $(LIB_PATH) $(OUTPUT) $@

# Copy the executable to a hex file
copy: $(PROGRAM).hex $(PROGRAM).eep
$(PROGRAM).hex: $(PROGRAM).elf 
	@echo "Ouputting Hex file"
	$(OBJCOPY) $(OBJCOPY_FLAGS) -R .eeprom $< $@
#	
	
	
#Upload the hex file to the connected micro	
upload: copy size

# Programming routines for the specified Programmer
	@echo "Uploading hex data file to microcontroller..."
	$(PRG) $($(PRG)_FLAGS) $($(PRG)_FLASH_FLAGS)
# Program EEPROM only if the HAS_EEPROM flag has been set	
ifeq ($(HAS_EEPROM),1)
	@echo "Uploading eeprom file to microcontroller..."
	$(PRG) $($(PRG)_FLAGS) $($(PRG)_EEPROM_FLAGS)
endif


$(PROGRAM).eep: $(PROGRAM).elf

	@echo "Extracting eeprom contents from elf file"
	$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O $(FORMAT) $< $@

#Compute the size of the hex file
size:
	@echo
	avr-size --mcu=$(MMCU) -C $(PROGRAM).elf
	@echo
#size

#Delete all object files and delete the hex file
clean:
	@echo "Cleaning up objects, elfs and hex files." 
	$(RM) $(OBJECTS) $(MAIN_OBJECT)
	$(RM) *.elf *.hex *.eep
#clean

#Erase the connected micro	
erase:
	$(PRG) $($(PRG)_FLAGS) $($(PRG)_ERASE)
#Erase


	

