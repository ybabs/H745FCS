################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../State_Estimation/Src/ekf_attitude.c \
../State_Estimation/Src/ekf_pose.c \
../State_Estimation/Src/madgwick.c 

OBJS += \
./State_Estimation/Src/ekf_attitude.o \
./State_Estimation/Src/ekf_pose.o \
./State_Estimation/Src/madgwick.o 

C_DEPS += \
./State_Estimation/Src/ekf_attitude.d \
./State_Estimation/Src/ekf_pose.d \
./State_Estimation/Src/madgwick.d 


# Each subdirectory must supply rules for building sources it contributes
State_Estimation/Src/%.o: ../State_Estimation/Src/%.c State_Estimation/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I"C:/Users/Home/OneDrive/WaffleFlyer/Firmware/Baremetal/Common/Inc" -I"C:/Users/Home/OneDrive/WaffleFlyer/Firmware/Baremetal/CM7/State_Estimation/Inc" -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../FATFS/Target -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../FATFS/App -I../../Middlewares/Third_Party/FatFs/src -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"C:/Users/Home/OneDrive/WaffleFlyer/Firmware/Baremetal/CM7/Control/Inc" -I"C:/Users/Home/OneDrive/WaffleFlyer/Firmware/Baremetal/CM7/Filters/Inc" -I"C:/Users/Home/OneDrive/WaffleFlyer/Firmware/Baremetal/CM7/Buffer_Lib/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-State_Estimation-2f-Src

clean-State_Estimation-2f-Src:
	-$(RM) ./State_Estimation/Src/ekf_attitude.d ./State_Estimation/Src/ekf_attitude.o ./State_Estimation/Src/ekf_pose.d ./State_Estimation/Src/ekf_pose.o ./State_Estimation/Src/madgwick.d ./State_Estimation/Src/madgwick.o

.PHONY: clean-State_Estimation-2f-Src

