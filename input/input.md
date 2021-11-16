#input 子系统


	root@v-pc:/home/v/udev/input# cat /proc/bus/input/devices
	I: Bus=0019 Vendor=0000 Product=0005 Version=0000
	N: Name="Lid Switch"
	P: Phys=PNP0C0D/button/input0
	S: Sysfs=/devices/LNXSYSTM:00/LNXSYBUS:00/PNP0C0D:00/input/input0
	U: Uniq=
	H: Handlers=event0
	B: PROP=0
	B: EV=21
	B: SW=1

	I: Bus=0019 Vendor=0000 Product=0001 Version=0000
	N: Name="Power Button"
	P: Phys=LNXPWRBN/button/input0
	S: Sysfs=/devices/LNXSYSTM:00/LNXPWRBN:00/input/input2
	U: Uniq=
	H: Handlers=kbd event2
	B: PROP=0
	B: EV=3
	B: KEY=10000000000000 0

	I: Bus=0011 Vendor=0001 Product=0001 Version=ab41
	N: Name="AT Translated Set 2 keyboard"
	P: Phys=isa0060/serio0/input0
	S: Sysfs=/devices/platform/i8042/serio0/input/input3
	U: Uniq=
	H: Handlers=sysrq kbd event3 leds
	B: PROP=0
	B: EV=120013
	B: KEY=402000000 3803078f800d001 feffffdfffefffff fffffffffffffffe
	B: MSC=10
	B: LED=7

	I: Bus=0011 Vendor=0002 Product=000e Version=0000
	N: Name="ETPS/2 Elantech Touchpad"
	P: Phys=isa0060/serio1/input0
	S: Sysfs=/devices/platform/i8042/serio1/input/input5
	U: Uniq=
	H: Handlers=mouse0 event4
	B: PROP=5
	B: EV=b
	B: KEY=e420 10000 0 0 0 0
	B: ABS=661800011000003

	I: Bus=0019 Vendor=1043 Product=0000 Version=0000
	N: Name="Asus Wireless Radio Control"
	P: Phys=asus-wireless/input0
	S: Sysfs=/devices/LNXSYSTM:00/LNXSYBUS:00/ATK4002:00/input/input8
	U: Uniq=
	H: Handlers=rfkill kbd event7
	B: PROP=0
	B: EV=3
	B: KEY=80000000000000 0 0 0

	I: Bus=0003 Vendor=04f2 Product=b483 Version=9914
	N: Name="USB2.0 VGA UVC WebCam: USB2.0 V"
	P: Phys=usb-0000:00:14.0-5/button
	S: Sysfs=/devices/pci0000:00/0000:00:14.0/usb1/1-5/1-5:1.0/input/input10
	U: Uniq=
	H: Handlers=kbd event9
	B: PROP=0
	B: EV=3
	B: KEY=100000 0 0 0

	I: Bus=0000 Vendor=0000 Product=0000 Version=0000
	N: Name="HDA Intel HDMI HDMI/DP,pcm=3"
	P: Phys=ALSA
	S: Sysfs=/devices/pci0000:00/0000:00:03.0/sound/card0/input11
	U: Uniq=
	H: Handlers=event10
	B: PROP=0
	B: EV=21
	B: SW=140

再插入一个鼠标
	
	[ 5926.602579] usb 1-2: new low-speed USB device number 5 using xhci_hcd
	[ 5926.753900] usb 1-2: New USB device found, idVendor=192f, idProduct=0916
	[ 5926.753906] usb 1-2: New USB device strings: Mfr=0, Product=2, SerialNumber=0
	[ 5926.753909] usb 1-2: Product: USB Optical Mouse
	[ 5926.777152] hidraw: raw HID events driver (C) Jiri Kosina
	[ 5926.782309] usbcore: registered new interface driver usbhid
	[ 5926.782311] usbhid: USB HID core driver
	[ 5926.789778] input: USB Optical Mouse as /devices/pci0000:00/0000:00:14.0/usb1/1-2/1-2:1.0/0003:192F:0916.0001/input/input19
	[ 5926.846676] hid-generic 0003:192F:0916.0001: input,hidraw0: USB HID v1.11 Mouse [USB Optical Mouse] on usb-0000:00:14.0-2/input0
	
	
	I: Bus=0003 Vendor=192f Product=0916 Version=0111
	N: Name="USB Optical Mouse"
	P: Phys=usb-0000:00:14.0-2/input0
	S: Sysfs=/devices/pci0000:00/0000:00:14.0/usb1/1-2/1-2:1.0/0003:192F:0916.0001/input/input19
	U: Uniq=
	H: Handlers=mouse1 event16
	B: PROP=0
	B: EV=17
	B: KEY=70000 0 0 0 0
	B: REL=103
	B: MSC=10
	
键盘对应input3


输入字符 a

	root@v-pc:/home/v/udev/input# cat /dev/input/event3 | hexdump
	0000000 70ae 6193 0000 0000 ea23 0008 0000 0000
	0000010 0004 0004 001e 0000 70ae 6193 0000 0000
	0000020 ea23 0008 0000 0000 0001 001e 0001 0000
	0000030 70ae 6193 0000 0000 ea23 0008 0000 0000
	0000040 0000 0000 0000 0000 70ae 6193 0000 0000
	0000050 06b1 000b 0000 0000 0004 0004 001e 0000
	0000060 70ae 6193 0000 0000 06b1 000b 0000 0000
	0000070 0001 001e 0000 0000 70ae 6193 0000 0000
	0000080 06b1 000b 0000 0000 0000 0000 0000 0000
	

	hexdump -C /dev/input/event3

	00001ea0  00 00 00 00 00 00 00 00  11 72 93 61 00 00 00 00  |.........r.a....|
	00001eb0  b3 df 0a 00 00 00 00 00  04 00 04 00 1e 00 00 00  |................|
	00001ec0  11 72 93 61 00 00 00 00  b3 df 0a 00 00 00 00 00  |.r.a............|
	00001ed0  01 00 1e 00 01 00 00 00  11 72 93 61 00 00 00 00  |.........r.a....|
	00001ee0  b3 df 0a 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
	00001ef0  11 72 93 61 00 00 00 00  d8 79 0c 00 00 00 00 00  |.r.a.....y......|
	00001f00  04 00 04 00 1e 00 00 00  11 72 93 61 00 00 00 00  |.........r.a....|
	00001f10  d8 79 0c 00 00 00 00 00  01 00 1e 00 00 00 00 00  |.y..............|
	00001f20  11 72 93 61 00 00 00 00  d8 79 0c 00 00 00 00 00  |.r.a.....y......|


[include/uapi/linux/input-event-codes.h](https://elixir.bootlin.com/linux/latest/source/include/uapi/linux/input-event-codes.h#)







