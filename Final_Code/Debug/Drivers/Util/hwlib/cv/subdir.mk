################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Neeraj/Workspace/ELEC5620M-Resources/Drivers/Util/hwlib/cv/alt_clock_manager.c 

OBJS += \
./Drivers/Util/hwlib/cv/alt_clock_manager.o 

C_DEPS += \
./Drivers/Util/hwlib/cv/alt_clock_manager.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Util/hwlib/cv/alt_clock_manager.o: C:/Users/Neeraj/Workspace/ELEC5620M-Resources/Drivers/Util/hwlib/cv/alt_clock_manager.c Drivers/Util/hwlib/cv/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Arm C Compiler for Embedded 6'
	armclang.exe --target=arm-arm-none-eabi -mcpu=cortex-a9 -mfpu=none -mfloat-abi=soft -marm -I"C:\Users\Neeraj\Workspace\ELEC5620M-Resources\Drivers" -O0 -g -mno-unaligned-access -MD -MP -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


