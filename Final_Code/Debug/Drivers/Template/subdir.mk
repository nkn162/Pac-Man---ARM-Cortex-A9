################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Neeraj/Workspace/ELEC5620M-Resources/Drivers/Template/Template.c 

OBJS += \
./Drivers/Template/Template.o 

C_DEPS += \
./Drivers/Template/Template.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Template/Template.o: C:/Users/Neeraj/Workspace/ELEC5620M-Resources/Drivers/Template/Template.c Drivers/Template/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Arm C Compiler for Embedded 6'
	armclang.exe --target=arm-arm-none-eabi -mcpu=cortex-a9 -mfpu=none -mfloat-abi=soft -marm -I"C:\Users\Neeraj\Workspace\ELEC5620M-Resources\Drivers" -O0 -g -mno-unaligned-access -MD -MP -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


