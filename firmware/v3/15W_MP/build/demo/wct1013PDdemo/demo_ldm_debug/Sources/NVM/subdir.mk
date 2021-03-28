################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(WCT1013PD)/NVM/NVM.c" \

ASM_SRCS += \
$(WCT1013PD)/NVM/EEdata_FlashDefaults.asm \

C_SRCS += \
$(WCT1013PD)/NVM/NVM.c \

ASM_SRCS_QUOTED += \
"$(WCT1013PD)/NVM/EEdata_FlashDefaults.asm" \

OBJS += \
./Sources/NVM/EEdata_FlashDefaults_asm.obj \
./Sources/NVM/NVM_c.obj \

ASM_DEPS += \
./Sources/NVM/EEdata_FlashDefaults_asm.d \

OBJS_QUOTED += \
"./Sources/NVM/EEdata_FlashDefaults_asm.obj" \
"./Sources/NVM/NVM_c.obj" \

C_DEPS += \
./Sources/NVM/NVM_c.d \

ASM_DEPS_QUOTED += \
"./Sources/NVM/EEdata_FlashDefaults_asm.d" \

C_DEPS_QUOTED += \
"./Sources/NVM/NVM_c.d" \

OBJS_OS_FORMAT += \
./Sources/NVM/EEdata_FlashDefaults_asm.obj \
./Sources/NVM/NVM_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/NVM/EEdata_FlashDefaults_asm.obj: $(WCT1013PD)/NVM/EEdata_FlashDefaults.asm
	@echo 'Building file: $<'
	@echo 'Executing target #42 $<'
	@echo 'Invoking: DSC Assembler'
	"$(DSC_ToolsDirEnv)/mwasm56800e" @@"Sources/NVM/EEdata_FlashDefaults.args" -o "Sources/NVM/EEdata_FlashDefaults_asm.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/NVM/EEdata_FlashDefaults_asm.d: $(WCT1013PD)/NVM/EEdata_FlashDefaults.asm
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/NVM/NVM_c.obj: $(WCT1013PD)/NVM/NVM.c
	@echo 'Building file: $<'
	@echo 'Executing target #43 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/NVM/NVM.args" -o "Sources/NVM/NVM_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/NVM/NVM_c.d: $(WCT1013PD)/NVM/NVM.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


