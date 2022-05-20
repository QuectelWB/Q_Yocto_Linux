# device mapper 系统

https://www.jianshu.com/p/01a938464bb6
Device Mapper 和 docker 的原理

       dmsetup — low level logical volume management

       dmsetup clear device_name
       dmsetup create device_name [-u|--uuid uuid] [--addnodeoncreate|--addnodeonresume] [-n|--notable|--table {table|table_file}] [--readahead
                {[+]sectors|auto|none}]
       dmsetup deps [-o options] [device_name]
       dmsetup help [-c|-C|--columns]
       dmsetup info [device_name]
       dmsetup info -c|-C|--columns [--count count] [--interval seconds] [--nameprefixes] [--noheadings] [-o fields] [-O|--sort sort_fields] [--separa‐
                tor separator] [device_name]
       dmsetup load device_name [--table {table|table_file}]
       dmsetup ls [--target target_type] [--exec command] [--tree] [-o options]
       dmsetup mangle [device_name]
       dmsetup message device_name sector message
       dmsetup mknodes [device_name]
       dmsetup reload device_name [--table {table|table_file}]
       dmsetup remove [-f|--force] [--retry] [--deferred] device_name
       dmsetup remove_all [-f|--force] [--deferred]
       dmsetup rename device_name new_name
       dmsetup rename device_name --setuuid uuid
       dmsetup resume device_name [--addnodeoncreate|--addnodeonresume] [--noflush] [--nolockfs] [--readahead {[+]sectors|auto|none}]
       dmsetup setgeometry device_name cyl head sect start
       dmsetup splitname device_name [subsystem]
       dmsetup stats command [options]
       dmsetup status [--target target_type] [--noflush] [device_name]

https://www.lijiaocn.com/%E6%8A%80%E5%B7%A7/2017/07/07/linux-tool-devicemapper.html

将块设备映射成虚拟块设备。

todo:

	   lvm provides the command-line tools for LVM2.  A separate manual page describes each command in detail.

       If  lvm is invoked with no arguments it presents a readline prompt (assuming it was compiled with readline support).  LVM commands may be
       entered interactively at this prompt with readline facilities including history and command name and option completion.  Refer  to  read‐
       line(3) for details.


	命令
	pvcreate  vgdisplay  vgcreate  pvs  lvm lvm2


## dmsetup 命令

dmsetup table

	root@ubuntu:/home/biao# dmsetup  table
	ubuntu--vg-swap_1: 0 1998848 linear 8:19 464209920
	ubuntu--vg-root: 0 464207872 linear 8:19 2048
	

TABLE FORMAT
       Each line of the table specifies a single target and is of the form:

       logical_start_sector 	num_sectors 	target_type 	target_args
	开始扇区		扇区数		设备类型	设备参数


	root@ubuntu:/home/biao# dmsetup ls
	ubuntu--vg-swap_1       (252:1)
	ubuntu--vg-root 	(252:0)


创建虚拟块设备
dmsetup create 设备名称

thin,thin-pool 是device mapper 提供的一种存储类型

可以将多个虚拟设备存放在同一个数据卷上，Documentation/device-mapper/thin-provisioning.txt

## thin-pool 实验

	# 创建metadata 设备
	dd 	if=/dev/zero 	of=metadata 	bs=512 	count=100000
	losetup /dev/loop0  ./metadata

	# 创建一个data设备
	dd	if=/dev/zero	of=data		bs=512	count=100000
	losetup	/dev/loop1	./data

	# 创建pool
	dmsetup create pool --table "0 1000000 thin-pool /dev/loop0 /dev/loop1 512 1000"

其中的参数含义

	dmsetup create pool \
		--table "0 20971520 thin-pool $metadata_dev $data_dev \
		 $data_block_size $low_water_mark"



	pool:              自定义的名字
	0:                 开始扇区
	20971520:          结束扇区
	thin-pool:         设备类型
	$metadata_dev:     存放元数据的设备	
	$data_dev:         存放数据的设备
	$data_block_size:  数据块大小
	$low_water_mark:   空闲的数据块少于该数值时，发送通知



