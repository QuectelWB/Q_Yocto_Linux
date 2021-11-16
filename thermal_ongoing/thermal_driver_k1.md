#thermal driver

## msm_thermal的probe函数
我们先从msm_thermal.c的驱动初始化分析，device在dts里面。

	static struct of_device_id msm_thermal_match_table[] = {
		{.compatible = "qcom,msm-thermal"},
		{},
	};

	static struct platform_driver msm_thermal_device_driver = {
		.probe = msm_thermal_dev_probe,
		.driver = {
			.name = "msm-thermal",
			.owner = THIS_MODULE,
			.of_match_table = msm_thermal_match_table,
		},
		.remove = msm_thermal_dev_exit,
	};

	int __init msm_thermal_device_init(void)
	{
		return platform_driver_register(&msm_thermal_device_driver);
	}
	arch_initcall(msm_thermal_device_init);

dts关于msm_thermal如下：主要就是一些参数比如限频温度、拔核温度等。

	qcom,msm-thermal {
					compatible = "qcom,msm-thermal";
					qcom,sensor-id = <3>;
					qcom,poll-ms = <250>;
					qcom,limit-temp = <60>;
					qcom,temp-hysteresis = <10>;
					qcom,freq-step = <2>;
					qcom,core-limit-temp = <80>;
					qcom,core-temp-hysteresis = <10>;
					qcom,hotplug-temp = <97>;
					qcom,hotplug-temp-hysteresis = <12>;
					qcom,freq-mitigation-temp = <97>;
					qcom,freq-mitigation-temp-hysteresis = <12>;
					qcom,freq-mitigation-value = <400000>;
					qcom,online-hotplug-core;
					qcom,therm-reset-temp = <115>;
					qcom,disable-cx-phase-ctrl;
					qcom,disable-gfx-phase-ctrl;
					qcom,disable-vdd-mx;
					qcom,disable-psm;
					qcom,disable-ocr;
					qcom,vdd-restriction-temp = <5>;
					qcom,vdd-restriction-temp-hysteresis = <10>;
					vdd-dig-supply = <&pm8909_s1_floor_corner>;
	
					qcom,vdd-dig-rstr{
							qcom,vdd-rstr-reg = "vdd-dig";
							qcom,levels = <5 7 7>; /* Nominal, Super Turbo, Super Turbo */
							qcom,min-level = <1>; /* No Request */
					};
			};
	
 
