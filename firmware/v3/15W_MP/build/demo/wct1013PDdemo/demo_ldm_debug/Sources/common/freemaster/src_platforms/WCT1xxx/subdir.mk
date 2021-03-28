################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(COMMON)/56800E/freemaster/src_platforms/WCT1xxx/freemaster_WCT1xxx.c" \
"$(COMMON)/56800E/freemaster/src_platforms/WCT1xxx/freemaster_fastrec.c" \

C_SRCS += \
$(COMMON)/56800E/freemaster/src_platforms/WCT1xxx/freemaster_WCT1xxx.c \
$(COMMON)/56800E/freemaster/src_platforms/WCT1xxx/freemaster_fastrec.c \

OBJS += \
./Sources/common/freemaster/src_platforms/WCT1xxx/freemaster_WCT1xxx_c.obj \
./Sources/common/freemaster/src_platforms/WCT1xxx/freemaster_fastrec_c.obj \

OBJS_QUOTED += \
"./Sources/common/freemaster/src_platforms/WCT1xxx/freemaster_WCT1xxx_c.obj" \
"./Sources/common/freemaster/src_platforms/WCT1xxx/freemaster_fastrec_c.obj" \

C_DEPS += \
./Sources/common/freemaster/src_platforms/WCT1xxx/freemaster_WCT1xxx_c.d \
./Sources/common/freemaster/src_platforms/WCT1xxx/freemaster_fastrec_c.d \

C_DEPS_QUOTED += \
"./Sources/common/freemaster/src_platforms/WCT1xxx/freemaster_WCT1xxx_c.d" \
"./Sources/common/freemaster/src_platforms/WCT1xxx/freemaster_fastrec_c.d" \

OBJS_OS_FORMAT += \
./Sources/common/freemaster/src_platforms/WCT1xxx/freemaster_WCT1xxx_c.obj \
./Sources/common/freemaster/src_platforms/WCT1xxx/freemaster_fastrec_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/common/freemaster/src_platforms/WCT1xxx/freemaster_WCT1xxx_c.obj: $(COMMON)/56800E/freemaster/src_platforms/WCT1xxx/freemaster_WCT1xxx.c
	@echo 'Building file: $<'
	@echo 'Executing target #23 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/common/freemaster/src_platforms/WCT1xxx/freemaster_WCT1xxx.args" -o "Sources/common/freemaster/src_platforms/WCT1xxx/freemaster_WCT1xxx_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/common/freemaster/src_platforms/WCT1xxx/freemaster_WCT1xxx_c.d: $(COMMON)/56800E/freemaster/src_platforms/WCT1xxx/freemaster_WCT1xxx.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/common/freemaster/src_platforms/WCT1xxx/freemaster_fastrec_c.obj: $(COMMON)/56800E/freemaster/src_platforms/WCT1xxx/freemaster_fastrec.c
	@echo 'Building file: $<'
	@echo 'Executing target #24 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/common/freemaster/src_platforms/WCT1xxx/freemaster_fastrec.args" -o "Sources/common/freemaster/src_platforms/WCT1xxx/freemaster_fastrec_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/common/freemaster/src_platforms/WCT1xxx/freemaster_fastrec_c.d: $(COMMON)/56800E/freemaster/src_platforms/WCT1xxx/freemaster_fastrec.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


