################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USB_DEVICE/App/usb_device.c \
../USB_DEVICE/App/usbd_cdc_if.c \
../USB_DEVICE/App/usbd_desc.c 

C_DEPS += \
./USB_DEVICE/App/usb_device.d \
./USB_DEVICE/App/usbd_cdc_if.d \
./USB_DEVICE/App/usbd_desc.d 

OBJS += \
./USB_DEVICE/App/usb_device.o \
./USB_DEVICE/App/usbd_cdc_if.o \
./USB_DEVICE/App/usbd_desc.o 


# Each subdirectory must supply rules for building sources it contributes
USB_DEVICE/App/%.o USB_DEVICE/App/%.su: ../USB_DEVICE/App/%.c USB_DEVICE/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Middlewares/Third_Party/FatFs/src -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/Common/Inc" -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/CM7/Filters/Inc" -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/CM7/State_Estimation/Inc" -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/CM7/Buffer_Lib/Inc" -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/Middlewares/Third_Party/FatFs/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-USB_DEVICE-2f-App

clean-USB_DEVICE-2f-App:
	-$(RM) ./USB_DEVICE/App/usb_device.d ./USB_DEVICE/App/usb_device.o ./USB_DEVICE/App/usb_device.su ./USB_DEVICE/App/usbd_cdc_if.d ./USB_DEVICE/App/usbd_cdc_if.o ./USB_DEVICE/App/usbd_cdc_if.su ./USB_DEVICE/App/usbd_desc.d ./USB_DEVICE/App/usbd_desc.o ./USB_DEVICE/App/usbd_desc.su

.PHONY: clean-USB_DEVICE-2f-App

