################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 61
first : all
-include ../makefile.init

# This file contains definitions of environment variables used in the makefiles and .args files if exist.
-include makefile.local

RM := "$(GNU_Make_Install_DirEnv)/rm" -f

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include Utility/subdir.mk
-include Sources/subdir.mk
-include Sources/lib/subdir.mk
-include Sources/driver/subdir.mk
-include Sources/configure/subdir.mk
-include Sources/common/freemaster/src_platforms/WCT1xxx/subdir.mk
-include Sources/common/freemaster/src_common/subdir.mk
-include Sources/USBPD/subdir.mk
-include Sources/USBPD/osa/subdir.mk
-include Sources/USBPD/cmsis_wrapper/subdir.mk
-include Sources/NVM/subdir.mk
-include Sources/HAL/subdir.mk
-include Sources/DeviceDriver/PTN5110/subdir.mk
-include Sources/Application/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
wct1013PDdemo_debug.elf \

EXECUTABLES_QUOTED += \
"wct1013PDdemo_debug.elf" \

EXECUTABLES_OS_FORMAT += \
wct1013PDdemo_debug.elf \


# All Target
all: warning-messages explicit-dependencies wct1013PDdemo_debug.elf

# Tool invocations
wct1013PDdemo_debug.elf: $(OBJS) $(USER_OBJS) ../Project_Settings/Linker_Files/MWCT1013_Internal_PFlash_LDM.cmd
	@echo 'Building target: $@'
	@echo 'Executing target #62 $@'
	@echo 'Invoking: DSC Linker'
	"$(DSC_ToolsDirEnv)/mwld56800e" -o "wct1013PDdemo_debug.elf" @@"wct1013PDdemo_debug.args"  $(USER_OBJS_QUOTED)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(S_DEPS_QUOTED) $(CPP_DEPS_QUOTED) $(CC_DEPS_QUOTED) "./*/*/*.obj" "./*/*/*/*.obj" "./*/*/*/*/*.obj" "./*/*/*/*/*/*.obj" "./*/*.obj"  $(S_UPPER_DEPS_QUOTED) $(CXX_DEPS_QUOTED) "./*/*/*.d" "./*/*.d"  $(ASM_UPPER_DEPS_QUOTED) $(C_UPPER_DEPS_QUOTED) $(C++_DEPS_QUOTED) $(EXECUTABLES_QUOTED) "./*/*/*.d" "./*/*/*/*.d" "./*/*/*/*/*.d" "./*/*/*/*/*/*.d" "./*/*.d"  $(ProjDirPath)/demo_ldm_debug/wct1013PDdemo_debug.elf
	-@echo ' '

.PHONY: all clean dependents explicit-dependencies warning-messages
.SECONDARY:

-include ../makefile.targets

explicit-dependencies: ../Project_Settings/Linker_Files/MWCT1013_Internal_PFlash_LDM.cmd 

warning-messages: 

