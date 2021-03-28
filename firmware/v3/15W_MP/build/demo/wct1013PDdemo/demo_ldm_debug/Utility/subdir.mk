################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../Utility/DSP56800E_promxram.asm \
../Utility/DSP56800E_zeroBSS.asm \

ASM_SRCS_QUOTED += \
"../Utility/DSP56800E_promxram.asm" \
"../Utility/DSP56800E_zeroBSS.asm" \

OBJS += \
./Utility/DSP56800E_promxram_asm.obj \
./Utility/DSP56800E_zeroBSS_asm.obj \

ASM_DEPS += \
./Utility/DSP56800E_promxram_asm.d \
./Utility/DSP56800E_zeroBSS_asm.d \

OBJS_QUOTED += \
"./Utility/DSP56800E_promxram_asm.obj" \
"./Utility/DSP56800E_zeroBSS_asm.obj" \

ASM_DEPS_QUOTED += \
"./Utility/DSP56800E_promxram_asm.d" \
"./Utility/DSP56800E_zeroBSS_asm.d" \

OBJS_OS_FORMAT += \
./Utility/DSP56800E_promxram_asm.obj \
./Utility/DSP56800E_zeroBSS_asm.obj \


# Each subdirectory must supply rules for building sources it contributes
Utility/DSP56800E_promxram_asm.obj: ../Utility/DSP56800E_promxram.asm
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: DSC Assembler'
	"$(DSC_ToolsDirEnv)/mwasm56800e" @@"Utility/DSP56800E_promxram.args" -o "Utility/DSP56800E_promxram_asm.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Utility/%.d: ../Utility/%.asm
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Utility/DSP56800E_zeroBSS_asm.obj: ../Utility/DSP56800E_zeroBSS.asm
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: DSC Assembler'
	"$(DSC_ToolsDirEnv)/mwasm56800e" @@"Utility/DSP56800E_zeroBSS.args" -o "Utility/DSP56800E_zeroBSS_asm.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '


