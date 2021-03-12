################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(WCT1013PD)/DeviceDriver/PTN5110/usb_pd_ptn5110_connect.c" \
"$(WCT1013PD)/DeviceDriver/PTN5110/usb_pd_ptn5110_hal.c" \
"$(WCT1013PD)/DeviceDriver/PTN5110/usb_pd_ptn5110_interface.c" \
"$(WCT1013PD)/DeviceDriver/PTN5110/usb_pd_ptn5110_msg.c" \

C_SRCS += \
$(WCT1013PD_ESCAPED)/DeviceDriver/PTN5110/usb_pd_ptn5110_connect.c \
$(WCT1013PD_ESCAPED)/DeviceDriver/PTN5110/usb_pd_ptn5110_hal.c \
$(WCT1013PD_ESCAPED)/DeviceDriver/PTN5110/usb_pd_ptn5110_interface.c \
$(WCT1013PD_ESCAPED)/DeviceDriver/PTN5110/usb_pd_ptn5110_msg.c \

OBJS += \
./Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_connect_c.obj \
./Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_hal_c.obj \
./Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_interface_c.obj \
./Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_msg_c.obj \

OBJS_QUOTED += \
"./Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_connect_c.obj" \
"./Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_hal_c.obj" \
"./Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_interface_c.obj" \
"./Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_msg_c.obj" \

C_DEPS += \
./Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_connect_c.d \
./Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_hal_c.d \
./Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_interface_c.d \
./Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_msg_c.d \

C_DEPS_QUOTED += \
"./Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_connect_c.d" \
"./Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_hal_c.d" \
"./Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_interface_c.d" \
"./Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_msg_c.d" \

OBJS_OS_FORMAT += \
./Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_connect_c.obj \
./Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_hal_c.obj \
./Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_interface_c.obj \
./Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_msg_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_connect_c.obj: $(WCT1013PD_ESCAPED)/DeviceDriver/PTN5110/usb_pd_ptn5110_connect.c
	@echo 'Building file: $<'
	@echo 'Executing target #47 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_connect.args" -o "Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_connect_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_connect_c.d: $(WCT1013PD_ESCAPED)/DeviceDriver/PTN5110/usb_pd_ptn5110_connect.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_hal_c.obj: $(WCT1013PD_ESCAPED)/DeviceDriver/PTN5110/usb_pd_ptn5110_hal.c
	@echo 'Building file: $<'
	@echo 'Executing target #48 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_hal.args" -o "Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_hal_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_hal_c.d: $(WCT1013PD_ESCAPED)/DeviceDriver/PTN5110/usb_pd_ptn5110_hal.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_interface_c.obj: $(WCT1013PD_ESCAPED)/DeviceDriver/PTN5110/usb_pd_ptn5110_interface.c
	@echo 'Building file: $<'
	@echo 'Executing target #49 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_interface.args" -o "Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_interface_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_interface_c.d: $(WCT1013PD_ESCAPED)/DeviceDriver/PTN5110/usb_pd_ptn5110_interface.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_msg_c.obj: $(WCT1013PD_ESCAPED)/DeviceDriver/PTN5110/usb_pd_ptn5110_msg.c
	@echo 'Building file: $<'
	@echo 'Executing target #50 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_msg.args" -o "Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_msg_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/DeviceDriver/PTN5110/usb_pd_ptn5110_msg_c.d: $(WCT1013PD_ESCAPED)/DeviceDriver/PTN5110/usb_pd_ptn5110_msg.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