内核找到msm_thermal device后执行probe函数，我们来分析下msm_thermal_dev_probe函数，前面主要是获取dts里面各个参数，放入msm_thermal_data数据里面。

	static int msm_thermal_dev_probe(struct platform_device *pdev)
	{
		int ret = 0;
		char *key = NULL;
		struct device_node *node = pdev->dev.of_node;
		struct msm_thermal_data data;

		if (!mitigation)
			return ret;

		memset(&data, 0, sizeof(struct msm_thermal_data));
		data.pdev = pdev;

		ret = msm_thermal_pre_init(&pdev->dev);
		if (ret) {
			pr_err("thermal pre init failed. err:%d\n", ret);
			goto fail;
		}

		key = "qcom,sensor-id";
		ret = of_property_read_u32(node, key, &data.sensor_id);
		if (ret)
			goto fail;

		key = "qcom,poll-ms";
		ret = of_property_read_u32(node, key, &data.poll_ms);
		if (ret)
			goto fail;

		key = "qcom,limit-temp";
		ret = of_property_read_u32(node, key, &data.limit_temp_degC);
		if (ret)
			goto fail;

		key = "qcom,temp-hysteresis";
		ret = of_property_read_u32(node, key, &data.temp_hysteresis_degC);
		if (ret)
			goto fail;

		key = "qcom,freq-step";
		ret = of_property_read_u32(node, key, &data.bootup_freq_step);
		if (ret)
			goto fail;

		key = "qcom,online-hotplug-core";
		if (of_property_read_bool(node, key))
			online_core = true;
		else
			online_core = false;

		probe_sensor_info(node, &data, pdev);
		ret = probe_cc(node, &data, pdev);

		ret = probe_freq_mitigation(node, &data, pdev);
		ret = probe_cx_phase_ctrl(node, &data, pdev);
		ret = probe_gfx_phase_ctrl(node, &data, pdev);
		ret = probe_therm_reset(node, &data, pdev);

		ret = probe_vdd_mx(node, &data, pdev);
		if (ret == -EPROBE_DEFER)
			goto fail;
		/*
		 * Probe optional properties below. Call probe_psm before
		 * probe_vdd_rstr because rpm_regulator_get has to be called
		 * before devm_regulator_get
		 * probe_ocr should be called after probe_vdd_rstr to reuse the
		 * regualtor handle. calling devm_regulator_get more than once
		 * will fail.
		 */
		ret = probe_psm(node, &data, pdev);
		if (ret == -EPROBE_DEFER)
			goto fail;

		update_cpu_topology(&pdev->dev);
		ret = probe_vdd_rstr(node, &data, pdev);
		if (ret == -EPROBE_DEFER)
			goto fail;
		ret = probe_ocr(node, &data, pdev);

		ret = fetch_cpu_mitigaiton_info(&data, pdev);
		if (ret) {
			pr_err("Error fetching CPU mitigation information. err:%d\n",
					ret);
			goto probe_exit;
		}

		/*
		 * In case sysfs add nodes get called before probe function.
		 * Need to make sure sysfs node is created again
		 */
		if (psm_nodes_called) {
			msm_thermal_add_psm_nodes();
			psm_nodes_called = false;
		}
		if (vdd_rstr_nodes_called) {
			msm_thermal_add_vdd_rstr_nodes();
			vdd_rstr_nodes_called = false;
		}
		if (sensor_info_nodes_called) {
			msm_thermal_add_sensor_info_nodes();
			sensor_info_nodes_called = false;
		}
		if (ocr_nodes_called) {
			msm_thermal_add_ocr_nodes();
			ocr_nodes_called = false;
		}
		if (cluster_info_nodes_called) {
			create_cpu_topology_sysfs();
			cluster_info_nodes_called = false;
		}
		msm_thermal_ioctl_init();
		ret = msm_thermal_init(&data);
		msm_thermal_probed = true;

		return ret;
	fail:
		if (ret)
			pr_err("Failed reading node=%s, key=%s. err:%d\n",
				node->full_name, key, ret);
	probe_exit:
		return ret;
	}

## 读取sensor信息
我们先看probe_sensor_info函数，就是读取dts中的qcom,sensor-information中各个sensor的信息，然后放入到sensors这个数组中。

	static void probe_sensor_info(struct device_node *node,
			struct msm_thermal_data *data, struct platform_device *pdev)
	{
		int err = 0;
		int i = 0;
		char *key = NULL;
		struct device_node *child_node = NULL;
		struct device_node *np = NULL;
		int scale_tsens_found = 0;

		key = "qcom,disable-sensor-info";
		if (of_property_read_bool(node, key)) {
			sensor_info_probed = true;
			return;
		}

		np = of_find_compatible_node(NULL, NULL, "qcom,sensor-information");
		if (!np) {
			dev_info(&pdev->dev,
			"%s:unable to find DT for sensor-information.KTM continues\n",
			__func__);
			sensor_info_probed = true;
			return;
		}
		sensor_cnt = of_get_child_count(np);
		if (sensor_cnt == 0) {
			err = -ENODEV;
			goto read_node_fail;
		}

		sensors = devm_kzalloc(&pdev->dev,//创建sensors放入各个sensor的信息
				sizeof(struct msm_sensor_info) * sensor_cnt,
				GFP_KERNEL);
		if (!sensors) {
			pr_err("Fail to allocate memory for sensor_info.\n");
			err = -ENOMEM;
			goto read_node_fail;
		}

		for_each_child_of_node(np, child_node) {
			const char *alias_name = NULL;

			key = "qcom,sensor-type";
			err = of_property_read_string(child_node,
					key, &sensors[i].type);
			if (err)
				goto read_node_fail;

			key = "qcom,sensor-name";
			err = of_property_read_string(child_node,
					key, &sensors[i].name);
			if (err)
				goto read_node_fail;

			key = "qcom,alias-name";
			of_property_read_string(child_node, key, &alias_name);
			if (alias_name && !strnstr(alias_name, "cpu",
				strlen(alias_name)))
				sensors[i].alias = alias_name;

			key = "qcom,scaling-factor";
			err = of_property_read_u32(child_node, key,
					&sensors[i].scaling_factor);
			if (err || sensors[i].scaling_factor == 0) {
				sensors[i].scaling_factor = SENSOR_SCALING_FACTOR;
				err = 0;
			}
			if (scale_tsens_found == 0) {
				if (!strcmp(sensors[i].type, "tsens")) {
					scale_tsens_found = 1;
					tsens_scaling_factor =
						sensors[i].scaling_factor;
				}
			}
			i++;
		}

	read_node_fail:
		sensor_info_probed = true;
		if (err) {
			dev_info(&pdev->dev,
			"%s:Failed reading node=%s, key=%s. err=%d. KTM continues\n",
				__func__, np->full_name, key, err);
			devm_kfree(&pdev->dev, sensors);
		}
	}


