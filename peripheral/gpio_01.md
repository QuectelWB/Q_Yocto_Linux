# GPIO

## 关于GPIO 复用

	/sys/class/gpio # echo 31 > export
	sh: write error: Device or resource busy

这种情况31已经被复用成其他功能了。

	# cat /sys/kernel/debug/gpio
	GPIOs 0-112, platform/1000000.pinctrl, 1000000.pinctrl:
	...
	gpio31  : in  0 2mA no pull
	gpio67  : out 0 2mA no pull
	gpio68  : in  0 2mA pull down
	gpio69  : in  0 2mA pull down
	...

pinctrl系统

	/sys/kernel/debug/pinctrl/1000000.pinctrl # ls
	gpio-ranges     pinconf-groups  pingroups         pinmux-pins
	pinconf-config  pinconf-pins    pinmux-functions  pins

	cat pinmux-pins
	Pinmux settings per pin
	Format: pin (name): mux_owner gpio_owner hog?
	...
	pin 31 (GPIO_31): soc:flashlight 1000000.pinctrl:31 function gpio group gpio31
	pin 32 (GPIO_32): soc:flashlight 1000000.pinctrl:32 function gpio group gpio32
	pin 67 (GPIO_67): (MUX UNCLAIMED) (GPIO UNCLAIMED)
	pin 68 (GPIO_68): (MUX UNCLAIMED) (GPIO UNCLAIMED)
	pin 69 (GPIO_69): (MUX UNCLAIMED) (GPIO UNCLAIMED)
	pin 70 (GPIO_70): (MUX UNCLAIMED) (GPIO UNCLAIMED)
	...

	GPIO_31	GPIO_31	LDO5_1V8 "Flash mode control for Flash LED General GPIO by default"	GP_CLK0	"Can be reassigned as GPIO"


