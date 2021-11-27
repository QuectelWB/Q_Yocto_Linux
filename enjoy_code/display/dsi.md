# LCM 的code


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
	

## 数据结构

	struct msm_fb_panel_data {
	    struct msm_panel_info panel_info; /* 描述panel的数据结构 */
	    struct fbcon_config fb;
	    int mdp_rev;
	    int rotate;

	    /* function entry chain */    /* 与msm底层寄存器操作相关函数API */
	    int (*power_func) (int enable, struct msm_panel_info *);
	    int (*clk_func) (int enable);
	    int (*bl_func) (int enable);
	    int (*pll_clk_func) (int enable, struct msm_panel_info *);
	    int (*post_power_func)(int enable);
	    int (*pre_init_func)(void);
	};



	struct panel_struct {
		struct panel_config         *paneldata;
		struct panel_resolution     *panelres;
		struct color_info           *color;
		struct videopanel_info      *videopanel;
		struct commandpanel_info    *commandpanel;
		struct command_state        *state;
		struct lane_configuration   *laneconfig;
		struct panel_timing         *paneltiminginfo;
		struct panel_reset_sequence *panelresetseq;
		struct backlight            *backlightinfo;
		struct fb_compression       fbcinfo;
		struct topology_config      *config;
	};

## 
