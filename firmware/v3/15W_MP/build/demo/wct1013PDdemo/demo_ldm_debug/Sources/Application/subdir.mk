################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(WCT1013PD)/application/systemCallback.c" \
"$(WCT1013PD)/application/systemCommand.c" \
"$(WCT1013PD)/application/systemDebug.c" \
"$(WCT1013PD)/application/systemDisplay.c" \
"$(WCT1013PD)/application/systemPrint.c" \
"$(WCT1013PD)/application/systemProtection.c" \
"$(WCT1013PD)/application/systemQfactor.c" \
"$(WCT1013PD)/application/systemTask.c" \
"$(WCT1013PD)/application/systemTimers.c" \
"$(WCT1013PD)/application/systemUSBPower.c" \

C_SRCS += \
$(WCT1013PD)/application/systemCallback.c \
$(WCT1013PD)/application/systemCommand.c \
$(WCT1013PD)/application/systemDebug.c \
$(WCT1013PD)/application/systemDisplay.c \
$(WCT1013PD)/application/systemPrint.c \
$(WCT1013PD)/application/systemProtection.c \
$(WCT1013PD)/application/systemQfactor.c \
$(WCT1013PD)/application/systemTask.c \
$(WCT1013PD)/application/systemTimers.c \
$(WCT1013PD)/application/systemUSBPower.c \

OBJS += \
./Sources/Application/systemCallback_c.obj \
./Sources/Application/systemCommand_c.obj \
./Sources/Application/systemDebug_c.obj \
./Sources/Application/systemDisplay_c.obj \
./Sources/Application/systemPrint_c.obj \
./Sources/Application/systemProtection_c.obj \
./Sources/Application/systemQfactor_c.obj \
./Sources/Application/systemTask_c.obj \
./Sources/Application/systemTimers_c.obj \
./Sources/Application/systemUSBPower_c.obj \

OBJS_QUOTED += \
"./Sources/Application/systemCallback_c.obj" \
"./Sources/Application/systemCommand_c.obj" \
"./Sources/Application/systemDebug_c.obj" \
"./Sources/Application/systemDisplay_c.obj" \
"./Sources/Application/systemPrint_c.obj" \
"./Sources/Application/systemProtection_c.obj" \
"./Sources/Application/systemQfactor_c.obj" \
"./Sources/Application/systemTask_c.obj" \
"./Sources/Application/systemTimers_c.obj" \
"./Sources/Application/systemUSBPower_c.obj" \

C_DEPS += \
./Sources/Application/systemCallback_c.d \
./Sources/Application/systemCommand_c.d \
./Sources/Application/systemDebug_c.d \
./Sources/Application/systemDisplay_c.d \
./Sources/Application/systemPrint_c.d \
./Sources/Application/systemProtection_c.d \
./Sources/Application/systemQfactor_c.d \
./Sources/Application/systemTask_c.d \
./Sources/Application/systemTimers_c.d \
./Sources/Application/systemUSBPower_c.d \

C_DEPS_QUOTED += \
"./Sources/Application/systemCallback_c.d" \
"./Sources/Application/systemCommand_c.d" \
"./Sources/Application/systemDebug_c.d" \
"./Sources/Application/systemDisplay_c.d" \
"./Sources/Application/systemPrint_c.d" \
"./Sources/Application/systemProtection_c.d" \
"./Sources/Application/systemQfactor_c.d" \
"./Sources/Application/systemTask_c.d" \
"./Sources/Application/systemTimers_c.d" \
"./Sources/Application/systemUSBPower_c.d" \

OBJS_OS_FORMAT += \
./Sources/Application/systemCallback_c.obj \
./Sources/Application/systemCommand_c.obj \
./Sources/Application/systemDebug_c.obj \
./Sources/Application/systemDisplay_c.obj \
./Sources/Application/systemPrint_c.obj \
./Sources/Application/systemProtection_c.obj \
./Sources/Application/systemQfactor_c.obj \
./Sources/Application/systemTask_c.obj \
./Sources/Application/systemTimers_c.obj \
./Sources/Application/systemUSBPower_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/Application/systemCallback_c.obj: $(WCT1013PD)/application/systemCallback.c
	@echo 'Building file: $<'
	@echo 'Executing target #51 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/Application/systemCallback.args" -o "Sources/Application/systemCallback_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Application/systemCallback_c.d: $(WCT1013PD)/application/systemCallback.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/Application/systemCommand_c.obj: $(WCT1013PD)/application/systemCommand.c
	@echo 'Building file: $<'
	@echo 'Executing target #52 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/Application/systemCommand.args" -o "Sources/Application/systemCommand_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Application/systemCommand_c.d: $(WCT1013PD)/application/systemCommand.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/Application/systemDebug_c.obj: $(WCT1013PD)/application/systemDebug.c
	@echo 'Building file: $<'
	@echo 'Executing target #53 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/Application/systemDebug.args" -o "Sources/Application/systemDebug_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Application/systemDebug_c.d: $(WCT1013PD)/application/systemDebug.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/Application/systemDisplay_c.obj: $(WCT1013PD)/application/systemDisplay.c
	@echo 'Building file: $<'
	@echo 'Executing target #54 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/Application/systemDisplay.args" -o "Sources/Application/systemDisplay_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Application/systemDisplay_c.d: $(WCT1013PD)/application/systemDisplay.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/Application/systemPrint_c.obj: $(WCT1013PD)/application/systemPrint.c
	@echo 'Building file: $<'
	@echo 'Executing target #55 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/Application/systemPrint.args" -o "Sources/Application/systemPrint_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Application/systemPrint_c.d: $(WCT1013PD)/application/systemPrint.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/Application/systemProtection_c.obj: $(WCT1013PD)/application/systemProtection.c
	@echo 'Building file: $<'
	@echo 'Executing target #56 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/Application/systemProtection.args" -o "Sources/Application/systemProtection_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Application/systemProtection_c.d: $(WCT1013PD)/application/systemProtection.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/Application/systemQfactor_c.obj: $(WCT1013PD)/application/systemQfactor.c
	@echo 'Building file: $<'
	@echo 'Executing target #57 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/Application/systemQfactor.args" -o "Sources/Application/systemQfactor_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Application/systemQfactor_c.d: $(WCT1013PD)/application/systemQfactor.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/Application/systemTask_c.obj: $(WCT1013PD)/application/systemTask.c
	@echo 'Building file: $<'
	@echo 'Executing target #58 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/Application/systemTask.args" -o "Sources/Application/systemTask_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Application/systemTask_c.d: $(WCT1013PD)/application/systemTask.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/Application/systemTimers_c.obj: $(WCT1013PD)/application/systemTimers.c
	@echo 'Building file: $<'
	@echo 'Executing target #59 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/Application/systemTimers.args" -o "Sources/Application/systemTimers_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Application/systemTimers_c.d: $(WCT1013PD)/application/systemTimers.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/Application/systemUSBPower_c.obj: $(WCT1013PD)/application/systemUSBPower.c
	@echo 'Building file: $<'
	@echo 'Executing target #60 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/Application/systemUSBPower.args" -o "Sources/Application/systemUSBPower_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Application/systemUSBPower_c.d: $(WCT1013PD)/application/systemUSBPower.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


