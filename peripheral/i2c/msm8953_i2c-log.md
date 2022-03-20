#

	
		/sys/bus/i2c/devices/i2c-3/device
		/sys/devices/soc/78b7000.i2c # ls
		driver  driver_override  i2c-3  modalias  power  subsystem  uevent
		/sys/devices/soc/78b7000.i2c # cat modalias
		of:Ni2cT<NULL>Cqcom,i2c-msm-v2
	
这个 modalias 其实是和dtsi 中的compatible 一样的

		i2c_2: i2c@78b6000 { /* BLSP1 QUP2 */
			compatible = "qcom,i2c-msm-v2";
			#address-cells = <1>;
			#size-cells = <0>;
			reg-names = "qup_phys_addr";
			reg = <0x78b6000 0x600>;
			interrupt-names = "qup_irq";
			interrupts = <0 96 0>;


		i2c_3: i2c@78b7000 { /* BLSP1 QUP3 */
			compatible = "qcom,i2c-msm-v2";
			#address-cells = <1>;
			#size-cells = <0>;
			reg-names = "qup_phys_addr";
			reg = <0x78b7000 0x600>;
			interrupt-names = "qup_irq";
			interrupts = <0 97 0>;
			qcom,clk-freq-out = <400000>;

	/sys/devices/soc/78b7000.i2c/i2c-3/3-0014 # ls
	modalias  name  power  subsystem  uevent
	/sys/devices/soc/78b7000.i2c/i2c-3/3-0014 # cat modalias
	i2c:gt1x
	/sys/devices/soc/78b7000.i2c/i2c-3/3-0014 # cat name
	gt1x





	/sys/bus/i2c/drivers # ls -la
	total 0
	drwxr-xr-x. 22 root root 0 Jan  1 01:11 .
	drwxr-xr-x.  4 root root 0 Jan  1 00:00 ..
	drwxr-xr-x.  2 root root 0 Jan  1 01:11 Goodix-TP
	drwxr-xr-x.  2 root root 0 Jan  1 01:11 Goodix-TS
	drwxr-xr-x.  2 root root 0 Jan  1 01:11 Goodix_sec-TP
	drwxr-xr-x.  2 root root 0 Jan  1 01:11 Goodix_sec-TS
	drwxr-xr-x.  2 root root 0 Jan  1 01:11 camera
	drwxr-xr-x.  2 root root 0 Jan  1 01:11 dummy
	drwxr-xr-x.  2 root root 0 Jan  1 01:11 ft5x06_ts
	drwxr-xr-x.  2 root root 0 Jan  1 01:11 fts_sec_ts
	drwxr-xr-x.  2 root root 0 Jan  1 01:11 fts_ts
	drwxr-xr-x.  2 root root 0 Jan  1 01:11 msm_eeprom
	drwxr-xr-x.  2 root root 0 Jan  1 01:11 nq-nci
	drwxr-xr-x.  2 root root 0 Jan  1 01:11 qcom,actuator
	drwxr-xr-x.  2 root root 0 Jan  1 01:11 qcom,led-flash
	drwxr-xr-x.  2 root root 0 Jan  1 01:11 silabs-fm
	drwxr-xr-x.  2 root root 0 Jan  1 01:11 smb1351-charger
	drwxr-xr-x.  2 root root 0 Jan  1 01:11 smb135x-charger
	drwxr-xr-x.  2 root root 0 Jan  1 01:11 tabla-i2c-core
	drwxr-xr-x.  2 root root 0 Jan  1 01:11 tasha-i2c-core
	drwxr-xr-x.  2 root root 0 Jan  1 01:11 wcd9xxx-i2c-core
	drwxr-xr-x.  2 root root 0 Jan  1 01:11 wsa881x-i2c-codec

		
	/sys/bus/i2c/drivers/fts_ts # cd 3-0038/
	/sys/devices/soc/78b7000.i2c/i2c-3/3-0038 # ls
	driver            fts_dump_reg       fts_irq          input      uevent
	fts_boot_mode     fts_force_upgrade  fts_log_level    modalias
	fts_charger_mode  fts_fw_version     fts_rw_reg       name
	fts_cover_mode    fts_glove_mode     fts_touch_point  power
	fts_driver_info   fts_hw_reset       fts_upgrade_bin  subsystem
	/sys/devices/soc/78b7000.i2c/i2c-3/3-0038 # cat name
	fts
	/sys/devices/soc/78b7000.i2c/i2c-3/3-0038 #
	/sys/devices/soc/78b7000.i2c/i2c-3/3-0038 #
	/sys/devices/soc/78b7000.i2c/i2c-3/3-0038 # cat fts_rw_reg
	Read Reg: [00]-[00]
	/sys/devices/soc/78b7000.i2c/i2c-3/3-0038 # cat fts_irq
	irq_depth:0
	/sys/devices/soc/78b7000.i2c/i2c-3/3-0038 # cat fts_dump_reg
	Power Mode:0x01
	FW Ver:0x02
	LCD Initcode Ver:0xff
	Param Ver:0x00
	Param status:0x00
	Vendor ID:0x79
	LCD Busy Number:0x00
	Gesture Mode:0x00
	charge stat:0x00
	INT count:0x00
	ESD count:0x9d

	
	
	I: Bus=0018 Vendor=0000 Product=0000 Version=0000
	N: Name="fts_ts"
	P: Phys=
	S: Sysfs=/devices/soc/78b7000.i2c/i2c-3/3-0038/input/input1
	U: Uniq=
	H: Handlers=mdss_fb mdss_fb kgsl event1
	B: PROP=2
	B: EV=b
	B: KEY=400 0 0 0 0 1000 40000800 0 0 0 0
	B: ABS=6618000 0



		focaltech@38{
			compatible = "focaltech,fts";
			reg = <0x38>;
			interrupt-parent = <&tlmm>;
			interrupts = <65 0x2>;
			focaltech,reset-gpio = <&tlmm 64 0x0>;
			focaltech,irq-gpio = <&tlmm 65 0x02008>;
			focaltech,power-gpio = <&tlmm 0 0x0>;
			vdd_ana-supply = <&pm8953_l10>;
			vcc_i2c-supply  = <&pm8953_l6>;
			focaltech,max-touch-number = <5>;
			focaltech,display-coords =  <0 0 720 1280>;
			pinctrl-names = "pmx_ts_active","pmx_ts_suspend","pmx_ts_release";
			pinctrl-0 = <&ts_int_active &ts_reset_active>;
			pinctrl-1 = <&ts_int_suspend &ts_reset_suspend>;
			pinctrl-2 = <&ts_release>;
			focaltech,have-key;
			focaltech,key-number = <3>;
			focaltech,keys = <139 172 158>;
			focaltech,key-y-coords = <1340 1340 1340>;
			focaltech,key-x-coords = <150 330 660>;
		};


kernel/msm-3.18/drivers/input/touchscreen/focaltech_touch$ grep -rnH "probe"

	focaltech_core.c:1325: static int fts_ts_probe_entry(struct fts_ts_data *ts_data)
	focaltech_core.c:1653: static int fts_ts_probe(struct i2c_client *client, const struct i2c_device_id *id)
	focaltech_core.c:1678:    ret = fts_ts_probe_entry(ts_data);
	focaltech_core.c:1680:        FTS_ERROR("Touch Screen(I2C BUS) driver probe fail");
	focaltech_core.c:1705:    .probe = fts_ts_probe,

	
	[   11.629674] [FTS_TS/I]fts_ts_probe:Touch Screen(I2C BUS) driver prboe...
	[   11.972661] [FTS_TS/I]fts_ts_probe:Touch Screen(I2C BUS) driver prboe successfully
	[   11.990416] [FTS_SEC_TS/I]fts_sec_ts_probe:Touch Screen(I2C BUS) driver prboe...
	[   12.326064] [FTS_SEC_TS/I]fts_sec_ts_probe:Touch Screen(I2C BUS) driver prboe successfully

	/ # i2ctest -B 2 -S 0x38 -R 4
	i2ctest: bus_number [2], slave_addr [0x38]
	i2ctest: write data length [ 0 ]
	i2ctest: write data [ ]
	i2ctest: read data length [ 4 ]
	i2ctest: read data [ ff ff ff ff ]
	i2ctest: exit succeeded


i2c 通信没有问题
	
	
	/ # i2ctest -B 2 -S 0x38 -R 10
	i2ctest: bus_number [2], slave_addr [0x38]
	i2ctest: write data length [ 0 ]
	i2ctest: write data [ ]
	i2ctest: read data length [ 10 ]
	i2ctest: read data [ ff ff ff ff ff 00 01 81 7a 04 ]
	i2ctest: exit succeeded

