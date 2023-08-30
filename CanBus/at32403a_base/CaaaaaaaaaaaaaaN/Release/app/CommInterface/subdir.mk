################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/CommInterface/CommInterface.c \
../app/CommInterface/CompileTime.c \
../app/CommInterface/Message.c 

OBJS += \
./app/CommInterface/CommInterface.o \
./app/CommInterface/CompileTime.o \
./app/CommInterface/Message.o 

C_DEPS += \
./app/CommInterface/CommInterface.d \
./app/CommInterface/CompileTime.d \
./app/CommInterface/Message.d 


# Each subdirectory must supply rules for building sources it contributes
app/CommInterface/%.o: ../app/CommInterface/%.c app/CommInterface/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -flto -Werror -Wunused -Wuninitialized -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g -DAT_START_F403A_V1 -DNDEBUG -DAT32F403AVGT7 -DUSE_STDPERIPH_DRIVER -I"../include" -I"../include/libraries/drivers/inc" -I"../include/libraries/cmsis/cm4/core_support" -I"../include/libraries/cmsis/cm4/device_support" -std=c99 -Wmissing-prototypes -Wstrict-prototypes -Wbad-function-cast -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


