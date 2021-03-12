################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(WCT1013PD)/driver/adc.c" \
"$(WCT1013PD)/driver/cmp.c" \
"$(WCT1013PD)/driver/dac.c" \
"$(WCT1013PD)/driver/dma.c" \
"$(WCT1013PD)/driver/flash.c" \
"$(WCT1013PD)/driver/gpio.c" \
"$(WCT1013PD)/driver/i2c.c" \
"$(WCT1013PD)/driver/occs.c" \
"$(WCT1013PD)/driver/pmc.c" \
"$(WCT1013PD)/driver/pwm.c" \
"$(WCT1013PD)/driver/qsci.c" \
"$(WCT1013PD)/driver/sim.c" \
"$(WCT1013PD)/driver/timer.c" \
"$(WCT1013PD)/driver/trigger.c" \
"$(WCT1013PD)/driver/wdg.c" \

C_SRCS += \
$(WCT1013PD_ESCAPED)/driver/adc.c \
$(WCT1013PD_ESCAPED)/driver/cmp.c \
$(WCT1013PD_ESCAPED)/driver/dac.c \
$(WCT1013PD_ESCAPED)/driver/dma.c \
$(WCT1013PD_ESCAPED)/driver/flash.c \
$(WCT1013PD_ESCAPED)/driver/gpio.c \
$(WCT1013PD_ESCAPED)/driver/i2c.c \
$(WCT1013PD_ESCAPED)/driver/occs.c \
$(WCT1013PD_ESCAPED)/driver/pmc.c \
$(WCT1013PD_ESCAPED)/driver/pwm.c \
$(WCT1013PD_ESCAPED)/driver/qsci.c \
$(WCT1013PD_ESCAPED)/driver/sim.c \
$(WCT1013PD_ESCAPED)/driver/timer.c \
$(WCT1013PD_ESCAPED)/driver/trigger.c \
$(WCT1013PD_ESCAPED)/driver/wdg.c \

OBJS += \
./Sources/driver/adc_c.obj \
./Sources/driver/cmp_c.obj \
./Sources/driver/dac_c.obj \
./Sources/driver/dma_c.obj \
./Sources/driver/flash_c.obj \
./Sources/driver/gpio_c.obj \
./Sources/driver/i2c_c.obj \
./Sources/driver/occs_c.obj \
./Sources/driver/pmc_c.obj \
./Sources/driver/pwm_c.obj \
./Sources/driver/qsci_c.obj \
./Sources/driver/sim_c.obj \
./Sources/driver/timer_c.obj \
./Sources/driver/trigger_c.obj \
./Sources/driver/wdg_c.obj \

OBJS_QUOTED += \
"./Sources/driver/adc_c.obj" \
"./Sources/driver/cmp_c.obj" \
"./Sources/driver/dac_c.obj" \
"./Sources/driver/dma_c.obj" \
"./Sources/driver/flash_c.obj" \
"./Sources/driver/gpio_c.obj" \
"./Sources/driver/i2c_c.obj" \
"./Sources/driver/occs_c.obj" \
"./Sources/driver/pmc_c.obj" \
"./Sources/driver/pwm_c.obj" \
"./Sources/driver/qsci_c.obj" \
"./Sources/driver/sim_c.obj" \
"./Sources/driver/timer_c.obj" \
"./Sources/driver/trigger_c.obj" \
"./Sources/driver/wdg_c.obj" \

C_DEPS += \
./Sources/driver/adc_c.d \
./Sources/driver/cmp_c.d \
./Sources/driver/dac_c.d \
./Sources/driver/dma_c.d \
./Sources/driver/flash_c.d \
./Sources/driver/gpio_c.d \
./Sources/driver/i2c_c.d \
./Sources/driver/occs_c.d \
./Sources/driver/pmc_c.d \
./Sources/driver/pwm_c.d \
./Sources/driver/qsci_c.d \
./Sources/driver/sim_c.d \
./Sources/driver/timer_c.d \
./Sources/driver/trigger_c.d \
./Sources/driver/wdg_c.d \

C_DEPS_QUOTED += \
"./Sources/driver/adc_c.d" \
"./Sources/driver/cmp_c.d" \
"./Sources/driver/dac_c.d" \
"./Sources/driver/dma_c.d" \
"./Sources/driver/flash_c.d" \
"./Sources/driver/gpio_c.d" \
"./Sources/driver/i2c_c.d" \
"./Sources/driver/occs_c.d" \
"./Sources/driver/pmc_c.d" \
"./Sources/driver/pwm_c.d" \
"./Sources/driver/qsci_c.d" \
"./Sources/driver/sim_c.d" \
"./Sources/driver/timer_c.d" \
"./Sources/driver/trigger_c.d" \
"./Sources/driver/wdg_c.d" \

