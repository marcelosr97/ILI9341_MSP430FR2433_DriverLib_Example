# DIRECTORIES
WORKSPACE_DIR	:= $(CURDIR)
BUILD_DIR		:= build
# FILES
SOURCES 	    := $(shell DIR /B /S *.c)
OBJECTS		    := $(SOURCES:%.c=%.o)
WARNING_FILE	:= $(BUILD_DIR)\warnings.log
# BINARY CONFIGURATION
OUT = build\ILI9341_MSP430FR2433_DriverLib_Example.out

# MSP430 GCC CONFIGURATION
DEVICE = msp430fr2433
MAP = $(patsubst %.out,%.map, $(OUT))
GCC_DIR = C:/ti/msp430-gcc/bin
LD_FILES_DIR = C:/ti/msp430-gcc/include
CC      = $(GCC_DIR)/msp430-elf-gcc
GDB     = $(GCC_DIR)/msp430-elf-gdb 
INCLUDES		:= -I"C:/ti/msp430-gcc/msp430-elf/include" \
	-I"C:/ti/ccs1220/ccs/ccs_base/msp430/include_gcc" \
	-I"C:/ti/sp430-gcc/include" \
	-I"C:/Users/marce/Documents/GitHub/ILI9341_MSP430FR2433_DriverLib_Example" \
	-I"C:/Users/marce/Documents/GitHub/ILI9341_MSP430FR2433_DriverLib_Example/driverlib/MSP430FR2xx_4xx" \
	-I"C:/Users/marce/Documents/GitHub/ILI9341_MSP430FR2433_DriverLib_Example/common" \
	-I"C:/Users/marce/Documents/GitHub/ILI9341_MSP430FR2433_DriverLib_Example/LcdIf"

CFLAGS = -I $(LD_FILES_DIR) -mmcu=$(DEVICE) -D__TI_COMPILER_VERSION__ -Og -Wall -g $(INCLUDES) 2> $(WARNING_FILE)
LFLAGS = -L $(LD_FILES_DIR) -mmcu=$(DEVICE) -Wl,-Map,$(MAP),--gc-sections 

# WINDOWS COMMANDS
RM := del /q