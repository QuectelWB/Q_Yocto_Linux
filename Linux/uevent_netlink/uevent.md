	/*
	 * The actions here must match the index to the string array
	 * in lib/kobject_uevent.c
	 *
	 * Do not add new actions here without checking with the driver-core
	 * maintainers. Action strings are not meant to express subsystem
	 * or device specific properties. In most cases you want to send a
	 * kobject_uevent_env(kobj, KOBJ_CHANGE, env) with additional event
	 * specific variables added to the event environment.
	 */
	
	enum kobject_action {
		KOBJ_ADD,
		KOBJ_REMOVE,
		KOBJ_CHANGE,
		KOBJ_MOVE,
		KOBJ_ONLINE,
		KOBJ_OFFLINE,
		KOBJ_BIND,
		KOBJ_UNBIND,
		KOBJ_MAX
	};


	内核中通过kobject_uevent_env函数将事件发送到用户空间

	
