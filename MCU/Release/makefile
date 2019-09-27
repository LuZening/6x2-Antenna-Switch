################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Drivers/STM32F0xx_HAL_Driver/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Antenna\ Switch\ 6x2.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Antenna\ Switch\ 6x2.list \

OBJCOPY_HEX += \
Antenna\ Switch\ 6x2.hex \


# All Target
all: Antenna\ Switch\ 6x2.elf secondary-outputs

# Tool invocations
Antenna\ Switch\ 6x2.elf: $(OBJS) $(USER_OBJS) C:\Users\Zening\OneDrive\RADIO\Projects\6x2_Antenna_switch\MCU\STM32F030F4PX_FLASH.ld
	arm-none-eabi-gcc -o "Antenna Switch 6x2.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m0 -T"C:\Users\Zening\OneDrive\RADIO\Projects\6x2_Antenna_switch\MCU\STM32F030F4PX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Antenna Switch 6x2.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Antenna\ Switch\ 6x2.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Antenna Switch 6x2.list"
	@echo 'Finished building: $@'
	@echo ' '

Antenna\ Switch\ 6x2.hex: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "Antenna Switch 6x2.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