关于sensor的info如下：

			qcom,sensor-information {
					compatible = "qcom,sensor-information";
					sensor_information0: qcom,sensor-information-0 {
							qcom,sensor-type = "tsens";
							qcom,sensor-name = "tsens_tz_sensor0";
							qcom,alias-name = "pop_mem";
					};
	
					sensor_information1: qcom,sensor-information-1 {
							qcom,sensor-type =  "tsens";
							qcom,sensor-name = "tsens_tz_sensor1";
					};
	
					sensor_information2: qcom,sensor-information-2 {
							qcom,sensor-type =  "tsens";
							qcom,sensor-name = "tsens_tz_sensor2";
					};
	
					sensor_information3: qcom,sensor-information-3 {
							qcom,sensor-type =  "tsens";
							qcom,sensor-name = "tsens_tz_sensor3";
					};
	
					sensor_information4: qcom,sensor-information-4 {
							qcom,sensor-type = "tsens";
							qcom,sensor-name = "tsens_tz_sensor4";
					};
	
					sensor_information5: qcom,sensor-information-5 {
							qcom,sensor-type = "adc";
							qcom,sensor-name = "pa_therm0";
					};
	
					sensor_information6: qcom,sensor-information-6 {
							qcom,sensor-type = "adc";
							qcom,sensor-name = "case_therm";
					};
	
					sensor_information7: qcom,sensor-information-7 {
							qcom,sensor-type = "alarm";
							qcom,sensor-name = "pm8909_tz";
							qcom,scaling-factor = <1000>;
					};
	
					sensor_information8: qcom,sensor-information-8 {
							qcom,sensor-type = "adc";
							qcom,sensor-name = "xo_therm";
					};
	
					sensor_information9: qcom,sensor-information-9 {
							qcom,sensor-type = "adc";
							qcom,sensor-name = "xo_therm_buf";
					};
			};
 
 
我们再来看下probe_cc函数，主要是获取一些控制的参数。

	static int probe_cc(struct device_node *node, struct msm_thermal_data *data,
			struct platform_device *pdev)
	{
		char *key = NULL;
		int ret = 0;

		if (num_possible_cpus() > 1) {
			core_control_enabled = 1;
			hotplug_enabled = 1;
		}

		key = "qcom,core-limit-temp";
		ret = of_property_read_u32(node, key, &data->core_limit_temp_degC);
		if (ret)
			goto read_node_fail;

		key = "qcom,core-temp-hysteresis";
		ret = of_property_read_u32(node, key, &data->core_temp_hysteresis_degC);
		if (ret)
			goto read_node_fail;

		key = "qcom,hotplug-temp";
		ret = of_property_read_u32(node, key, &data->hotplug_temp_degC);
		if (ret)
			goto hotplug_node_fail;

		key = "qcom,hotplug-temp-hysteresis";
		ret = of_property_read_u32(node, key,
				&data->hotplug_temp_hysteresis_degC);
		if (ret)
			goto hotplug_node_fail;

	read_node_fail:
		if (ret) {
			dev_info(&pdev->dev,
			"%s:Failed reading node=%s, key=%s. err=%d. KTM continues\n",
				KBUILD_MODNAME, node->full_name, key, ret);
			core_control_enabled = 0;
		} else {
			snprintf(mit_config[MSM_LIST_MAX_NR + HOTPLUG_CONFIG]
				.config_name, MAX_DEBUGFS_CONFIG_LEN,
				"hotplug");
			mit_config[MSM_LIST_MAX_NR + HOTPLUG_CONFIG].disable_config
				= thermal_cpu_hotplug_mit_disable;
		}

		return ret;

	hotplug_node_fail:
		if (ret) {
			dev_info(&pdev->dev,
			"%s:Failed reading node=%s, key=%s. err=%d. KTM continues\n",
				KBUILD_MODNAME, node->full_name, key, ret);
			hotplug_enabled = 0;
		}

		return ret;
	
## 创建sensor_info的ss节点
我们再来看下msm_thermal_add_sensor_info_nodes函数，这个函数主要就是创建sensor_info这个sys的节点。

	static struct kobj_attribute sensor_info_attr =
			__ATTR_RO(sensor_info);
	static int msm_thermal_add_sensor_info_nodes(void)
	{
		struct kobject *module_kobj = NULL;
		int ret = 0;

		if (!sensor_info_probed) {
			sensor_info_nodes_called = true;
			return ret;
		}
		if (sensor_info_probed && sensor_cnt == 0)
			return ret;

		module_kobj = kset_find_obj(module_kset, KBUILD_MODNAME);
		if (!module_kobj) {
			pr_err("cannot find kobject\n");
			return -ENOENT;
		}
		sysfs_attr_init(&sensor_info_attr.attr);
		ret = sysfs_create_file(module_kobj, &sensor_info_attr.attr);//创建sys节点文件
		if (ret) {
			pr_err(
			"cannot create sensor info kobject attribute. err:%d\n",
			ret);
			return ret;
		}

		return ret;
	}

我们来看下__ATTR_RO这个宏，结合上面代码，就是节点的名字是senor_info，show函数是sensor_info_show

	#define __ATTR_RO(_name) {						\
		.attr	= { .name = __stringify(_name), .mode = S_IRUGO },	\
		.show	= _name##_show,						\
	}

