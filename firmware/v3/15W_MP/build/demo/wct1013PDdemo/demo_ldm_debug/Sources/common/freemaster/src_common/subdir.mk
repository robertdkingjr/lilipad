################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(COMMON)/56800E/freemaster/src_common/freemaster_appcmd.c" \
"$(COMMON)/56800E/freemaster/src_common/freemaster_bdm.c" \
"$(COMMON)/56800E/freemaster/src_common/freemaster_can.c" \
"$(COMMON)/56800E/freemaster/src_common/freemaster_pipes.c" \
"$(COMMON)/56800E/freemaster/src_common/freemaster_protocol.c" \
"$(COMMON)/56800E/freemaster/src_common/freemaster_rec.c" \
"$(COMMON)/56800E/freemaster/src_common/freemaster_scope.c" \
"$(COMMON)/56800E/freemaster/src_common/freemaster_serial.c" \
"$(COMMON)/56800E/freemaster/src_common/freemaster_sfio.c" \
"$(COMMON)/56800E/freemaster/src_common/freemaster_tsa.c" \

C_SRCS += \
$(COMMON)/56800E/freemaster/src_common/freemaster_appcmd.c \
$(COMMON)/56800E/freemaster/src_common/freemaster_bdm.c \
$(COMMON)/56800E/freemaster/src_common/freemaster_can.c \
$(COMMON)/56800E/freemaster/src_common/freemaster_pipes.c \
$(COMMON)/56800E/freemaster/src_common/freemaster_protocol.c \
$(COMMON)/56800E/freemaster/src_common/freemaster_rec.c \
$(COMMON)/56800E/freemaster/src_common/freemaster_scope.c \
$(COMMON)/56800E/freemaster/src_common/freemaster_serial.c \
$(COMMON)/56800E/freemaster/src_common/freemaster_sfio.c \
$(COMMON)/56800E/freemaster/src_common/freemaster_tsa.c \

OBJS += \
./Sources/common/freemaster/src_common/freemaster_appcmd_c.obj \
./Sources/common/freemaster/src_common/freemaster_bdm_c.obj \
./Sources/common/freemaster/src_common/freemaster_can_c.obj \
./Sources/common/freemaster/src_common/freemaster_pipes_c.obj \
./Sources/common/freemaster/src_common/freemaster_protocol_c.obj \
./Sources/common/freemaster/src_common/freemaster_rec_c.obj \
./Sources/common/freemaster/src_common/freemaster_scope_c.obj \
./Sources/common/freemaster/src_common/freemaster_serial_c.obj \
./Sources/common/freemaster/src_common/freemaster_sfio_c.obj \
./Sources/common/freemaster/src_common/freemaster_tsa_c.obj \

OBJS_QUOTED += \
"./Sources/common/freemaster/src_common/freemaster_appcmd_c.obj" \
"./Sources/common/freemaster/src_common/freemaster_bdm_c.obj" \
"./Sources/common/freemaster/src_common/freemaster_can_c.obj" \
"./Sources/common/freemaster/src_common/freemaster_pipes_c.obj" \
"./Sources/common/freemaster/src_common/freemaster_protocol_c.obj" \
"./Sources/common/freemaster/src_common/freemaster_rec_c.obj" \
"./Sources/common/freemaster/src_common/freemaster_scope_c.obj" \
"./Sources/common/freemaster/src_common/freemaster_serial_c.obj" \
"./Sources/common/freemaster/src_common/freemaster_sfio_c.obj" \
"./Sources/common/freemaster/src_common/freemaster_tsa_c.obj" \

C_DEPS += \
./Sources/common/freemaster/src_common/freemaster_appcmd_c.d \
./Sources/common/freemaster/src_common/freemaster_bdm_c.d \
./Sources/common/freemaster/src_common/freemaster_can_c.d \
./Sources/common/freemaster/src_common/freemaster_pipes_c.d \
./Sources/common/freemaster/src_common/freemaster_protocol_c.d \
./Sources/common/freemaster/src_common/freemaster_rec_c.d \
./Sources/common/freemaster/src_common/freemaster_scope_c.d \
./Sources/common/freemaster/src_common/freemaster_serial_c.d \
./Sources/common/freemaster/src_common/freemaster_sfio_c.d \
./Sources/common/freemaster/src_common/freemaster_tsa_c.d \

C_DEPS_QUOTED += \
"./Sources/common/freemaster/src_common/freemaster_appcmd_c.d" \
"./Sources/common/freemaster/src_common/freemaster_bdm_c.d" \
"./Sources/common/freemaster/src_common/freemaster_can_c.d" \
"./Sources/common/freemaster/src_common/freemaster_pipes_c.d" \
"./Sources/common/freemaster/src_common/freemaster_protocol_c.d" \
"./Sources/common/freemaster/src_common/freemaster_rec_c.d" \
"./Sources/common/freemaster/src_common/freemaster_scope_c.d" \
"./Sources/common/freemaster/src_common/freemaster_serial_c.d" \
"./Sources/common/freemaster/src_common/freemaster_sfio_c.d" \
"./Sources/common/freemaster/src_common/freemaster_tsa_c.d" \

