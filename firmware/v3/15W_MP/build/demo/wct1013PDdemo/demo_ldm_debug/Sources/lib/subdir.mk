################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(WCT1013PD)/lib/wct_LibParams.c" \
"$(WCT1013PD)/lib/wct_libdata.c" \

C_SRCS += \
$(WCT1013PD)/lib/wct_LibParams.c \
$(WCT1013PD)/lib/wct_libdata.c \

OBJS += \
./Sources/lib/wct_LibParams_c.obj \
./Sources/lib/wct_libdata_c.obj \

OBJS_QUOTED += \
"./Sources/lib/wct_LibParams_c.obj" \
"./Sources/lib/wct_libdata_c.obj" \

C_DEPS += \
./Sources/lib/wct_LibParams_c.d \
./Sources/lib/wct_libdata_c.d \

C_DEPS_QUOTED += \
"./Sources/lib/wct_LibParams_c.d" \
"./Sources/lib/wct_libdata_c.d" \

OBJS_OS_FORMAT += \
./Sources/lib/wct_LibParams_c.obj \
./Sources/lib/wct_libdata_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/lib/wct_LibParams_c.obj: $(WCT1013PD)/lib/wct_LibParams.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/lib/wct_LibParams.args" -o "Sources/lib/wct_LibParams_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/lib/wct_LibParams_c.d: $(WCT1013PD)/lib/wct_LibParams.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/lib/wct_libdata_c.obj: $(WCT1013PD)/lib/wct_libdata.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/lib/wct_libdata.args" -o "Sources/lib/wct_libdata_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/lib/wct_libdata_c.d: $(WCT1013PD)/lib/wct_libdata.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