我们再来看下show函数：很简单就是从sensors这个数组中读取数据。

	static ssize_t sensor_info_show(
		struct kobject *kobj, struct kobj_attribute *attr, char *buf)
	{
		int i;
		ssize_t tot_size = 0, size = 0;

		for (i = 0; i < sensor_cnt; i++) {
			size = snprintf(&buf[tot_size], PAGE_SIZE - tot_size,
				"%s:%s:%s:%d ",
				sensors[i].type, sensors[i].name,
				sensors[i].alias ? : "",
				sensors[i].scaling_factor);
			if (tot_size + size >= PAGE_SIZE) {
				pr_err("Not enough buffer size\n");
				break;
			}
			tot_size += size;
		}
		if (tot_size)
			buf[tot_size - 1] = '\n';

		return tot_size;
	}


我们从手机上看下这个节点，而这个节点也是daemon程序在创建sensor的时候从这个节点中读取信息。

	msm8909go_benz:/sys/module/msm_thermal # cat sensor_info
	cat sensor_info
	tsens:tsens_tz_sensor0:pop_mem:1 tsens:tsens_tz_sensor1::1 tsens:tsens_tz_sensor2::1 tsens:tsens_tz_sensor3:cpu0-cpu2:1 tsens:tsens_tz_sensor4:cpu1-cpu3:1 adc:pa_therm0::1 adc:case_therm::1 alarm:pm8909_tz::1000 adc:xo_therm::1 adc:xo_therm_buf::1

## msm_thermal_ioctl（给用户进程cpu调频）
probe函数后面调用了msm_thermal_ioctl_init函数，我们再来看下msm_thermal_ioctl_init函数。这个函数主要就是创建了字符设备dev/msm_thermal_query然后daemon可以通过ioctl的方式来限制cpu频率等。

	int msm_thermal_ioctl_init()
	{
		int ret = 0;
		dev_t thermal_dev;
		struct device *therm_device;

		ret = alloc_chrdev_region(&thermal_dev, 0, 1,
			MSM_THERMAL_IOCTL_NAME);
		if (ret < 0) {
			pr_err("%s: Error in allocating char device region. Err:%d\n",
				KBUILD_MODNAME, ret);
			goto ioctl_init_exit;
		}

		msm_thermal_major = MAJOR(thermal_dev);

		thermal_class = class_create(THIS_MODULE, "msm_thermal");
		if (IS_ERR(thermal_class)) {
			pr_err("%s: Error in creating class\n",
				KBUILD_MODNAME);
			ret = PTR_ERR(thermal_class);
			goto ioctl_class_fail;
		}

		therm_device = device_create(thermal_class, NULL, thermal_dev, NULL,
					MSM_THERMAL_IOCTL_NAME);
		if (IS_ERR(therm_device)) {
			pr_err("%s: Error in creating character device\n",
				KBUILD_MODNAME);
			ret = PTR_ERR(therm_device);
			goto ioctl_dev_fail;
		}
		msm_thermal_dev = kmalloc(sizeof(struct msm_thermal_ioctl_dev),
					GFP_KERNEL);
		if (!msm_thermal_dev) {
			pr_err("%s: Error allocating memory\n",
				KBUILD_MODNAME);
			ret = -ENOMEM;
			goto ioctl_clean_all;
		}

		memset(msm_thermal_dev, 0, sizeof(struct msm_thermal_ioctl_dev));
		sema_init(&msm_thermal_dev->sem, 1);
		cdev_init(&msm_thermal_dev->char_dev, &msm_thermal_fops);
		ret = cdev_add(&msm_thermal_dev->char_dev, thermal_dev, 1);
		if (ret < 0) {
			pr_err("%s: Error in adding character device\n",
				KBUILD_MODNAME);
			goto ioctl_clean_all;
		}

		return ret;

	ioctl_clean_all:
		device_destroy(thermal_class, thermal_dev);
	ioctl_dev_fail:
		class_destroy(thermal_class);
	ioctl_class_fail:
		unregister_chrdev_region(thermal_dev, 1);
	ioctl_init_exit:
		return ret;
	}


