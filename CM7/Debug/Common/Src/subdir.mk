################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Home/STM32CubeIDE/workspace_1.1.0/H745ZI-Q/Common/Src/system_stm32h7xx_dualcore_boot_cm4_cm7.c 

OBJS += \
./Common/Src/system_stm32h7xx_dualcore_boot_cm4_cm7.o 

C_DEPS += \
./Common/Src/system_stm32h7xx_dualcore_boot_cm4_cm7.d 


# Each subdirectory must supply rules for building sources it contributes
Common/Src/system_stm32h7xx_dualcore_boot_cm4_cm7.o: C:/Users/Home/STM32CubeIDE/workspace_1.1.0/H745ZI-Q/Common/Src/system_stm32h7xx_dualcore_boot_cm4_cm7.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DCORE_CM7 -DDEBUG -DSTM32H745xx -c -I"C:/Users/Home/STM32CubeIDE/workspace_1.1.0/H745ZI-Q/Common/Inc" -I../Core/Inc -I../../Drivers/CMSIS/Include -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/STM32H7xx_HAL_Driver/Inc -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/CMSIS/Include -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Middlewares/Third_Party/FreeRTOS/Source/include -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/STM32H7xx_HAL_Driver/Inc -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -IC:/Users/Home/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Common/Src/system_stm32h7xx_dualcore_boot_cm4_cm7.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

