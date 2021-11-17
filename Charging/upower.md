#UPower


UPower provides an interface to enumerate power sources on the system and control system-wide power management. Any application can
       access the org.freedesktop.UPower service on the system message bus

	
	root@v-pc:/etc/UPower# upower -e
	/org/freedesktop/UPower/devices/line_power_AC0
	/org/freedesktop/UPower/devices/battery_BAT0
	/org/freedesktop/UPower/devices/DisplayDevice
	root@v-pc:/etc/UPower# upower -i
	root@v-pc:/etc/UPower# upower -i /org/freedesktop/UPower/devices/battery_BAT0
	  native-path:          BAT0
	  vendor:               ASUSTeK
	  model:                ASUS Battery
	  power supply:         yes
	  updated:              Wed 17 Nov 2021 12:39:24 PM +03 (108 seconds ago)
	  has history:          yes
	  has statistics:       yes
	  battery
		present:             yes
		rechargeable:        yes
		state:               pending-charge
		warning-level:       none
		energy:              27.189 Wh
		energy-empty:        0 Wh
		energy-full:         28.169 Wh
		energy-full-design:  48.336 Wh
		energy-rate:         0.0116868 W
		voltage:             11.4 V
		percentage:          96%
		capacity:            58.2775%
		technology:          lithium-ion
		icon-name:          'battery-full-charging-symbolic'



