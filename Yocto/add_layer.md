#添加Layer


## 拉代码

	cd poky
	git clone -b sumo https://github.com/openembedded/meta-openembedded.git
	git clone -b sumo  git://github.com/openembedded/openembedded-core.git
	git clone -b sumo  https://github.com/meta-rust/meta-rust.git
	git clone -b sumo  https://github.com/Azure/meta-iotedge.git
	git clone -b sumo  git://git.yoctoproject.org/meta-virtualization



## 报错处理

- 后面执行source build/conf/set_bb_env.sh 遇到的报错处理：
	
	meta-virtualization/recipes-devtools/python/python-websocket-client.inc:6: Could not inherit file classes/pypi.bbclass 

将  openembedded-core/meta/classes/pypi.bbclass 拷贝到 meta/classes/里，其他遇到类似的问题同理；



- 遇到找不到 linux-yocto-4.12.bb 和 linux-yocto-4.15.bb 删除

meta-openembedded/recipes-kernel/ 这个目录；



- 将 openembedded-core/meta/lib 拷贝覆盖掉 meta/lib 目录，否则会遇到 一些python 函数没有定义的情况。检查过部分代码，基本上sumo 上的  openembedded-core/meta/lib  已经覆盖了  meta/lib  里的python 函数。



- 重新检查sdk，发现我们的代码Yocto 版本应该是 rocko ，如果上面  rust virtualization openembedded 拉 rock 分支或许会少许多问题。

由于一开始拉的是 sumo ，后来存在 版本兼容问题，因此删掉 

meta-openembedded/meta-XXX/conf/layer.conf 中关于
	
	LAYERSERIES_COMPAT_meta-XXX = "sumo"
	

