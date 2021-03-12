################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(WCT1013PD)/Configure/freemaster_cfg.c" \
"$(WCT1013PD)/Configure/wct_debug.c" \

C_SRCS += \
$(WCT1013PD_ESCAPED)/Configure/freemaster_cfg.c \
$(WCT1013PD_ESCAPED)/Configure/wct_debug.c \

OBJS += \
./Sources/configure/freemaster_cfg_c.obj \
./Sources/configure/wct_debug_c.obj \

OBJS_QUOTED += \
"./Sources/configure/freemaster_cfg_c.obj" \
"./Sources/configure/wct_debug_c.obj" \

C_DEPS += \
./Sources/configure/freemaster_cfg_c.d \
./Sources/configure/wct_debug_c.d \

C_DEPS_QUOTED += \
"./Sources/configure/freemaster_cfg_c.d" \
"./Sources/configure/wct_debug_c.d" \

OBJS_OS_FORMAT += \
./Sources/configure/freemaster_cfg_c.obj \
./Sources/configure/wct_debug_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/configure/freemaster_cfg_c.obj: $(WCT1013PD_ESCAPED)/Configure/freemaster_cfg.c
	@echo 'Building file: $<'
	@echo 'Executing target #21 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/configure/freemaster_cfg.args" -o "Sources/configure/freemaster_cfg_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/configure/freemaster_cfg_c.d: $(WCT1013PD_ESCAPED)/Configure/freemaster_cfg.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/configure/wct_debug_c.obj: $(WCT1013PD_ESCAPED)/Configure/wct_debug.c
	@echo 'Building file: $<'
	@echo 'Executing target #22 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/configure/wct_debug.args" -o "Sources/configure/wct_debug_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/configure/wct_debug_c.d: $(WCT1013PD_ESCAPED)/Configure/wct_debug.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


