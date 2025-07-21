################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/CH395.c \
../Src/CH395CMD.c \
../Src/CH395SPI_HW.c \
../Src/Config_user_define.c \
../Src/Delay.c \
../Src/FS.c \
../Src/HTTPResponders.c \
../Src/HTTPServer.c \
../Src/Lib485.c \
../Src/command_funcs_OTRSP.c \
../Src/commands.c \
../Src/main.c \
../Src/stm32f0xx_hal_msp.c \
../Src/stm32f0xx_it.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32f0xx.c 

OBJS += \
./Src/CH395.o \
./Src/CH395CMD.o \
./Src/CH395SPI_HW.o \
./Src/Config_user_define.o \
./Src/Delay.o \
./Src/FS.o \
./Src/HTTPResponders.o \
./Src/HTTPServer.o \
./Src/Lib485.o \
./Src/command_funcs_OTRSP.o \
./Src/commands.o \
./Src/main.o \
./Src/stm32f0xx_hal_msp.o \
./Src/stm32f0xx_it.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32f0xx.o 

C_DEPS += \
./Src/CH395.d \
./Src/CH395CMD.d \
./Src/CH395SPI_HW.d \
./Src/Config_user_define.d \
./Src/Delay.d \
./Src/FS.d \
./Src/HTTPResponders.d \
./Src/HTTPServer.d \
./Src/Lib485.d \
./Src/command_funcs_OTRSP.d \
./Src/commands.d \
./Src/main.d \
./Src/stm32f0xx_hal_msp.d \
./Src/stm32f0xx_it.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32f0xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g -DUSE_HAL_DRIVER -DDEBUG -DSTM32F030x8 -c -I"E:/Projects/RADIO/Projects/6x2_Antenna_switch/MCU2/Inc" -I"E:/Projects/RADIO/Projects/6x2_Antenna_switch/MCU2/Drivers" -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Inc -I"E:/Projects/RADIO/Projects/6x2_Antenna_switch/MCU2/Drivers/SerialOverTCP" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