我们再来看下其ioctl函数，最后是通过msm_thermal_set_frequency来设置频率。

	static long msm_thermal_ioctl_process(struct file *filep, unsigned int cmd,
		unsigned long arg)
	{
		long ret = 0;
		struct msm_thermal_ioctl query;

		pr_debug("%s: IOCTL: processing cmd:%u\n", KBUILD_MODNAME, cmd);

		ret = validate_and_copy(&cmd, &arg, &query);
		if (ret)
			return ret;

		mutex_lock(&ioctl_access_mutex);
		switch (cmd) {
		case MSM_THERMAL_SET_CPU_MAX_FREQUENCY:
			ret = msm_thermal_set_frequency(query.cpu_freq.cpu_num,
				query.cpu_freq.freq_req, true);
			break;
		case MSM_THERMAL_SET_CPU_MIN_FREQUENCY:
			ret = msm_thermal_set_frequency(query.cpu_freq.cpu_num,
				query.cpu_freq.freq_req, false);
			break;
		case MSM_THERMAL_SET_CLUSTER_MAX_FREQUENCY:
			ret = msm_thermal_set_cluster_freq(query.cpu_freq.cpu_num,
				query.cpu_freq.freq_req, true);
			break;
		case MSM_THERMAL_SET_CLUSTER_MIN_FREQUENCY:
			ret = msm_thermal_set_cluster_freq(query.cpu_freq.cpu_num,
				query.cpu_freq.freq_req, false);
			break;
		case MSM_THERMAL_GET_CLUSTER_FREQUENCY_PLAN:
			ret = msm_thermal_process_freq_table_req(&query, &arg);
			break;
		case MSM_THERMAL_GET_CLUSTER_VOLTAGE_PLAN:
			ret = msm_thermal_process_voltage_table_req(&query, &arg);
			break;
		default:
			ret = -ENOTTY;
			goto process_exit;
		}
	process_exit:
		mutex_unlock(&ioctl_access_mutex);
		return ret;
	}


我们再来看下msm_thermal_set_frequency函数，最后设置的cpu和cpu频率也是到msm_thermal定义的一个cpus的数组中。

然后是如何更新到实际cpu中，我们继续分析。我们注意到这里有一个complete函数，这里就是类似condition。因此我们去找其他的。

	int msm_thermal_set_frequency(uint32_t cpu, uint32_t freq, bool is_max)
	{
		int ret = 0;

		if (!mitigation) {
			pr_err("Thermal Mitigations disabled.\n");
			goto set_freq_exit;
		}

		if (cpu >= num_possible_cpus()) {
			pr_err("Invalid input\n");
			ret = -EINVAL;
			goto set_freq_exit;
		}

		pr_debug("Userspace requested %s frequency %u for CPU%u\n",
				(is_max) ? "Max" : "Min", freq, cpu);
		if (is_max) {
			if (cpus[cpu].user_max_freq == freq)
				goto set_freq_exit;

			cpus[cpu].user_max_freq = freq;
		} else {
			if (cpus[cpu].user_min_freq == freq)
				goto set_freq_exit;

			cpus[cpu].user_min_freq = freq;
		}

		if (freq_mitigation_task) {
			complete(&freq_mitigation_complete);
		} else {
			pr_err("Frequency mitigation task is not initialized\n");
			ret = -ESRCH;
			goto set_freq_exit;
		}

	set_freq_exit:
		return ret;
	}

