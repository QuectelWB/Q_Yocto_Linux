
#uvc

	
	 apt update
	 apt install -y v4l-utils
	 apt install -y fswebcam
	 v4l2-ctl --list-devices
	 fswebcam -d /dev/video0 --no-banner -r 640x640 -S  5 ./image.jpg
	
	
	root@v-pc:/home/v/Videos# v4l2-ctl --list-devices
	USB2.0 VGA UVC WebCam: USB2.0 V (usb-0000:00:14.0-5):
        	/dev/video0
	
	root@v-pc:/home/v/Videos# lsusb -t
	/:  Bus 02.Port 1: Dev 1, Class=root_hub, Driver=xhci_hcd/4p, 5000M
	/:  Bus 01.Port 1: Dev 1, Class=root_hub, Driver=xhci_hcd/11p, 480M
	    |__ Port 5: Dev 2, If 0, Class=Video, Driver=uvcvideo, 480M
	    |__ Port 5: Dev 2, If 1, Class=Video, Driver=uvcvideo, 480M
	
	[    4.631325] uvcvideo: Found UVC 1.00 device USB2.0 VGA UVC WebCam (04f2:b483)
	[    4.639815] input: USB2.0 VGA UVC WebCam: USB2.0 V as /devices/pci0000:00/0000:00:14.0/usb1/1-5/1-5:1.0/input/input10
	[    4.640480] usbcore: registered new interface driver uvcvideo
	[    4.640481] USB Video Class driver (1.1.1)
	
	v@v-pc:~$ ls /sys/class/video4linux/ -l
	total 0
	lrwxrwxrwx 1 root root 0 Nov 16 10:05 video0 -> ../../devices/pci0000:00/0000:00:14.0/usb1/1-5/1-5:1.0/video4linux/video0
	

