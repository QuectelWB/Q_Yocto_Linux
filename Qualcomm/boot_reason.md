#开机方式

	
## kernel

	/proc/sys/kernel/boot_reason
	
	1：reset/reboot
	2：SMPL
	3: RTC ALARM
	7: POWER KEY

kernel_dir/Documentations/sysctl/kernel.txt

	...
	 18 Currently, these files might (depending on your configuration)
	 19 show up in /proc/sys/kernel:
	 20
	 21 - acct
	 22 - acpi_video_flags
	 23 - auto_msgmni
	 24 - bootloader_type            [ X86 only ]
	 25 - bootloader_version         [ X86 only ]
	 26 - boot_reason                [ ARM and ARM64 only ]
	 27 - callhome                   [ S390 only ]
	...
	158 boot_reason:
	159
	160 ARM and ARM64 -- reason for device boot
	161
	162 A single bit will be set in the unsigned integer value to identify the
	163 reason the device was booted / powered on. The value will be zero if this
	164 feature is not supported on the ARM device being booted.
	165
	166 See the power-on-status field definitions in
	167 Documentation/arm/msm/boot.txt for Qualcomm's family of devices.
	
Documentation/arm/msm/boot.txt
	
	power_on_status values set by the PMIC for power on event:
	----------------------------------------------------------
	0x01 -- keyboard power on
	0x02 -- RTC alarm
	0x04 -- cable power on
	0x08 -- SMPL
	0x10 -- Watch Dog timeout
	0x20 -- USB charger
	0x40 -- Wall charger
	
		
