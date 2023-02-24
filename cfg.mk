# DIRECTORIES
WORKSPACE_DIR	:= $(CURDIR)
BUILD_DIR		:= build
CYGWIN_DIR		:= C:\ti\ccs1220\ccs\utils\cygwin
# FILES
SOURCES 	    := $(subst $(WORKSPACE_DIR),.,$(subst \,/,$(shell DIR /B /S *.c)))
OBJECTS			:= $(foreach path,$(SOURCES:%.c=%.obj),build$(subst ./,/,$(path)))
OBJECTS_QUOTED	:= $(foreach path,$(OBJECTS),"$(path)")
RAWS		    := $(OBJECTS:%.obj=%.d_raw)
WARNING_FILE	:= $(BUILD_DIR)\warnings.log
BUILD_DIRS		:= $(sort $(foreach path,$(OBJECTS),$(dir $(path))))
# BINARY CONFIGURATION
OUT = build\ILI9341_MSP430FR2433_DriverLib_Example.out

# MSP430 GCC CONFIGURATION
DEVICE = msp430fr2433
MAP = $(patsubst %.out,%.map, $(OUT))
XML = $(patsubst %.out,%_linkInfo.xml, $(OUT))
CC := C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/bin/cl430
LD := $(CC)
LINKER_FILE := "lnk_msp430fr2433.cmd"
CC_INCLUDES		:= --include_path="C:/Users/marce/Documents/GitHub/ILI9341_MSP430FR2433_DriverLib_Example/src/common" \
	--include_path="C:/Users/marce/Documents/GitHub/ILI9341_MSP430FR2433_DriverLib_Example/src/LcdIf" \
	--include_path="C:/ti/ccs1220/ccs/ccs_base/msp430/include" \
	--include_path="C:/Users/marce/Documents/GitHub/ILI9341_MSP430FR2433_DriverLib_Example" \
	--include_path="C:/Users/marce/Documents/GitHub/ILI9341_MSP430FR2433_DriverLib_Example/src/driverlib/MSP430FR2xx_4xx" \
	--include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/include"

LD_INCLUDES		:= -i"C:/ti/ccs1220/ccs/ccs_base/msp430/include" \
	-i"C:/ti/ccs1220/ccs/ccs_base/msp430/lib/5xx_6xx_FRxx" \
	-i"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/lib" \
	-i"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/include"

LD_LIBS := -llibmath.a -llibc.a
CFLAGS =-vmspx --code_model=small --data_model=small -Ooff --use_hw_mpy=F5 $(CC_INCLUDES) \
	-advice:power=all --advice:hw_config=all --define=__MSP430FR2433__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="$(patsubst %.c,build/%.d_raw, $<)" \
	--obj_directory="build/$(subst .,,$(dir $<))" 2> $(WARNING_FILE)

LFLAGS = -vmspx --code_model=small --data_model=small -Ooff \
	--use_hw_mpy=F5 --advice:power=all --advice:hw_config=all \
	--define=__MSP430FR2433__ -g --printf_support=minimal --diag_warning=225 \
	--diag_wrap=off --display_error_number --silicon_errata=CPU21 \
	--silicon_errata=CPU22 --silicon_errata=CPU40 -z \
	-m"$(MAP)" --heap_size=160 --stack_size=160 --cinit_hold_wdt=on \
	$(LD_INCLUDES) --reread_libs --diag_wrap=off --display_error_number \
	--warn_sections --xml_link_info="$(XML)" \
	--use_hw_mpy=F5 --rom_model

# WINDOWS COMMANDS
RM 		:= $(CYGWIN_DIR)\rm -f
MKDIR 	:= $(CYGWIN_DIR)\mkdir -p