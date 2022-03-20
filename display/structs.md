fb_var_screeninfo

	struct fb_var_screeninfo {
		__u32 xres;			/* visible resolution		*/
		__u32 yres;
		__u32 xres_virtual;		/* virtual resolution		*/
		__u32 yres_virtual;
		__u32 xoffset;			/* offset from virtual to visible */
		__u32 yoffset;			/* resolution			*/

		__u32 bits_per_pixel;		/* guess what			*/
		__u32 grayscale;		/* 0 = color, 1 = grayscale,	*/
						/* >1 = FOURCC			*/
		struct fb_bitfield red;		/* bitfield in fb mem if true color, */
		struct fb_bitfield green;	/* else only length is significant */
		struct fb_bitfield blue;
		struct fb_bitfield transp;	/* transparency			*/	

		__u32 nonstd;			/* != 0 Non standard pixel format */

		__u32 activate;			/* see FB_ACTIVATE_*		*/

		__u32 height;			/* height of picture in mm    */
		__u32 width;			/* width of picture in mm     */

		__u32 accel_flags;		/* (OBSOLETE) see fb_info.flags */

		/* Timing: All values in pixclocks, except pixclock (of course) */
		__u32 pixclock;			/* pixel clock in ps (pico seconds) */
		__u32 left_margin;		/* time from sync to picture	*/
		__u32 right_margin;		/* time from picture to sync	*/
		__u32 upper_margin;		/* time from sync to picture	*/
		__u32 lower_margin;
		__u32 hsync_len;		/* length of horizontal sync	*/
		__u32 vsync_len;		/* length of vertical sync	*/
		__u32 sync;			/* see FB_SYNC_*		*/
		__u32 vmode;			/* see FB_VMODE_*		*/
		__u32 rotate;			/* angle we rotate counter clockwise */
		__u32 colorspace;		/* colorspace for FOURCC-based modes */
		__u32 reserved[4];		/* Reserved for future compatibility */
	};

		
mdss_panel.h
	
	struct mdss_panel_info {
		u32 xres;
		u32 yres;
		u32 physical_width;
		u32 physical_height;
		u32 bpp;
		u32 type;
		u32 wait_cycle;
		u32 pdest;
		u32 brightness_max;
		u32 bl_max;
		u32 bl_min;
		u32 fb_num;
		u64 clk_rate;
		u32 clk_min;
		u64 clk_max;
		u32 mdp_transfer_time_us;
		u32 frame_count;
		u32 is_3d_panel;
		u32 out_format;
		u32 rst_seq[MDSS_DSI_RST_SEQ_LEN];
		u32 rst_seq_len;
		u32 vic; /* video identification code */
		u32 deep_color;
		struct mdss_rect roi;
		struct mdss_dsi_dual_pu_roi dual_roi;
		int pwm_pmic_gpio;
		int pwm_lpg_chan;
		int pwm_period;
		bool dynamic_fps;
		bool dynamic_bitclk;
		u32 *supp_bitclks;
		u32 supp_bitclk_len;
		bool ulps_feature_enabled;
		bool ulps_suspend_enabled;
		bool panel_ack_disabled;
		bool esd_check_enabled;
		bool allow_phy_power_off;
		char dfps_update;
		/* new requested bitclk before it is updated in hw */
		int new_clk_rate;
		/* new requested fps before it is updated in hw */
		int new_fps;
		/* stores initial fps after boot */
		u32 default_fps;
		/* stores initial vtotal (vfp-method) or htotal (hfp-method) */
		u32 saved_total;
		/* stores initial vfp (vfp-method) or hfp (hfp-method) */
		u32 saved_fporch;
		/* current fps, once is programmed in hw */
		int current_fps;
		u32 mdp_koff_thshold_low;
		u32 mdp_koff_thshold_high;
		bool mdp_koff_thshold;
		u32 mdp_koff_delay;

		int panel_max_fps;
		int panel_max_vtotal;
		u32 mode_sel_state;
		u32 min_fps;
		u32 max_fps;
		u32 prg_fet;
		struct mdss_panel_roi_alignment roi_alignment;

		u32 cont_splash_enabled;
		bool esd_rdy;
		u32 partial_update_supported; /* value from dts if pu is supported */
		u32 partial_update_enabled; /* is pu currently allowed */
		u32 dcs_cmd_by_left;
		u32 partial_update_roi_merge;
		struct ion_handle *splash_ihdl;
		int panel_power_state;
		int compression_mode;

		split_link_enableduint32_t panel_dead;
		u32 panel_force_dead;
		u32 panel_orientation;
		bool dynamic_switch_pending;
		bool is_lpm_mode;
		bool is_split_display; /* two DSIs in one display, pp split or not */
		bool use_pingpong_split;
		bool split_link_enabled;

		/*
		 * index[0] = left layer mixer, value of 0 not valid
		 * index[1] = right layer mixer, 0 is possible
		 *
		 * Ex(1): 1080x1920 display using single DSI and single lm, [1080 0]
		 * Ex(2): 1440x2560 display using two DSIs and two lms,
		 *        each with 720x2560, [720 0]
		 * Ex(3): 1440x2560 display using single DSI w/ compression and
		 *        single lm, [1440 0]
		 * Ex(4): 1440x2560 display using single DSI w/ compression and
		 *        two lms, [720 720]
		 * Ex(5): 1080x1920 display using single DSI and two lm, [540 540]
		 * Ex(6): 1080x1920 display using single DSI and two lm,
		 *        [880 400] - not practical but possible
		 */

		u32 lm_widths[2];

		bool is_prim_panel;
		bool is_pluggable;
		char display_id[MDSS_DISPLAY_ID_MAX_LEN];
		bool is_cec_supported;

		/* refer sim_panel_modes enum for different modes */
		u8 sim_panel_mode;

		void *edid_data;
		void *dba_data;
		void *cec_data;
		void *hdcp_1x_data;

		char panel_name[MDSS_MAX_PANEL_LEN];
		struct mdss_mdp_pp_tear_check te;
		struct mdss_mdp_pp_tear_check te_cached;

		/*
		 * Value of 2 only when single DSI is configured with 2 DSC
		 * encoders. When 2 encoders are used, currently both use
		 * same configuration.
		 */
		u8 dsc_enc_total; /* max 2 */
		struct dsc_desc dsc;

		/*
		 * To determine, if DSC panel requires the pps to be sent
		 * before or after the switch, during dynamic resolution switching
		 */
		bool send_pps_before_switch;

		struct lcd_panel_info lcdc;
		struct fbc_panel_info fbc;
		struct mipi_panel_info mipi;
		struct lvds_panel_info lvds;
		struct edp_panel_info edp;

		bool is_dba_panel;

		/*
		 * Delay(in ms) to accommodate s/w delay while
		 * configuring the event timer wakeup logic.
		 */
		u32 adjust_timer_delay_ms;

		/* debugfs structure for the panel */
		struct mdss_panel_debugfs_info *debugfs_info;

		/* persistence mode on/off */
		bool persist_mode;

		/* stores initial adaptive variable refresh vtotal value */
		u32 saved_avr_vtotal;

		/* HDR properties of display panel*/
		struct mdss_panel_hdr_properties hdr_properties;

		/* esc clk recommended for the panel */
		u32 esc_clk_rate_hz;
	};

