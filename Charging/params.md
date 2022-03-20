
## 按下 powerkey 的开机时间

	https://blog.csdn.net/u010689853/article/details/112283914
	看起来要修改rpm和xbl，目前release 的代码看不到。
	
	boot_images/…/pm_config_target.c
	
	/*key press in milli seconds for valid boot*/
	pm_pwrkey_dbnc_chk_type pm_pon_pwrkey_dbnc_chk[] = {{PM_PON_PWRKEY_DBNC_CHK_AT_CORE, 3000}} ; //check power key press at, time in milli sec
	
	