果然在freq_mitigation_init函数中，这个函数先是将从dts中获取的阈值放入cpus数组的threshold的high和low中。然后设置对sensor的阈值，最后启动一个Thread来执行do_freq_mitigation函数，而这个函数就会去更新cpu的最大最小频率了。

	static void freq_mitigation_init(void)
	{
		uint32_t cpu = 0;
		struct sensor_threshold *hi_thresh = NULL, *low_thresh = NULL;

		if (freq_mitigation_task)
			return;
		if (!freq_mitigation_enabled)
			goto init_freq_thread;

		for_each_possible_cpu(cpu) {
			/*
			 * Hotplug may not be enabled,
			 * make sure core sensor id is initialized.
			 */
			cpus[cpu].sensor_id =
				sensor_get_id((char *)cpus[cpu].sensor_type);
			cpus[cpu].id_type = THERM_ZONE_ID;
			if (!(msm_thermal_info.freq_mitig_control_mask & BIT(cpu)))
				continue;
			hi_thresh = &cpus[cpu].threshold[FREQ_THRESHOLD_HIGH];
			low_thresh = &cpus[cpu].threshold[FREQ_THRESHOLD_LOW];

			hi_thresh->temp = msm_thermal_info.freq_mitig_temp_degc//门限最高值
					* tsens_scaling_factor;
			hi_thresh->trip = THERMAL_TRIP_CONFIGURABLE_HI;
			low_thresh->temp = (msm_thermal_info.freq_mitig_temp_degc -//门限最低值
				msm_thermal_info.freq_mitig_temp_hysteresis_degc)
					* tsens_scaling_factor;
			low_thresh->trip = THERMAL_TRIP_CONFIGURABLE_LOW;
			hi_thresh->notify = low_thresh->notify =
				freq_mitigation_notify;
			hi_thresh->data = low_thresh->data = (void *)&cpus[cpu];

			sensor_mgr_set_threshold(cpus[cpu].sensor_id, hi_thresh);//更新每个sensor的阈值
		}
	init_freq_thread:
		init_completion(&freq_mitigation_complete);
		freq_mitigation_task = kthread_run(do_freq_mitigation, NULL,//开启一个线程更新频率
			"msm_thermal:freq_mitig");

		if (IS_ERR(freq_mitigation_task)) {
			pr_err("Failed to create frequency mitigation thread. err:%ld\n",
					PTR_ERR(freq_mitigation_task));
			return;
		} else {
			complete(&freq_mitigation_complete);
		}
	}

## thermal驱动控制温度
这节我们从msm_thermal_init分析thermal驱动是如何控制温度的，这里就是KTM。这个函数我们主要分析创建了一个work，并且执行了我们看下check_temp函数。

	int msm_thermal_init(struct msm_thermal_data *pdata)
	{
		int ret = 0;

		ret = devmgr_devices_init(pdata->pdev);
		if (ret)
			pr_err("cannot initialize devm devices. err:%d\n", ret);

		msm_thermal_init_cpu_mit(CPU_FREQ_MITIGATION | CPU_HOTPLUG_MITIGATION);
		BUG_ON(!pdata);
		memcpy(&msm_thermal_info, pdata, sizeof(struct msm_thermal_data));

		if (check_sensor_id(msm_thermal_info.sensor_id)) {
			pr_err("Invalid sensor:%d for polling\n",
					msm_thermal_info.sensor_id);
			return -EINVAL;
		}

		enabled = 1;
		polling_enabled = 1;
		ret = cpufreq_register_notifier(&msm_thermal_cpufreq_notifier,
				CPUFREQ_POLICY_NOTIFIER);
		if (ret)
			pr_err("cannot register cpufreq notifier. err:%d\n", ret);

		register_reboot_notifier(&msm_thermal_reboot_notifier);
		pm_notifier(msm_thermal_suspend_callback, 0);
		INIT_DELAYED_WORK(&retry_hotplug_work, retry_hotplug);
		INIT_DELAYED_WORK(&check_temp_work, check_temp);
		schedule_delayed_work(&check_temp_work, 0);

		if (num_possible_cpus() > 1) {
			cpus_previously_online_update();
			register_cpu_notifier(&msm_thermal_cpu_notifier);
		}
		msm_thermal_panic_notifier_init(&pdata->pdev->dev);

		return ret;
	}


