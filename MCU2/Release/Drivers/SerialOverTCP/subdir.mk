################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/SerialOverTCP/SerialOverTCP.c 

OBJS += \
./Drivers/SerialOverTCP/SerialOverTCP.o 

C_DEPS += \
./Drivers/SerialOverTCP/SerialOverTCP.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/SerialOverTCP/%.o: ../Drivers/SerialOverTCP/%.c Drivers/SerialOverTCP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F030x8 -c -I"E:/Projects/RADIO/Projects/6x2_Antenna_switch/MCU2/Inc" -I"E:/Projects/RADIO/Projects/6x2_Antenna_switch/MCU2/Drivers" -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Inc -I"E:/Projects/RADIO/Projects/6x2_Antenna_switch/MCU2/Drivers/SerialOverTCP" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

