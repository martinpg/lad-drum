#Makefile template for MSP430
#by Adrian Gin

# Get Time header file 
# Set GETTIME = 1 if you wish for the gettime.h header file to be updated
GETTIME 			= 0

GETTIME_PATH 	= D:/WinAVR/dev/Time/
GETTIME_EXEC	= gettime.exe 
GETTIME.H 		= gettime.h 



# msp430-GCC Options
CC = msp430-gcc
FLAGS = $(WARNINGS) $(DEBUG) $(OPTIMISE) $(CDEFS)

WARNINGS = -Wall
DEBUG = -gstabs
OPTIMISE = -O2

COMPILE = -c
OUTPUT = -o
MMCU = msp430x169
F_CPU = 8000000
# Place -D or -U options here for C sources
CDEFS = -DF_CPU=$(F_CPU)UL
# End of msp430-GCC Options



# msp430 Elf to Hex conversion options
OBJCOPY = msp430-objcopy
OBJCOPY_FLAGS = -O ihex
# End of msp430 Elf to Hex conversion options

# msp430-Linker options
LD = msp430-gcc
LD_FLAGS = $(WARNINGS) $(DEBUG) $(OPTIMISE)
# End of msp430-Linker options

#Size options
SIZE = msp430-size
NAME = msp430-nm

# msp430-Archiver options
AR = msp430-ar
AR_FLAGS = rc
AR_INDEX = msp430-ranlib

LIBRARY_PATH = ./
LIBRARY_NAME = libmylib.a

LIB_PATH = ./libmylib.a

# End of msp430-Archiver options

MSP430-BSL = 1

#msp430-fet programmer
PRG = mspfet
PRG_FLAGS = +ep -BSL=COM2

# msp430-Flash Programmer Options
ifeq ($(MSP430-BSL) , 1)
PRG = msp430-bsl
PRG_FLAGS = -e -r -w --speed=38400 --comport=0
endif
# msp430-Flash Programmer Options 



PERIPHERALS_DIR = ../FuncLib/Peripherals
EXTERNALS_DIR = ../FuncLib/External
APPLICATIONS_DIR = ../FuncLib/Applications
MISC_DIR = ../FuncLib/Other
UTIL_DIR = ../FuncLib/Util

#Source File paths
#Add the full path name to each source under SOURCE_FILES followed by a '\'#

MAIN_FILE = main.c
MAIN_OBJECT = $(MAIN_FILE:.c=.o)

SOURCE_FILES = $(PERIPHERALS_DIR)/UART/uart.c \
               $(PERIPHERALS_DIR)/ADC/adc12.c \
               $(PERIPHERALS_DIR)/SoftI2C/softi2c.c \
               $(PERIPHERALS_DIR)/PWM/pwm.c \
               $(EXTERNALS_DIR)/MAX7300/max7300.c \
               $(UTIL_DIR)/MSB2LSB/MSB2LSB.c \
               $(UTIL_DIR)/Delay/delay.c \
               $(MISC_DIR)/mmculib/uint16toa.c \
					$(MISC_DIR)/mmculib/uint8toa.c \
					$(APPLICATIONS_DIR)/Menu/Menu.c \
					Profiles/profiles.c \
					UserFunctions/userFunctions.c \
					MIDI/midi.c \
               Sample/sample.c \
               UI/UI.c \
               UI_LCD/UI_LCD.c \
               UI_KP/UI_KP.c
					
#Source File paths

#Header and Object File paths
HEADER_FILES = $(SOURCE_FILES:.c=.h)
OBJECTS = $(SOURCE_FILES:.c=.o)
LIB_FILES = J:\Summer\MSPGCC\msp430\lib\libmspgcc.a 
#Header and Object File paths

#Project name:
PROGRAM = main
#Project name
	
#Remove file command#	
RM = rm
#Remove file command#


# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
#     Use forward slashes for directory separators.
#     For a directory that has spaces, enclose it in quotes.
EXTRAINCDIRS = ./ $(PERIPHERALS_DIR) $(EXTERNALS_DIR) $(UTIL_DIR) $(MISC_DIR) $(APPLICATIONS_DIR)


FLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))


# Compiles all source files, links them, then uploads to micro#
# Note that linking depends on the library files produced
# So ensure that $(LIB_PATH) is upto date by invoking >'make lib'
all: compile copy size
	@echo "Make complete!"



# Compile all source files to produce object files #
compile: $(OBJECTS) $(MAIN_OBJECT) 

#Compiling the source files	
%.o : %.h %.c
	@echo "Now compiling" $*.c
	$(CC) $(FLAGS) -mmcu=$(MMCU) $(COMPILE) $*.c $(OUTPUT) $@

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
	$(SIZE) $(LIB_PATH)


# Linking the source files to generate an executable
link: $(PROGRAM).elf
$(PROGRAM).elf: $(OBJECTS) $(MAIN_OBJECT)
	@echo "Linking the object and library files..."
	$(LD) $(LD_FLAGS) -mmcu=$(MMCU) $(MAIN_OBJECT) $(OBJECTS) $(LIB_FILES) $(OUTPUT) $(PROGRAM).elf

# Copy the executable to a hex file
copy: $(PROGRAM).a43 
$(PROGRAM).a43: $(PROGRAM).elf 
	@echo "Ouputting Hex file"
	$(OBJCOPY) $(OBJCOPY_FLAGS) $(PROGRAM).elf $(PROGRAM).a43
#	
	
	
#Upload the hex file to the connected micro	
upload: copy size
	@echo "Uploading hex file to microcontroller..."
	$(PRG) $(PRG_FLAGS) $(PROGRAM).a43 $(PRG_FUSES)


#Compute the size of the hex file
size:
	$(SIZE) $(PROGRAM).elf
#size

#Delete all object files and delete the hex file
clean:
	@echo "Cleaning up objects, elfs and hex files." 
	$(RM) $(OBJECTS) $(MAIN_OBJECT)
	$(RM) *.elf *.a43
#clean

#Erase the connected micro	
erase:
	$(PRG) $(PRG_ERASE_FLAG)
#Erase


	