checkt_temp函数就是控制温度的主函数，函数最后通过polling_enabled来控制是否调用schedule_delayed_work函数来继续执行checkt_temp函数，而polling_enabled主要是上层通过/sys/module/msm_thermal/parameters/enabled节点来控制。而这个间隔时间是由msm_thermal_info.poll_ms（dts的qcom,poll-ms ）决定的。

我们下面先主要分析这个函数，这里主要分析do_core_control和do_freq_control两个关于cpu的函数。

	static void check_temp(struct work_struct *work)
	{
		long temp = 0;
		int ret = 0;

		do_therm_reset();

		ret = therm_get_temp(msm_thermal_info.sensor_id, THERM_TSENS_ID, &temp);//tsens一般都是cpu相关的温度
		if (ret) {
			pr_err("Unable to read TSENS sensor:%d. err:%d\n",
					msm_thermal_info.sensor_id, ret);
			goto reschedule;
		}
		do_core_control(temp);
		do_vdd_mx();
		do_psm();
		do_gfx_phase_cond();
		do_cx_phase_cond();
		do_ocr();

		/*
		** All mitigation involving CPU frequency should be
		** placed below this check. The mitigation following this
		** frequency table check, should be able to handle the failure case.
		*/
		if (!freq_table_get)
			check_freq_table();

		do_vdd_restriction();
		do_freq_control(temp);

	reschedule:
		if (polling_enabled)
			schedule_delayed_work(&check_temp_work,
					msecs_to_jiffies(msm_thermal_info.poll_ms));
	}
	
我们先看do_core_control这个函数是有一个CONFIG_SMP来控制。这个函数很简单主要看temp是否大于core_limit_temp_degc（就是dts里面的qcom,core-limit-temp），如果大于直接除了cpu0的cpu拔核。而当温度小于core_limit_temp_degC减去core_temp_hysteresis_degC（dts的qcom,core-temp-hysteresis）时，再把所有的cpu online。

	#ifdef CONFIG_SMP
	static void __ref do_core_control(long temp)
	{
		int i = 0;
		int ret = 0;
		struct device *cpu_dev = NULL;

		if (!core_control_enabled)
			return;

		mutex_lock(&core_control_mutex);
		if (msm_thermal_info.core_control_mask &&
			temp >= msm_thermal_info.core_limit_temp_degC) {
			for (i = num_possible_cpus(); i > 0; i--) {
				if (!(msm_thermal_info.core_control_mask & BIT(i)))
					continue;
				if (cpus_offlined & BIT(i) && !cpu_online(i))
					continue;
				pr_info("Set Offline: CPU%d Temp: %ld\n",
						i, temp);
				lock_device_hotplug();
				if (cpu_online(i)) {
					cpu_dev = get_cpu_device(i);
					trace_thermal_pre_core_offline(i);
					ret = device_offline(cpu_dev);
					if (ret < 0)
						pr_err("Error %d offline core %d\n",
							   ret, i);
					trace_thermal_post_core_offline(i,
						cpumask_test_cpu(i, cpu_online_mask));
				}
				unlock_device_hotplug();
				cpus_offlined |= BIT(i);
				break;
			}
		} else if (msm_thermal_info.core_control_mask && cpus_offlined &&
			temp <= (msm_thermal_info.core_limit_temp_degC -
				msm_thermal_info.core_temp_hysteresis_degC)) {
			for (i = 0; i < num_possible_cpus(); i++) {
				if (!(cpus_offlined & BIT(i)))
					continue;
				cpus_offlined &= ~BIT(i);
				pr_info("Allow Online CPU%d Temp: %ld\n",
						i, temp);
				/*
				 * If this core is already online, then bring up the
				 * next offlined core.
				 */
				lock_device_hotplug();
				if (cpu_online(i)) {
					unlock_device_hotplug();
					continue;
				}
				/* If this core wasn't previously online don't put it
				   online */
				if (!(cpumask_test_cpu(i, cpus_previously_online))) {
					unlock_device_hotplug();
					continue;
				}
				cpu_dev = get_cpu_device(i);
				trace_thermal_pre_core_online(i);
				ret = device_online(cpu_dev);
				if (ret)
					pr_err("Error %d online core %d\n",
							ret, i);
				trace_thermal_post_core_online(i,
					cpumask_test_cpu(i, cpu_online_mask));
				unlock_device_hotplug();
				break;
			}
		}
		mutex_unlock(&core_control_mutex);
	}
	
