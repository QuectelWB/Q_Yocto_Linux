#屏幕信息的cmdline追踪


## 原始打印

lk里的打印
	
	[1120] display kernel cmdline: mdss_mdp3.panel=1:dsi:0:qcom,mdss_dsi_hx8394f_720p_video:1:none:cfg:single_dsi
	[1130] cmdline: noinitrd ro console=ttyHSL0,115200,n8 androidboot.hardware=qcom ehci-hcd.park=3 msm_rtb.filter=0x37 lpm_levels.sleep_disabled=1 rootwait androidboot.bootdevice=7824900.sdhci androidboot.bootdevice=7824900.sdhci androidboot.serialno=faa8079d andro
	[1160] Updating device tree: start

kernel的打印	
		
	[    0.965226] mdss_dsi_find_panel_of_node: cmdline:0:qcom,mdss_dsi_hx8394f_720p_video:1:none:cfg:single_dsi panel_name:qcom,mdss_dsi_hx8394f_720p_video
	
先找到这两个打印的位置。
	
	$ grep -rnH "display kernel cmdline"
	dev/gcdb/display/gcdb_display_param.c:525:              dprintf(INFO, "display kernel cmdline:%s\n"

打开改文件对应的函数 lk 中

	bool gcdb_display_cmdline_arg:
		display_cmd_line

在kernel中
	
	$ grep -rnH "mdss_dsi_find_panel_of_node"
	
	drivers/video/msm/mdss/mdss_dsi.c:2814: * mdss_dsi_find_panel_of_node(): find device node of dsi panel
	drivers/video/msm/mdss/mdss_dsi.c:2827:static struct device_node *mdss_dsi_find_panel_of_node(
	drivers/video/msm/mdss/mdss_dsi.c:2960: dsi_pan_node = mdss_dsi_find_panel_of_node(pdev, panel_cfg);
	
	mdss_dsi_find_panel_of_node             
		pr_info("%s: cmdline:%s panel_name:%s\n",__func__, panel_cfg, panel_name);

## lk 里的display_cmd_line

			cmd_boot/boot_linux_from_mmc/boot_linux_from_flash
				boot_linux						app/aboot/aboot.c
					update_cmdline					app/aboot/aboot.c
						target_display_panel_node		target/msm8909/target_display.c
							gcdb_display_cmdline_arg	dev/gcdb/display/gcdb_display_param.c
								display_cmd_line
		
关键的几个地方：

	update_cmdline:
		target_display_panel_node (display_panel_buf, MAX_PANEL_BUF_SIZE) 
	

	char display_panel_buf[MAX_PANEL_BUF_SIZE] 文件内的全局变量

	gcdb_display_cmdline_arg 和 target_display_panel_node 是一回事。
	
	 gcdb_display_cmdline_arg
		mdss_dsi_get_panel_data 获取panel的数据结构struct panel_struct panelstruct

	dev/gcdb/display/gcdb_display.c 中

		struct panel_struct panelstruct 也是一个全局变量

## lk 调用过程

	target_display_init()
	    |
	    gcdb_display_init()
		|
		oem_panel_select()  /* 根据panel_id选择屏幕 */
		|
		init_platform_data()  /* 初始化dsi接口 */
		|
		dsi_panel_init()
		|
		msm_display_init()
		    |
		    power_func()  /* panel电源开启 */
		    |
		    clk_func()    /* dis时钟开启 */
		    |
		    display_image_on_screen()  /* 载入logo并显示 */
		    |
		    msm_display_on()
		    |
		    bl_func()        /* 开启panel的背光灯 */	
	


