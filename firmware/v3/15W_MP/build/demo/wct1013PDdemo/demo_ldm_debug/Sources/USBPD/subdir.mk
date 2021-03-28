################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(WCT1013PD)/USBPD/usb_pd_connect.c" \
"$(WCT1013PD)/USBPD/usb_pd_interface.c" \
"$(WCT1013PD)/USBPD/usb_pd_msg.c" \
"$(WCT1013PD)/USBPD/usb_pd_policy.c" \
"$(WCT1013PD)/USBPD/usb_pd_timer.c" \

C_SRCS += \
$(WCT1013PD)/USBPD/usb_pd_connect.c \
$(WCT1013PD)/USBPD/usb_pd_interface.c \
$(WCT1013PD)/USBPD/usb_pd_msg.c \
$(WCT1013PD)/USBPD/usb_pd_policy.c \
$(WCT1013PD)/USBPD/usb_pd_timer.c \

OBJS += \
./Sources/USBPD/usb_pd_connect_c.obj \
./Sources/USBPD/usb_pd_interface_c.obj \
./Sources/USBPD/usb_pd_msg_c.obj \
./Sources/USBPD/usb_pd_policy_c.obj \
./Sources/USBPD/usb_pd_timer_c.obj \

OBJS_QUOTED += \
"./Sources/USBPD/usb_pd_connect_c.obj" \
"./Sources/USBPD/usb_pd_interface_c.obj" \
"./Sources/USBPD/usb_pd_msg_c.obj" \
"./Sources/USBPD/usb_pd_policy_c.obj" \
"./Sources/USBPD/usb_pd_timer_c.obj" \

C_DEPS += \
./Sources/USBPD/usb_pd_connect_c.d \
./Sources/USBPD/usb_pd_interface_c.d \
./Sources/USBPD/usb_pd_msg_c.d \
./Sources/USBPD/usb_pd_policy_c.d \
./Sources/USBPD/usb_pd_timer_c.d \

C_DEPS_QUOTED += \
"./Sources/USBPD/usb_pd_connect_c.d" \
"./Sources/USBPD/usb_pd_interface_c.d" \
"./Sources/USBPD/usb_pd_msg_c.d" \
"./Sources/USBPD/usb_pd_policy_c.d" \
"./Sources/USBPD/usb_pd_timer_c.d" \

OBJS_OS_FORMAT += \
./Sources/USBPD/usb_pd_connect_c.obj \
./Sources/USBPD/usb_pd_interface_c.obj \
./Sources/USBPD/usb_pd_msg_c.obj \
./Sources/USBPD/usb_pd_policy_c.obj \
./Sources/USBPD/usb_pd_timer_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/USBPD/usb_pd_connect_c.obj: $(WCT1013PD)/USBPD/usb_pd_connect.c
	@echo 'Building file: $<'
	@echo 'Executing target #35 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/USBPD/usb_pd_connect.args" -o "Sources/USBPD/usb_pd_connect_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/USBPD/usb_pd_connect_c.d: $(WCT1013PD)/USBPD/usb_pd_connect.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/USBPD/usb_pd_interface_c.obj: $(WCT1013PD)/USBPD/usb_pd_interface.c
	@echo 'Building file: $<'
	@echo 'Executing target #36 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/USBPD/usb_pd_interface.args" -o "Sources/USBPD/usb_pd_interface_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/USBPD/usb_pd_interface_c.d: $(WCT1013PD)/USBPD/usb_pd_interface.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/USBPD/usb_pd_msg_c.obj: $(WCT1013PD)/USBPD/usb_pd_msg.c
	@echo 'Building file: $<'
	@echo 'Executing target #37 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/USBPD/usb_pd_msg.args" -o "Sources/USBPD/usb_pd_msg_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/USBPD/usb_pd_msg_c.d: $(WCT1013PD)/USBPD/usb_pd_msg.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/USBPD/usb_pd_policy_c.obj: $(WCT1013PD)/USBPD/usb_pd_policy.c
	@echo 'Building file: $<'
	@echo 'Executing target #38 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/USBPD/usb_pd_policy.args" -o "Sources/USBPD/usb_pd_policy_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/USBPD/usb_pd_policy_c.d: $(WCT1013PD)/USBPD/usb_pd_policy.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/USBPD/usb_pd_timer_c.obj: $(WCT1013PD)/USBPD/usb_pd_timer.c
	@echo 'Building file: $<'
	@echo 'Executing target #39 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Sources/USBPD/usb_pd_timer.args" -o "Sources/USBPD/usb_pd_timer_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/USBPD/usb_pd_timer_c.d: $(WCT1013PD)/USBPD/usb_pd_timer.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