而do_freq_control函数是控制cpu频率，当温度大于msm_thermal_info.limit_temp_degC（dts中的qcom,limit-temp），直接将cpu减少bootup_freq_step个档位（dts中的qcom,freq-step ），而当温度小于msm_thermal_info.limit_temp_degC减去msm_thermal_info.temp_hysteresis_degC，cpu频率再增加bootup_freq_step个档位。

	static void do_freq_control(long temp)
	{
		uint32_t cpu = 0;
		uint32_t max_freq = cpus[cpu].limited_max_freq;

		if (core_ptr)
			return do_cluster_freq_ctrl(temp);
		if (!freq_table_get)
			return;

		if (temp >= msm_thermal_info.limit_temp_degC) {
			if (limit_idx == limit_idx_low)
				return;

			limit_idx -= msm_thermal_info.bootup_freq_step;
			if (limit_idx < limit_idx_low)
				limit_idx = limit_idx_low;
		} else if (temp < msm_thermal_info.limit_temp_degC -
			 msm_thermal_info.temp_hysteresis_degC) {
			if (limit_idx == limit_idx_high)
				return;

			limit_idx += msm_thermal_info.bootup_freq_step;
			if (limit_idx >= limit_idx_high)
				limit_idx = limit_idx_high;
		}

		/* Update new limits */
		get_online_cpus();
		max_freq = table[limit_idx].frequency;
		if (max_freq == cpus[cpu].limited_max_freq) {
			put_online_cpus();
			return;
		}

		for_each_possible_cpu(cpu) {
			if (!(msm_thermal_info.bootup_freq_control_mask & BIT(cpu)))
				continue;
			pr_info("Limiting CPU%d max frequency to %u. Temp:%ld\n",
				cpu, max_freq, temp);
			cpus[cpu].limited_max_freq =
					min(max_freq, cpus[cpu].vdd_max_freq);
			if (!SYNC_CORE(cpu))
				update_cpu_freq(cpu);
		}
		update_cluster_freq();
		put_online_cpus();
}
## 控制KTM
用户可以控制KTM是否执行，我们来看下。如下代码操作/sys/module/msm_thermal/parameters/enabled节点就是去执行set_enabled函数

	static int __ref set_enabled(const char *val, const struct kernel_param *kp)
	{
		int ret = 0;

		ret = param_set_bool(val, kp);
		if (!enabled)
			interrupt_mode_init();
		else
			pr_info("no action for enabled = %d\n",
				enabled);

		pr_info("enabled = %d\n", enabled);

		return ret;
	}

	static struct kernel_param_ops module_ops = {
		.set = set_enabled,
		.get = param_get_bool,
	};

	module_param_cb(enabled, &module_ops, &enabled, 0644);
	MODULE_PARM_DESC(enabled, "enforce thermal limit on cpu");
	
我们可以看下如下解释，name就是parameters下的enabled。

	/**
	 * module_param_cb - general callback for a module/cmdline parameter
	 * @name: a valid C identifier which is the parameter name.
	 * @ops: the set & get operations for this parameter.
	 * @perm: visibility in sysfs.
	 *
	 * The ops can have NULL set or get functions.
	 */
	#define module_param_cb(name, ops, arg, perm)				      \
		__module_param_call(MODULE_PARAM_PREFIX, name, ops, arg, perm, -1, 0)
		
因此我们来看set_enabled函数，当write为N的时候最后调用了interrupt_mode_init函数，这里我们先只关注polling_enabled为0.而为0后，前面分析在check_temp函数中就不会去执行schedule_delayed_work也就不会再去到check_temp。因而KTM检测温度也就不工作了。从这代码分析这个函数将polling_enabled置位1后不能恢复，也就是说KTM失效之后，不能再恢复了。
	
	static void interrupt_mode_init(void)
	{
		if (!msm_thermal_probed)
			return;

		if (polling_enabled) {
			polling_enabled = 0;
			create_sensor_zone_id_map();
			disable_msm_thermal();
			hotplug_init();
			freq_mitigation_init();
			thermal_monitor_init();
			msm_thermal_add_cx_nodes();
			msm_thermal_add_gfx_nodes();
		}
	}
	



