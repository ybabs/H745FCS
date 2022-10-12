################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/dma.c \
../Core/Src/gpio.c \
../Core/Src/i2c.c \
../Core/Src/pwm.c \
../Core/Src/sdmmc.c \
../Core/Src/stm32h7xx_hal_msp.c \
../Core/Src/stm32h7xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/tim.c \
../Core/Src/usart.c 

CPP_SRCS += \
../Core/Src/main.cpp \
../Core/Src/micros.cpp \
../Core/Src/sbus.cpp \
../Core/Src/sensors.cpp 

C_DEPS += \
./Core/Src/dma.d \
./Core/Src/gpio.d \
./Core/Src/i2c.d \
./Core/Src/pwm.d \
./Core/Src/sdmmc.d \
./Core/Src/stm32h7xx_hal_msp.d \
./Core/Src/stm32h7xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/tim.d \
./Core/Src/usart.d 

OBJS += \
./Core/Src/dma.o \
./Core/Src/gpio.o \
./Core/Src/i2c.o \
./Core/Src/main.o \
./Core/Src/micros.o \
./Core/Src/pwm.o \
./Core/Src/sbus.o \
./Core/Src/sdmmc.o \
./Core/Src/sensors.o \
./Core/Src/stm32h7xx_hal_msp.o \
./Core/Src/stm32h7xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/tim.o \
./Core/Src/usart.o 

CPP_DEPS += \
./Core/Src/main.d \
./Core/Src/micros.d \
./Core/Src/sbus.d \
./Core/Src/sensors.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Middlewares/Third_Party/FatFs/src -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/Common/Inc" -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/CM7/Filters/Inc" -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/CM7/State_Estimation/Inc" -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/CM7/Buffer_Lib/Inc" -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/Middlewares/Third_Party/FatFs/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.cpp Core/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -D__cplusplus=201402L -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Middlewares/Third_Party/FatFs/src -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/Common/Inc" -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/CM7/Filters/Inc" -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/CM7/State_Estimation/Inc" -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/CM7/Buffer_Lib/Inc" -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/Middlewares/Third_Party/FatFs/src" -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/CM7/Calibration/Inc" -O3 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -Wextra -Wswitch-enum -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/dma.su ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/i2c.d ./Core/Src/i2c.o ./Core/Src/i2c.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/micros.d ./Core/Src/micros.o ./Core/Src/micros.su ./Core/Src/pwm.d ./Core/Src/pwm.o ./Core/Src/pwm.su ./Core/Src/sbus.d ./Core/Src/sbus.o ./Core/Src/sbus.su ./Core/Src/sdmmc.d ./Core/Src/sdmmc.o ./Core/Src/sdmmc.su ./Core/Src/sensors.d ./Core/Src/sensors.o ./Core/Src/sensors.su ./Core/Src/stm32h7xx_hal_msp.d ./Core/Src/stm32h7xx_hal_msp.o ./Core/Src/stm32h7xx_hal_msp.su ./Core/Src/stm32h7xx_it.d ./Core/Src/stm32h7xx_it.o ./Core/Src/stm32h7xx_it.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su

.PHONY: clean-Core-2f-Src

