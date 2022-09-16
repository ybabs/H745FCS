################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Home/STM32CubeIDE/workspace_1.1.1/baremetal_2/Common/Src/crclib.c \
C:/Users/Home/STM32CubeIDE/workspace_1.1.1/baremetal_2/Common/Src/system_stm32h7xx_dualcore_boot_cm4_cm7.c 

C_DEPS += \
./Common/Src/crclib.d \
./Common/Src/system_stm32h7xx_dualcore_boot_cm4_cm7.d 

OBJS += \
./Common/Src/crclib.o \
./Common/Src/system_stm32h7xx_dualcore_boot_cm4_cm7.o 


# Each subdirectory must supply rules for building sources it contributes
Common/Src/crclib.o: C:/Users/Home/STM32CubeIDE/workspace_1.1.1/baremetal_2/Common/Src/crclib.c Common/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Middlewares/Third_Party/FatFs/src -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Home/STM32CubeIDE/workspace_1.1.1/baremetal_2/Common/Inc" -I"C:/Users/Home/STM32CubeIDE/workspace_1.1.1/baremetal_2/CM7/Filters/Inc" -I"C:/Users/Home/STM32CubeIDE/workspace_1.1.1/baremetal_2/CM7/State_Estimation/Inc" -I"C:/Users/Home/STM32CubeIDE/workspace_1.1.1/baremetal_2/CM7/Buffer_Lib/Inc" -I"C:/Users/Home/STM32CubeIDE/workspace_1.1.1/baremetal_2/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"C:/Users/Home/STM32CubeIDE/workspace_1.1.1/baremetal_2/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"C:/Users/Home/STM32CubeIDE/workspace_1.1.1/baremetal_2/Middlewares/Third_Party/FatFs/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/Src/system_stm32h7xx_dualcore_boot_cm4_cm7.o: C:/Users/Home/STM32CubeIDE/workspace_1.1.1/baremetal_2/Common/Src/system_stm32h7xx_dualcore_boot_cm4_cm7.c Common/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Middlewares/Third_Party/FatFs/src -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Home/STM32CubeIDE/workspace_1.1.1/baremetal_2/Common/Inc" -I"C:/Users/Home/STM32CubeIDE/workspace_1.1.1/baremetal_2/CM7/Filters/Inc" -I"C:/Users/Home/STM32CubeIDE/workspace_1.1.1/baremetal_2/CM7/State_Estimation/Inc" -I"C:/Users/Home/STM32CubeIDE/workspace_1.1.1/baremetal_2/CM7/Buffer_Lib/Inc" -I"C:/Users/Home/STM32CubeIDE/workspace_1.1.1/baremetal_2/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"C:/Users/Home/STM32CubeIDE/workspace_1.1.1/baremetal_2/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"C:/Users/Home/STM32CubeIDE/workspace_1.1.1/baremetal_2/Middlewares/Third_Party/FatFs/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Common-2f-Src

clean-Common-2f-Src:
	-$(RM) ./Common/Src/crclib.d ./Common/Src/crclib.o ./Common/Src/crclib.su ./Common/Src/system_stm32h7xx_dualcore_boot_cm4_cm7.d ./Common/Src/system_stm32h7xx_dualcore_boot_cm4_cm7.o ./Common/Src/system_stm32h7xx_dualcore_boot_cm4_cm7.su

.PHONY: clean-Common-2f-Src

