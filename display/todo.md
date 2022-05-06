# todo

- 如果电源供电改为GPIO控制，而不是LDO输出。如何修改驱动

- /dev/fb0 能不能直接写

- adb shell wm size

- 


## res

现成的 ADB 命令为：
wm size 1920x1280 ----> 这个 ADB 修改分辨率
wm density 240 ----> 这个 修改dpi

dpi 计算公式： ( √(x^2 + y^2) ) / 屏幕尺寸

其实 wm 调用的就 是 上层 WindowsManger.java 的方法，最终调用到 Framework 下的 WindowsManagerServices.java 代码。
如果上层想对 分辨率修改，可以重写 WindowsManagerServices.java 下的set 分辨率的方法。
这种方法也是最稳妥的办法。

wm 执行log 如下：

px30_evb:/ # wm size 640x480 

logcat 如下：

	10-12 08:05:46.995  1727  1727 D AndroidRuntime: Calling main entry com.android.commands.wm.Wm
	10-12 08:05:47.001   396   408 I WindowManager: Using new display size: 640x480



如果在驱动中写死的话，一般不建议这样修改，容易出问题：

高通平台：
	
	--- a/kernel/msm-3.18/drivers/video/msm/mdss/mdss_fb.c
	+++ b/kernel/msm-3.18/drivers/video/msm/mdss/mdss_fb.c
	@@ -2501,8 +2501,8 @@ static int mdss_fb_register(struct msm_fb_data_type *mfd)
		else
			fix->line_length = var->xres * bpp;
	 
	-       var->xres_virtual = var->xres;
	-       var->yres_virtual = panel_info->yres * mfd->fb_page;
	+       var->xres_virtual = 1920;	//var->xres;
	+       var->yres_virtual = 1080 * mfd->fb_page;
		var->bits_per_pixel = bpp * 8;  /* FrameBuffer color depth */
	 
		/*
	@@ -3410,8 +3410,8 @@ void mdss_panelinfo_to_fb_var(struct mdss_panel_info *pinfo,
	 {
		u32 frame_rate;
	 
	-       var->xres = mdss_fb_get_panel_xres(pinfo);
	-       var->yres = pinfo->yres;
	+       var->xres = 1920;		//mdss_fb_get_panel_xres(pinfo);
	+       var->yres = 1080;		//pinfo->yres;
		var->lower_margin = pinfo->lcdc.v_front_porch -
			pinfo->prg_fet;
		var->upper_margin = pinfo->lcdc.v_back_porch +

原文链接：https://blog.csdn.net/Ciellee/article/details/102583694


## 


