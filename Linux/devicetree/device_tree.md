#设备树


https://www.cnblogs.com/pengdonglin137/p/4495056.html

	
	make menuconfig ---> 
		Boot options ---> 
			Flattened Device Tree support
	
	make dtbs
	
	make scripts	// 得到dtc
	
	android:	
	/out/host/linux-x86/bin/dtc  -I dtb -O dts 
		\ ./out/target/product/msm8937_32/obj/KERNEL_OBJ/arch/arm/boot/dts/qcom/qcm2150.dtb	\
	 	> qcm2150_xxxou.dtsi 

## 反编译dtc

假设我们最终的设备树文件是：arch/arm/boot/dts/s3c2416-smdk2416.dtb

		
	./scripts/dtc/dtc -I dtb -O dts -o output.dts arch/arm/boot/dts/s3c2416-smdk2416.dtb

输出文件output.dts就是反汇编的结果，他就是实际生成的设备树，当然也可以重新将生成的dts文件在编译成dtb文件：
	
	./scripts/dtc/dtc -I dts -O dtb -o s3c2416-smdk2416.dtb output.dts
	
## fdtdump

scripts/dtc目录下除了提供dtc工具外，还提供了一个叫做fdtdump的工具.

	fdtdump -sd vexpress-v2p-ca9.dtb | tee vexpress.dts.debug

看看输出的文件：

	vexpress-v2p-ca9.dtb: found fdt at offset 0
	/dts-v1/;
	// magic:        0xd00dfeed
	// totalsize:        0x2e9c (11932)
	// off_dt_struct:    0x38
	// off_dt_strings:    0x2bb8
	// off_mem_rsvmap:    0x28
	// version:        17
	// last_comp_version:    16
	// boot_cpuid_phys:    0x0
	// size_dt_strings:    0x2e4
	// size_dt_struct:    0x2b80

	// 0038: tag: 0x00000001 (FDT_BEGIN_NODE)
	/ {
	// 0040: tag: 0x00000003 (FDT_PROP)
	// 2bb8: string: model
	// 004c: value
	    model = "V2P-CA9";

可以看到，其中这个工具会同时把分析语句和原始文件打印出来，其中 // 后输出的是分析语句，其他的是原始语句.
	
	 fdtdump -sd ../04_dtbdump_Qualcomm_Technologies,_Inc._MSM8909_QRD_SKUE.dtb  > qrd_skue.sd.dts

	// 2e066: string: qcom,tchg-mins
	// 19418: value
                    qcom,tchg-mins = <0x0000001e>;

	fdtdump ../04_dtbdump_Qualcomm_Technologies,_Inc._MSM8909_QRD_SKUE.dtb > qrd_skue.dts
	/
	soc{
		qcom,spmi@200f000 {
	 		qcom,pm8909@0{
	 			qcom,charger {
					...
					qcom,tchg-mins = <0x0000001e>;
					...
					}

## extract_dtb
	
	https://github.com/PabloCastellano/extract-dtb
	
这个可以直接从vmlinux或者bootimage里解析出dtb文件

## dt_to_config

scripts/dtc/dt_to_config


可以根据传入的设备树解析出每个节点对应那些驱动以及内核是否已经配置，甚至可以通过传递--config-format来直接修改.config文件，如果嫌节点的路径过长，可以传递参数--short-name，用法示例：

	# 这里的--include-suspec用于解析设备树中的include字段
	$./scripts/dtc/dt_to_config arch/arm/boot/dts/s3c2440-tq2440-dt.dts --include-suspect --config ./.config
	-dDc-E-Hx---- : /i2c-gpio-1 : i2c-gpio : arch/arm/mach-ep93xx/core.c : obj-y : x
	-dDc-E-H-n--F : /i2c-gpio-1 : i2c-gpio : arch/arm/mach-ixp4xx/avila-setup.c : CONFIG_MACH_AVILA : n
	-dDc-E-H-n--F : /i2c-gpio-1 : i2c-gpio : arch/arm/mach-ixp4xx/dsmg600-setup.c : CONFIG_MACH_DSMG600 : n
	...

也可以直接传dtb
	
	$./scripts/dtc/dt_to_config arch/arm/boot/dts/s3c2440-tq2440-dt.dtb --include-suspect --config ./.config
	-dDc-E-Hx---- : /i2c-gpio-1 : i2c-gpio : arch/arm/mach-ep93xx/core.c : obj-y : x
	-dDc-E-H-n--F : /i2c-gpio-1 : i2c-gpio : arch/arm/mach-ixp4xx/avila-setup.c : CONFIG_MACH_AVILA : n
	-dDc-E-H-n--F : /i2c-gpio-1 : i2c-gpio : arch/arm/mach-ixp4xx/dsmg600-setup.c : CONFIG_MACH_DSMG600 : n
	-dDc-E-H-n--F : /i2c-gpio-1 : i2c-gpio : arch/arm/mach-ixp4xx/fsg-setup.c : CONFIG_MACH_FSG : n
	...
	
# dtx_diff

./scripts/dtc/dtx_diff

用于比较两个设备树，也可以对设备树反编译

	#传递一个设备树，表示反编译
	./scripts/dtc/dtx_diff arch/arm/boot/dts/s3c2440-tq2440-dt.dtb

	#也可以直接传递一个dts文件，这个工具会将该dts文件展开
	./scripts/dtc/dtx_diff arch/arm/boot/dts/s3c2440-tq2440-dt.dts

	#传递两个设备树表示比较
	./scripts/dtc/dtx_diff arch/arm/boot/dts/s3c2440-tq2440-dt.dtb /tftpboot/dtb 
	--- /dev/fd/63    2017-09-23 22:01:48.407691984 +0800
	+++ /dev/fd/62    2017-09-23 22:01:48.407691984 +0800
	@@ -125,7 +125,7 @@
		 pinctrl-names = "default";
		 reg = <0x54000000 0x100>;
		 samsung,i2c-max-bus-freq = <0x30d40>;
	-        status = "disabled";
	+        status = "okay";
	 
		 demo0@51 {
		     compatible = "demo0";

