################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../user/RadarCan.c \
../user/adcInit.c \
../user/at32f403a_407_clock.c \
../user/at32f403a_407_int.c \
../user/canInit.c \
../user/main.c \
../user/nvicInit.c \
../user/pwmInit.c \
../user/timerInit.c \
../user/uartInit.c 

OBJS += \
./user/RadarCan.o \
./user/adcInit.o \
./user/at32f403a_407_clock.o \
./user/at32f403a_407_int.o \
./user/canInit.o \
./user/main.o \
./user/nvicInit.o \
./user/pwmInit.o \
./user/timerInit.o \
./user/uartInit.o 

C_DEPS += \
./user/RadarCan.d \
./user/adcInit.d \
./user/at32f403a_407_clock.d \
./user/at32f403a_407_int.d \
./user/canInit.d \
./user/main.d \
./user/nvicInit.d \
./user/pwmInit.d \
./user/timerInit.d \
./user/uartInit.d 


# Each subdirectory must supply rules for building sources it contributes
user/%.o: ../user/%.c user/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -flto -Werror -Wunused -Wuninitialized -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g -DAT_START_F403A_V1 -DNDEBUG -DAT32F403AVGT7 -DUSE_STDPERIPH_DRIVER -I"../include" -I"../include/libraries/drivers/inc" -I"../include/libraries/cmsis/cm4/core_support" -I"../include/libraries/cmsis/cm4/device_support" -std=c99 -Wmissing-prototypes -Wstrict-prototypes -Wbad-function-cast -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


