################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USB_DEVICE/App/usb_device.c \
../USB_DEVICE/App/usbd_cdc_if.c \
../USB_DEVICE/App/usbd_desc.c 

OBJS += \
./USB_DEVICE/App/usb_device.o \
./USB_DEVICE/App/usbd_cdc_if.o \
./USB_DEVICE/App/usbd_desc.o 

C_DEPS += \
./USB_DEVICE/App/usb_device.d \
./USB_DEVICE/App/usbd_cdc_if.d \
./USB_DEVICE/App/usbd_desc.d 


# Each subdirectory must supply rules for building sources it contributes
USB_DEVICE/App/usb_device.o: ../USB_DEVICE/App/usb_device.c USB_DEVICE/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DCORE_CM7 -DDEBUG -DSTM32H745xx -c -I"C:/Users/Home/OneDrive/WaffleFlyer/Firmware/Common/Inc" -I../Core/Inc -I../../Drivers/CMSIS/Include -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/STM32H7xx_HAL_Driver/Inc -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/CMSIS/Include -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Middlewares/Third_Party/FreeRTOS/Source/include -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/STM32H7xx_HAL_Driver/Inc -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"USB_DEVICE/App/usb_device.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
USB_DEVICE/App/usbd_cdc_if.o: ../USB_DEVICE/App/usbd_cdc_if.c USB_DEVICE/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DCORE_CM7 -DDEBUG -DSTM32H745xx -c -I"C:/Users/Home/OneDrive/WaffleFlyer/Firmware/Common/Inc" -I../Core/Inc -I../../Drivers/CMSIS/Include -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/STM32H7xx_HAL_Driver/Inc -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/CMSIS/Include -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Middlewares/Third_Party/FreeRTOS/Source/include -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/STM32H7xx_HAL_Driver/Inc -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"USB_DEVICE/App/usbd_cdc_if.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
USB_DEVICE/App/usbd_desc.o: ../USB_DEVICE/App/usbd_desc.c USB_DEVICE/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DCORE_CM7 -DDEBUG -DSTM32H745xx -c -I"C:/Users/Home/OneDrive/WaffleFlyer/Firmware/Common/Inc" -I../Core/Inc -I../../Drivers/CMSIS/Include -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/STM32H7xx_HAL_Driver/Inc -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/CMSIS/Include -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Middlewares/Third_Party/FreeRTOS/Source/include -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/STM32H7xx_HAL_Driver/Inc -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"USB_DEVICE/App/usbd_desc.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

