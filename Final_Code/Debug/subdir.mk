################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Ghost.c \
../PacMan_Control.c \
../about_image.c \
../controls_image.c \
../highscore_image.c \
../main.c \
../maze.c \
../menu_image.c \
../video_frames.c 

OBJS += \
./Ghost.o \
./PacMan_Control.o \
./about_image.o \
./controls_image.o \
./highscore_image.o \
./main.o \
./maze.o \
./menu_image.o \
./video_frames.o 

C_DEPS += \
./Ghost.d \
./PacMan_Control.d \
./about_image.d \
./controls_image.d \
./highscore_image.d \
./main.d \
./maze.d \
./menu_image.d \
./video_frames.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Arm C Compiler for Embedded 6'
	armclang.exe --target=arm-arm-none-eabi -mcpu=cortex-a9 -mfpu=none -mfloat-abi=soft -marm -I"C:\Users\Neeraj\Workspace\ELEC5620M-Resources\Drivers" -O0 -g -mno-unaligned-access -MD -MP -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


