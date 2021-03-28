################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(WCT1013PD)/main.c" \

C_SRCS += \
$(WCT1013PD)/main.c \

OBJS += \
./Sources/main_c.obj \

OBJS_QUOTED += \
"./Sources/main_c.obj" \

C_DEPS += \
./Sources/main_c.d \

C_DEPS_QUOTED += \
"./Sources/main_c.d" \

OBJS_OS_FORMAT += \
./Sources/main_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/main_c.obj: $(WCT1013PD)/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/main.args" -o "Sources/main_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/main_c.d: $(WCT1013PD)/main.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


