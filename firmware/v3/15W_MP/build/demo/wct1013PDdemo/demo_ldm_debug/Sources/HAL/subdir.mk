################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(WCT1013PD)/hal/event.c" \
"$(WCT1013PD)/hal/hal.c" \
"$(WCT1013PD)/hal/vectors.c" \

C_SRCS += \
$(WCT1013PD_ESCAPED)/hal/event.c \
$(WCT1013PD_ESCAPED)/hal/hal.c \
$(WCT1013PD_ESCAPED)/hal/vectors.c \

OBJS += \
./Sources/HAL/event_c.obj \
./Sources/HAL/hal_c.obj \
./Sources/HAL/vectors_c.obj \

OBJS_QUOTED += \
"./Sources/HAL/event_c.obj" \
"./Sources/HAL/hal_c.obj" \
"./Sources/HAL/vectors_c.obj" \

C_DEPS += \
./Sources/HAL/event_c.d \
./Sources/HAL/hal_c.d \
./Sources/HAL/vectors_c.d \

C_DEPS_QUOTED += \
"./Sources/HAL/event_c.d" \
"./Sources/HAL/hal_c.d" \
"./Sources/HAL/vectors_c.d" \

OBJS_OS_FORMAT += \
./Sources/HAL/event_c.obj \
./Sources/HAL/hal_c.obj \
./Sources/HAL/vectors_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/HAL/event_c.obj: $(WCT1013PD_ESCAPED)/hal/event.c
	@echo 'Building file: $<'
	@echo 'Executing target #44 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/HAL/event.args" -o "Sources/HAL/event_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/HAL/event_c.d: $(WCT1013PD_ESCAPED)/hal/event.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/HAL/hal_c.obj: $(WCT1013PD_ESCAPED)/hal/hal.c
	@echo 'Building file: $<'
	@echo 'Executing target #45 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/HAL/hal.args" -o "Sources/HAL/hal_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/HAL/hal_c.d: $(WCT1013PD_ESCAPED)/hal/hal.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/HAL/vectors_c.obj: $(WCT1013PD_ESCAPED)/hal/vectors.c
	@echo 'Building file: $<'
	@echo 'Executing target #46 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/HAL/vectors.args" -o "Sources/HAL/vectors_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/HAL/vectors_c.d: $(WCT1013PD_ESCAPED)/hal/vectors.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


