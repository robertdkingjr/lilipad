################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(WCT1013PD)/USBPD/osa/usb_osa_bm.c" \

C_SRCS += \
$(WCT1013PD_ESCAPED)/USBPD/osa/usb_osa_bm.c \

OBJS += \
./Sources/USBPD/osa/usb_osa_bm_c.obj \

OBJS_QUOTED += \
"./Sources/USBPD/osa/usb_osa_bm_c.obj" \

C_DEPS += \
./Sources/USBPD/osa/usb_osa_bm_c.d \

C_DEPS_QUOTED += \
"./Sources/USBPD/osa/usb_osa_bm_c.d" \

OBJS_OS_FORMAT += \
./Sources/USBPD/osa/usb_osa_bm_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/USBPD/osa/usb_osa_bm_c.obj: $(WCT1013PD_ESCAPED)/USBPD/osa/usb_osa_bm.c
	@echo 'Building file: $<'
	@echo 'Executing target #40 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/USBPD/osa/usb_osa_bm.args" -o "Sources/USBPD/osa/usb_osa_bm_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/USBPD/osa/usb_osa_bm_c.d: $(WCT1013PD_ESCAPED)/USBPD/osa/usb_osa_bm.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


