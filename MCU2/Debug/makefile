################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Drivers/kfifo_DMA/subdir.mk
-include Drivers/SerialOverTCP/subdir.mk
-include Drivers/STM32F0xx_HAL_Driver/Src/subdir.mk
-include Drivers/LiteSHA1/subdir.mk
-include Drivers/Flash_EEPROM/subdir.mk
-include Drivers/Config/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Antenna Switch 6x2 New
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Antenna\ Switch\ 6x2\ New.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Antenna\ Switch\ 6x2\ New.list \

OBJCOPY_HEX += \
Antenna\ Switch\ 6x2\ New.hex \

OBJCOPY_BIN += \
Antenna\ Switch\ 6x2\ New.bin \


# All Target
all: main-build

# Main-build Target
main-build: Antenna\ Switch\ 6x2\ New.elf secondary-outputs

# Tool invocations
Antenna\ Switch\ 6x2\ New.elf: $(OBJS) $(USER_OBJS) E:\Projects\RADIO\Projects\6x2_Antenna_switch\MCU2\STM32F030C8TX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "Antenna Switch 6x2 New.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m0 -T"E:\Projects\RADIO\Projects\6x2_Antenna_switch\MCU2\STM32F030C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Antenna Switch 6x2 New.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Antenna\ Switch\ 6x2\ New.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Antenna Switch 6x2 New.list"
	@echo 'Finished building: $@'
	@echo ' '

Antenna\ Switch\ 6x2\ New.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "Antenna Switch 6x2 New.hex"
	@echo 'Finished building: $@'
	@echo ' '

Antenna\ Switch\ 6x2\ New.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Antenna Switch 6x2 New.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJCOPY_HEX)$(SIZE_OUTPUT)$(OBJDUMP_LIST)$(EXECUTABLES)$(OBJS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(OBJCOPY_BIN) "Antenna Switch 6x2 New.elf"
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
