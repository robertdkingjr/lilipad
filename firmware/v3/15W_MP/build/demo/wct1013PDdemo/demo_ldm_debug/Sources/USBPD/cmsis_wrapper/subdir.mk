################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(WCT1013PD)/USBPD/cmsis_wrapper/usb_cmsis_wrapper.c" \

C_SRCS += \
$(WCT1013PD_ESCAPED)/USBPD/cmsis_wrapper/usb_cmsis_wrapper.c \

OBJS += \
./Sources/USBPD/cmsis_wrapper/usb_cmsis_wrapper_c.obj \

OBJS_QUOTED += \
"./Sources/USBPD/cmsis_wrapper/usb_cmsis_wrapper_c.obj" \

C_DEPS += \
./Sources/USBPD/cmsis_wrapper/usb_cmsis_wrapper_c.d \

C_DEPS_QUOTED += \
"./Sources/USBPD/cmsis_wrapper/usb_cmsis_wrapper_c.d" \

OBJS_OS_FORMAT += \
./Sources/USBPD/cmsis_wrapper/usb_cmsis_wrapper_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/USBPD/cmsis_wrapper/usb_cmsis_wrapper_c.obj: $(WCT1013PD_ESCAPED)/USBPD/cmsis_wrapper/usb_cmsis_wrapper.c
	@echo 'Building file: $<'
	@echo 'Executing target #41 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/USBPD/cmsis_wrapper/usb_cmsis_wrapper.args" -o "Sources/USBPD/cmsis_wrapper/usb_cmsis_wrapper_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/USBPD/cmsis_wrapper/usb_cmsis_wrapper_c.d: $(WCT1013PD_ESCAPED)/USBPD/cmsis_wrapper/usb_cmsis_wrapper.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