OBJS_OS_FORMAT += \
./Sources/common/freemaster/src_common/freemaster_appcmd_c.obj \
./Sources/common/freemaster/src_common/freemaster_bdm_c.obj \
./Sources/common/freemaster/src_common/freemaster_can_c.obj \
./Sources/common/freemaster/src_common/freemaster_pipes_c.obj \
./Sources/common/freemaster/src_common/freemaster_protocol_c.obj \
./Sources/common/freemaster/src_common/freemaster_rec_c.obj \
./Sources/common/freemaster/src_common/freemaster_scope_c.obj \
./Sources/common/freemaster/src_common/freemaster_serial_c.obj \
./Sources/common/freemaster/src_common/freemaster_sfio_c.obj \
./Sources/common/freemaster/src_common/freemaster_tsa_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/common/freemaster/src_common/freemaster_appcmd_c.obj: $(COMMON)/56800E/freemaster/src_common/freemaster_appcmd.c
	@echo 'Building file: $<'
	@echo 'Executing target #25 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/common/freemaster/src_common/freemaster_appcmd.args" -o "Sources/common/freemaster/src_common/freemaster_appcmd_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/common/freemaster/src_common/freemaster_appcmd_c.d: $(COMMON)/56800E/freemaster/src_common/freemaster_appcmd.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/common/freemaster/src_common/freemaster_bdm_c.obj: $(COMMON)/56800E/freemaster/src_common/freemaster_bdm.c
	@echo 'Building file: $<'
	@echo 'Executing target #26 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/common/freemaster/src_common/freemaster_bdm.args" -o "Sources/common/freemaster/src_common/freemaster_bdm_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/common/freemaster/src_common/freemaster_bdm_c.d: $(COMMON)/56800E/freemaster/src_common/freemaster_bdm.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/common/freemaster/src_common/freemaster_can_c.obj: $(COMMON)/56800E/freemaster/src_common/freemaster_can.c
	@echo 'Building file: $<'
	@echo 'Executing target #27 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/common/freemaster/src_common/freemaster_can.args" -o "Sources/common/freemaster/src_common/freemaster_can_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/common/freemaster/src_common/freemaster_can_c.d: $(COMMON)/56800E/freemaster/src_common/freemaster_can.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/common/freemaster/src_common/freemaster_pipes_c.obj: $(COMMON)/56800E/freemaster/src_common/freemaster_pipes.c
	@echo 'Building file: $<'
	@echo 'Executing target #28 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/common/freemaster/src_common/freemaster_pipes.args" -o "Sources/common/freemaster/src_common/freemaster_pipes_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/common/freemaster/src_common/freemaster_pipes_c.d: $(COMMON)/56800E/freemaster/src_common/freemaster_pipes.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/common/freemaster/src_common/freemaster_protocol_c.obj: $(COMMON)/56800E/freemaster/src_common/freemaster_protocol.c
	@echo 'Building file: $<'
	@echo 'Executing target #29 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/common/freemaster/src_common/freemaster_protocol.args" -o "Sources/common/freemaster/src_common/freemaster_protocol_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/common/freemaster/src_common/freemaster_protocol_c.d: $(COMMON)/56800E/freemaster/src_common/freemaster_protocol.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/common/freemaster/src_common/freemaster_rec_c.obj: $(COMMON)/56800E/freemaster/src_common/freemaster_rec.c
	@echo 'Building file: $<'
	@echo 'Executing target #30 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/common/freemaster/src_common/freemaster_rec.args" -o "Sources/common/freemaster/src_common/freemaster_rec_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/common/freemaster/src_common/freemaster_rec_c.d: $(COMMON)/56800E/freemaster/src_common/freemaster_rec.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/common/freemaster/src_common/freemaster_scope_c.obj: $(COMMON)/56800E/freemaster/src_common/freemaster_scope.c
	@echo 'Building file: $<'
	@echo 'Executing target #31 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/common/freemaster/src_common/freemaster_scope.args" -o "Sources/common/freemaster/src_common/freemaster_scope_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/common/freemaster/src_common/freemaster_scope_c.d: $(COMMON)/56800E/freemaster/src_common/freemaster_scope.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/common/freemaster/src_common/freemaster_serial_c.obj: $(COMMON)/56800E/freemaster/src_common/freemaster_serial.c
	@echo 'Building file: $<'
	@echo 'Executing target #32 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/common/freemaster/src_common/freemaster_serial.args" -o "Sources/common/freemaster/src_common/freemaster_serial_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/common/freemaster/src_common/freemaster_serial_c.d: $(COMMON)/56800E/freemaster/src_common/freemaster_serial.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/common/freemaster/src_common/freemaster_sfio_c.obj: $(COMMON)/56800E/freemaster/src_common/freemaster_sfio.c
	@echo 'Building file: $<'
	@echo 'Executing target #33 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/common/freemaster/src_common/freemaster_sfio.args" -o "Sources/common/freemaster/src_common/freemaster_sfio_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/common/freemaster/src_common/freemaster_sfio_c.d: $(COMMON)/56800E/freemaster/src_common/freemaster_sfio.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/common/freemaster/src_common/freemaster_tsa_c.obj: $(COMMON)/56800E/freemaster/src_common/freemaster_tsa.c
	@echo 'Building file: $<'
	@echo 'Executing target #34 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/common/freemaster/src_common/freemaster_tsa.args" -o "Sources/common/freemaster/src_common/freemaster_tsa_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/common/freemaster/src_common/freemaster_tsa_c.d: $(COMMON)/56800E/freemaster/src_common/freemaster_tsa.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


