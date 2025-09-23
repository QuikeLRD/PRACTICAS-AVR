################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../Keyboard.c \
../LCD.c \
../main.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
Keyboard.o \
LCD.o \
main.o

OBJS_AS_ARGS +=  \
Keyboard.o \
LCD.o \
main.o

C_DEPS +=  \
Keyboard.d \
LCD.d \
main.d

C_DEPS_AS_ARGS +=  \
Keyboard.d \
LCD.d \
main.d

OUTPUT_FILE_PATH +=Keyboard.elf

OUTPUT_FILE_PATH_AS_ARGS +=Keyboard.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler
./Keyboard.o: .././Keyboard.c
	@echo Building file: $<
	@echo Invoking: XC8 C Compiler : 2.36
	$(QUOTE)Q:\Archivosx86\MPLAB Compiler\bin\xc8-cc.exe$(QUOTE)  -mcpu=ATmega16  -mdfp="Q:\Archivosx86\MicrochipStudio7\7.0\Packs\atmel\ATmega_DFP\1.7.374\xc8"  -c -x c -funsigned-char -funsigned-bitfields -mext=cci -D__ATmega16__ -DDEBUG  -Og -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./LCD.o: .././LCD.c
	@echo Building file: $<
	@echo Invoking: XC8 C Compiler : 2.36
	$(QUOTE)Q:\Archivosx86\MPLAB Compiler\bin\xc8-cc.exe$(QUOTE)  -mcpu=ATmega16  -mdfp="Q:\Archivosx86\MicrochipStudio7\7.0\Packs\atmel\ATmega_DFP\1.7.374\xc8"  -c -x c -funsigned-char -funsigned-bitfields -mext=cci -D__ATmega16__ -DDEBUG  -Og -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./main.o: .././main.c
	@echo Building file: $<
	@echo Invoking: XC8 C Compiler : 2.36
	$(QUOTE)Q:\Archivosx86\MPLAB Compiler\bin\xc8-cc.exe$(QUOTE)  -mcpu=ATmega16  -mdfp="Q:\Archivosx86\MicrochipStudio7\7.0\Packs\atmel\ATmega_DFP\1.7.374\xc8"  -c -x c -funsigned-char -funsigned-bitfields -mext=cci -D__ATmega16__ -DDEBUG  -Og -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	




# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking:  XC8 C Linker : 2.36
	$(QUOTE)Q:\Archivosx86\MPLAB Compiler\bin\xc8-cc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -mcpu=ATmega16  -mdfp="Q:\Archivosx86\MicrochipStudio7\7.0\Packs\atmel\ATmega_DFP\1.7.374\xc8"  -Wl,-Map="Keyboard.map" -funsigned-char -funsigned-bitfields -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -Og -ffunction-sections -fdata-sections -fpack-struct -fshort-enums --memorysummary,memoryfile.xml  
	@echo Finished building target: $@
	"Q:\Archivosx86\MPLAB Compiler\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "Keyboard.elf" "Keyboard.hex"
	"Q:\Archivosx86\MPLAB Compiler\bin\avr-objcopy.exe" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O ihex "Keyboard.elf" "Keyboard.eep" || exit 0
	"Q:\Archivosx86\MPLAB Compiler\bin\avr-objdump.exe" -h -S "Keyboard.elf" > "Keyboard.lss"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "Keyboard.elf" "Keyboard.hex" "Keyboard.eep" "Keyboard.map" "Keyboard.usersignatures" "Keyboard.a"  "Keyboard.lss"
	