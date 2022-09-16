################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../App/Src/mainproj.cpp 

OBJS += \
./App/Src/mainproj.o 

CPP_DEPS += \
./App/Src/mainproj.d 


# Each subdirectory must supply rules for building sources it contributes
App/Src/%.o App/Src/%.su: ../App/Src/%.cpp App/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H745xx -D__cplusplus=201402L -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Home/STM32CubeIDE/workspace_1.1.1/baremetal_2/Common/Inc" -I"C:/Users/Home/STM32CubeIDE/workspace_1.1.1/baremetal_2/CM4/App/Inc" -I"C:/Users/Home/STM32CubeIDE/workspace_1.1.1/baremetal_2/CM4/Core/Src" -O3 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -Wextra -Wswitch-enum -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Src

clean-App-2f-Src:
	-$(RM) ./App/Src/mainproj.d ./App/Src/mainproj.o ./App/Src/mainproj.su

.PHONY: clean-App-2f-Src

