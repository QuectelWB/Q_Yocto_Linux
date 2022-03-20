
drivers/input/input.c  input_register_device函数
	
	input_register_device

	+ if(strcmp(dev->name,"ADS7846 Touchscreen")==0)
	+	dev_set_name(&dev->dev,"input7");
	+ else if(strcmp(dev->name,"gt928 Touchscreen")==0)
	+	dev_set_name(&dev->dev,"input6");
	+ else if(strcmp(dev->name,"eGalax USB Touch Controller")==0)
	+	dev_set_name(&dev->dev,"input8");
	+ else
	+	dev_set_name(&dev->dev,"input%ld",(unsigned log)atmoic_inc_return(&input_no)-1);
		
	error = device_add(&dev->dev);

drivers/input/mousedev.c   mousedev_connect函数
	
	if(strcmp(dev->name,"ADS7846 Touchscreen")==0)
		minor = 7;
	if(strcmp(dev->name,"gt928 Touchscreen")==0)
		minor = 6;
	if(strcmp(dev->name,"eGalax Inc.USB TouchController")==0)
		minor = 8;

	mousedev = mousedev_create(dev,handler,minor);


drivers/input/evdev.c  evdev_connect函数

	
	
	if(strcmp(dev->name,"ADS7846 Touchscreen")==0)
		minor = 7;
	+ if(strcmp(dev->name,"gt928 Touchscreen")==0)
	+		minor = 6;
	+ if(strcmp(dev->name,"eGalax Inc.USB TouchController")==0)
	+		minor = 8;
	
	devdev = kzalloc(sizeof(struct dedev),GFP_KERNEL);
		

		
	/ # 
	/ # cat /proc/bus/input/devices 
	I: Bus=0018 Vendor=0000 Product=0000 Version=0000
	N: Name="gt928 Touchscreen"
	P: Phys=1-0014/input0
	S: Sysfs=/devices/virtual/input/input6
	U: Uniq=
	H: Handlers=mouse6 event6 
	B: EV=b
	B: KEY=400 0 0 0 0 0 0 0 0 0 0
	B: ABS=1000003


	I: Bus=0000 Vendor=0000 Product=0000 Version=0000
	N: Name="ADS7846 Touchscreen"
	P: Phys=spi1.0/input0
	S: Sysfs=/devices/platform/omap2_mcspi.1/spi1.0/input/input7
	U: Uniq=
	H: Handlers=mouse7 event7 
	B: EV=b
	B: KEY=400 0 0 0 0 0 0 0 0 0 0
	B: ABS=1000003


	I: Bus=0003 Vendor=0eef Product=0001 Version=0100
	N: Name="eGalax Inc. USB TouchController"
	P: Phys=usb-ehci-omap.0-1.1/input0
	S: Sysfs=/devices/platform/ehci-omap.0/usb1/1-1/1-1.1/1-1.1:1.0/input/input8
	U: Uniq=
	H: Handlers=mouse8 event8 
	B: EV=b
	B: KEY=400 0 0 0 0 0 0 0 0 0 0
	B: ABS=1000003



https://blog.csdn.net/Auris/article/details/116156047


