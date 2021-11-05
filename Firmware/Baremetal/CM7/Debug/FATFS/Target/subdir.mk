################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FATFS/Target/bsp_driver_sd.c \
../FATFS/Target/sd_diskio.c 

OBJS += \
./FATFS/Target/bsp_driver_sd.o \
./FATFS/Target/sd_diskio.o 

C_DEPS += \
./FATFS/Target/bsp_driver_sd.d \
./FATFS/Target/sd_diskio.d 


# Each subdirectory must supply rules for building sources it contributes
FATFS/Target/%.o: ../FATFS/Target/%.c FATFS/Target/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I../Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../USB_DEVICE/App -I../USB_DEVICE/Target -I"C:/Users/Home/OneDrive/WaffleFlyer/Firmware/Baremetal/Common/Inc" -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

