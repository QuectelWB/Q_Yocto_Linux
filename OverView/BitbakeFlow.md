#Yocto编译流程详细攻略及bitbake实用方法

	ref from: https://www.kancloud.cn/digest/yocto


	(1): Yocto 编译后文件放在了哪里 输出文件位置
	(2): Yocto Linux内核编译目录在哪？
	(3): Yocto 如何重新编译Linux内核与dtb，并放到deploy目录？
	(4): Yocto 如何确定(找到)一个包的名字
	(5): Yocto如何更改source code的下载与git clone地址
	(6): Yocto中如何共享已经下载的文件
	(7): Yocto Bitbake的clean与cleanall以及cleansstate的区别
	(8): Yocto 如何加快软件包的下载 如何只下载所有需要的软件包而不编译
	(9): Yocto 软件包的下载地址在哪
	(10): Yocto hellworld 添加一个软件包
	(11): Yocto如何往最终的rootfs中添加软件
	(12): 使用Yocto帮组我们寻找难找的固件与固件放置路径
	(13): Yocto 查看软件包的依赖关系
	(14): Yocto中编译链的配置变更(tuning)
	(15): Yocto中的包管理器
	(16): Yocto 制作SDK分发Toolchain 脱离Yocto环境开发
	(17): Yocto License问题:restricted license not whitelisted in LICENSE_FLAGS_WHITELIST
	(18): Yocto SDK Toolchian中静态库的添加
	(19): Yocto SDK Toolchian的使用


## 输出文件位置

编译完成后内核以及rootfs，内核，uboot以及dtb会放在相对于build目录的tmp/deploy/images/MACHINE/下面,这里MACHINE需要可能为空（对于老版本的poky/yocto），也可能是你的板子名字，例如sabresd。
<br>
对于toolchain，那么放在mp/deploy/sdk/MACHINE/下。
编译完成后内核以及rootfs以及dtb会放在相对于build目录的tmp/deploy/images/imx6qsabresd/下面
<br>
各个文件都有一个软连接来指向最新编译产生的文件

## 内核目录

	bitbake -e linux-imx  | grep ^S=

## 编译内核

编译内核，并生成uImage+dtb，编译的命令如下：
	
	bitbake linux-imx -C compile
	
注意这里的-C的C是大写，和小写的c是有区别的。

## 包的名字
	
	bitbake -s | grep XXX


	root@6e3970505ba1:/home/biao/SC20/raw_SC20_SDK/SC20_Linux_R09_r00320.1/poky/build# bitbake -s | grep linux

	binutils-crosssdk-x86_64-oesdk-linux                :2.29.1-r0
	core-image-selinux                                    :1.0-r0
	core-image-selinux-minimal                            :1.0-r0
	cryptodev-linux                                       :1.9-r0
	cryptodev-linux-native                                :1.9-r0
	display-hal-linux                                     :git-r8
	display-noship-linux                                  :git-r3
	gcc-crosssdk-initial-x86_64-oesdk-linux                 :7.3.0-r0                 :6.4.0-r0
	gcc-crosssdk-x86_64-oesdk-linux                     :7.3.0-r0                 :6.4.0-r0
	libselinux                                            :2.6-r0
	libselinux-native                                     :2.6-r0
	linux-firmware                      1:0.0+gitAUTOINC+8fc2d4e556-r0
	linux-libc-headers                                   :4.12-r0
	linux-libc-headers-native                            :4.12-r0
	linux-msm                                  :4.9-r5_quec-smart       :3.18-r5_quec-smart
	nativesdk-cryptodev-linux                             :1.9-r0
	nativesdk-linux-libc-headers                         :4.12-r0
	nativesdk-syslinux                                   :6.03-r0
	nativesdk-util-linux                               :2.30-r0.3
	packagegroup-core-selinux                             :1.0-r0
	packagegroup-selinux-minimal                          :1.0-r0
	packagegroup-selinux-policycoreutils                   :1.0-r0
	selinux-autorelabel                                   :0.1-r0
	selinux-init                                          :0.1-r0
	selinux-labeldev                                      :0.1-r0
	syslinux-native                                      :6.03-r0
	util-linux                                         :2.30-r0.3
	util-linux-native                                  :2.30-r0.3
	
	
## git clone SRC_URI

	bitbake -e openvpn | grep ^SRC_URI=
	
	root@6e3970505ba1:/home/biao/SC20/raw_SC20_SDK/SC20_Linux_R09_r00320.1/poky/build# bitbake -e openvpn | grep ^SRC_URI=
	SRC_URI="http://openvpn.net/release/openvpn-2.1.3.tar.gz            file://openvpn"

这样就可以自己去手动下载


	root@6e3970505ba1:/home/biao/SC20/raw_SC20_SDK/SC20_Linux_R09_r00320.1/poky# find -name "openvpn*.bb"
	./meta-qti-bsp/recipes-connectivity/openvpn/openvpn_2.1.3.bb


	in openvpn_2.1.3.bb
	...
	SRC_URI = "http://openvpn.net/release/openvpn-${PV}.tar.gz \
	           file://openvpn"
	...


## license

Yocto中可以配置一个Distrbution的License，然后所有的软件包，都需要符合这个license才可以被shipped到image中，如果我们需要使用违反此license的软件包，那么就需要额外配置。


例如，在bitbake编译vlc的时候出现下面的错误log：
	
	$ bitbake vlc
	Parsing recipes: 100% |###################################################################################################################################| Time: 00:00:26
	Parsing of 2029 .bb files complete (0 cached, 2029 parsed). 2524 targets, 210 skipped, 1 masked, 0 errors.
	NOTE: Resolving any missing task queue dependencies
	ERROR: Nothing PROVIDES 'ffmpeg' (but /media/work/iMX6/Yocto/sources/meta-openembedded/meta-multimedia/recipes-multimedia/vlc/vlc_2.1.4.bb DEPENDS on or otherwise requires it)
	ERROR: libav PROVIDES ffmpeg but was skipped: because it has a restricted license not whitelisted in LICENSE_FLAGS_WHITELIST
	ERROR: libav PROVIDES ffmpeg but was skipped: because it has a restricted license not whitelisted in LICENSE_FLAGS_WHITELIST
	ERROR: Required build target 'vlc' has no buildable providers.
	Missing or unbuildable dependency chain was: ['vlc', 'ffmpeg']
	
那么就是因为vlc依赖libav，但是libav，的license不符合Distribution的License而无法编译。在libav中的bb文件配置中，可以看到其license flag为commercial：


