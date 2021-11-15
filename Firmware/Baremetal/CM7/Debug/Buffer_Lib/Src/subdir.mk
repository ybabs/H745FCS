################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Buffer_Lib/Src/ring_buffer.c 

OBJS += \
./Buffer_Lib/Src/ring_buffer.o 

C_DEPS += \
./Buffer_Lib/Src/ring_buffer.d 


# Each subdirectory must supply rules for building sources it contributes
Buffer_Lib/Src/%.o: ../Buffer_Lib/Src/%.c Buffer_Lib/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I"C:/Users/Home/OneDrive/WaffleFlyer/Firmware/Baremetal/Common/Inc" -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../FATFS/Target -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../FATFS/App -I../../Middlewares/Third_Party/FatFs/src -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"C:/Users/Home/OneDrive/WaffleFlyer/Firmware/Baremetal/CM7/Control/Inc" -I"C:/Users/Home/OneDrive/WaffleFlyer/Firmware/Baremetal/CM7/Filters/Inc" -I"C:/Users/Home/OneDrive/WaffleFlyer/Firmware/Baremetal/CM7/Buffer_Lib/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

