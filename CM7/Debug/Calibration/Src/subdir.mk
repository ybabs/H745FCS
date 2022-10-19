################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Calibration/Src/calibration.cpp 

OBJS += \
./Calibration/Src/calibration.o 

CPP_DEPS += \
./Calibration/Src/calibration.d 


# Each subdirectory must supply rules for building sources it contributes
Calibration/Src/%.o Calibration/Src/%.su: ../Calibration/Src/%.cpp Calibration/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -D__cplusplus=201402L -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Middlewares/Third_Party/FatFs/src -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/Common/Inc" -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/CM7/Filters/Inc" -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/CM7/State_Estimation/Inc" -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/CM7/Buffer_Lib/Inc" -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/Middlewares/Third_Party/FatFs/src" -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/CM7/Calibration/Inc" -I"C:/Users/Daniel/STM32CubeIDE/workspace_1.10.1/baremetal_2/CM7/Eigen" -O3 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -Wextra -Wswitch-enum -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Calibration-2f-Src

clean-Calibration-2f-Src:
	-$(RM) ./Calibration/Src/calibration.d ./Calibration/Src/calibration.o ./Calibration/Src/calibration.su

.PHONY: clean-Calibration-2f-Src