OBJS_OS_FORMAT += \
./Sources/driver/adc_c.obj \
./Sources/driver/cmp_c.obj \
./Sources/driver/dac_c.obj \
./Sources/driver/dma_c.obj \
./Sources/driver/flash_c.obj \
./Sources/driver/gpio_c.obj \
./Sources/driver/i2c_c.obj \
./Sources/driver/occs_c.obj \
./Sources/driver/pmc_c.obj \
./Sources/driver/pwm_c.obj \
./Sources/driver/qsci_c.obj \
./Sources/driver/sim_c.obj \
./Sources/driver/timer_c.obj \
./Sources/driver/trigger_c.obj \
./Sources/driver/wdg_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/driver/adc_c.obj: $(WCT1013PD_ESCAPED)/driver/adc.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/driver/adc.args" -o "Sources/driver/adc_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/driver/adc_c.d: $(WCT1013PD_ESCAPED)/driver/adc.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/driver/cmp_c.obj: $(WCT1013PD_ESCAPED)/driver/cmp.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/driver/cmp.args" -o "Sources/driver/cmp_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/driver/cmp_c.d: $(WCT1013PD_ESCAPED)/driver/cmp.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/driver/dac_c.obj: $(WCT1013PD_ESCAPED)/driver/dac.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/driver/dac.args" -o "Sources/driver/dac_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/driver/dac_c.d: $(WCT1013PD_ESCAPED)/driver/dac.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/driver/dma_c.obj: $(WCT1013PD_ESCAPED)/driver/dma.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/driver/dma.args" -o "Sources/driver/dma_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/driver/dma_c.d: $(WCT1013PD_ESCAPED)/driver/dma.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/driver/flash_c.obj: $(WCT1013PD_ESCAPED)/driver/flash.c
	@echo 'Building file: $<'
	@echo 'Executing target #10 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/driver/flash.args" -o "Sources/driver/flash_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/driver/flash_c.d: $(WCT1013PD_ESCAPED)/driver/flash.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/driver/gpio_c.obj: $(WCT1013PD_ESCAPED)/driver/gpio.c
	@echo 'Building file: $<'
	@echo 'Executing target #11 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/driver/gpio.args" -o "Sources/driver/gpio_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/driver/gpio_c.d: $(WCT1013PD_ESCAPED)/driver/gpio.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/driver/i2c_c.obj: $(WCT1013PD_ESCAPED)/driver/i2c.c
	@echo 'Building file: $<'
	@echo 'Executing target #12 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/driver/i2c.args" -o "Sources/driver/i2c_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/driver/i2c_c.d: $(WCT1013PD_ESCAPED)/driver/i2c.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/driver/occs_c.obj: $(WCT1013PD_ESCAPED)/driver/occs.c
	@echo 'Building file: $<'
	@echo 'Executing target #13 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/driver/occs.args" -o "Sources/driver/occs_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/driver/occs_c.d: $(WCT1013PD_ESCAPED)/driver/occs.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/driver/pmc_c.obj: $(WCT1013PD_ESCAPED)/driver/pmc.c
	@echo 'Building file: $<'
	@echo 'Executing target #14 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/driver/pmc.args" -o "Sources/driver/pmc_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/driver/pmc_c.d: $(WCT1013PD_ESCAPED)/driver/pmc.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/driver/pwm_c.obj: $(WCT1013PD_ESCAPED)/driver/pwm.c
	@echo 'Building file: $<'
	@echo 'Executing target #15 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/driver/pwm.args" -o "Sources/driver/pwm_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/driver/pwm_c.d: $(WCT1013PD_ESCAPED)/driver/pwm.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/driver/qsci_c.obj: $(WCT1013PD_ESCAPED)/driver/qsci.c
	@echo 'Building file: $<'
	@echo 'Executing target #16 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/driver/qsci.args" -o "Sources/driver/qsci_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/driver/qsci_c.d: $(WCT1013PD_ESCAPED)/driver/qsci.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/driver/sim_c.obj: $(WCT1013PD_ESCAPED)/driver/sim.c
	@echo 'Building file: $<'
	@echo 'Executing target #17 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/driver/sim.args" -o "Sources/driver/sim_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/driver/sim_c.d: $(WCT1013PD_ESCAPED)/driver/sim.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/driver/timer_c.obj: $(WCT1013PD_ESCAPED)/driver/timer.c
	@echo 'Building file: $<'
	@echo 'Executing target #18 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/driver/timer.args" -o "Sources/driver/timer_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/driver/timer_c.d: $(WCT1013PD_ESCAPED)/driver/timer.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/driver/trigger_c.obj: $(WCT1013PD_ESCAPED)/driver/trigger.c
	@echo 'Building file: $<'
	@echo 'Executing target #19 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/driver/trigger.args" -o "Sources/driver/trigger_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/driver/trigger_c.d: $(WCT1013PD_ESCAPED)/driver/trigger.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/driver/wdg_c.obj: $(WCT1013PD_ESCAPED)/driver/wdg.c
	@echo 'Building file: $<'
	@echo 'Executing target #20 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/driver/wdg.args" -o "Sources/driver/wdg_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/driver/wdg_c.d: $(WCT1013PD_ESCAPED)/driver/wdg.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


