# Yocto的构建示意图

OpenEmbedded是Yocto Project特有的构建系统，构建系统的核心是任务执行器BitBake，OpenEmbedded的构建过程流程如下图所示：


这个构建过程的流程图初一看晦涩难懂，但将其分成下面5个部分后，便很容易理解。

	区域1：配置文件：包括用户配置、元数据（包含配方、补丁）、机器配置、发行版配置；

	区域2：源文件：下载的三个途径：上游发行版，本地项目和SCM；

	区域3：构建系统：包括源代码获取、打补丁、编译。分析输出以生成软件包，创建和测试软件包，生成映像以及SDK；

	区域4：扩展软件包：包含输出软件包（RPM，DEB或IPK）的目录，这些目录随后用于构建系统生成的映像或软件开发套件（SDK）的构建。如果启用了运行时程序包管理，还可以使用Web服务器或其他方式复制和共享这些提要，以方便在运行时扩展或更新设备上的现有镜像；

	区域5：生成镜像文件和SDK。

整个构建过程的顺序分别是：

	源文件获取，源文件解压和打补丁，配置、编译和安装，输出分析和打包，创建镜像，创建SDK。

本节主要讲述第一部分：配置文件。

## 2 配置文件

### 2.1 用户配置

使用oe-init-build-env脚本创建构建环境

	$ source oe-init-build-env build

构建脚本便会在poky/build/conf目录下生成默认用户配置文件，包含local.conf和bblayers.conf。

local.conf文件提供了许多定义构建环境的基本变量。用户配置列出了构建镜像目标的体系结构、在何处存储下载源代码及构建属性等。用户目录拥有最高级别的配置。下图为local.conf文件，对部分常用变量进行描述：

切换到build目录，查看conf/local.conf

	$ cat conf/local.conf
	#目标机器的选择
	MACHINE ??= "qemux86-64"
	#发行版
	DISTRO ?= "poky"
	#指定OpenEmbedded构建系统在打包数据时使用的程序包管理器
	PACKAGE_CLASSES ?= 'package_rpm'
	
	EXTRA_IMAGE_FEATURES ?= "debug-tweaks"
	
	USER_CLASSES ?= "buildstats image-mklibs image-prelink"
	
	PATCHRESOLVE = "noop"

	#构建时，启用线程个数。

	BB_NUMBER_THREADS = "4"
	.........
	#源下载被存放的位置(注意此时该目录在BUILD目录下，建议把该目录放到构建环境目录之外)
	DL_DIR ?= "${TOPDIR}/downloads/"
	#共享缓存目录(注意此时该目录在BUILD目录下，建议把该目录放到构建环境目录之外)
	SSTATE_DIR ?= "${TOPDIR}/sstate_cache/"
	#构建输出目录
	TMP_DIR ?= "${TOPDIR}/tmp/"
	CONF_VERSION = "1"

注意：

- DL_DIR：软件包下载的目录，默认的设置是在构建环境目录下的downloads目录，建议修改到构建环境以外的目录，这样，当你新建一个构建环境时，不用重复下载这些软件包。（Yocto项目构建的大部分时间都浪费在下载这些软件包上）
SSTATE_DIR：共享的状态缓存文件目录，默认的设置是在构建环境目录下的sstate-cache目录，建议修改到构建环境以外的目录。因为在构建过程中，很多任务会产生大量的中间输出，而这些中间输出对将来其他的构建来说时可以重用的，这样可以大大加速构建过程。
您可以使用绝对路径对变量指向的路径进行设置：

	DL_DIR ?= "home/hann/poky/downloads/"
	SSTATE_DIR ?= "home/hann/poky/sstate_cache/"
	
	bblayers.conf文件告诉BitBake在构建过程中要使用哪些层。默认情况下，此文件中仅列出的层包括构建系统所需的层。所以，您必须手动添加已创建的所有自定义图层。下图为bblayers.conf的内容：

	# build/conf/bblayers.conf文件发生变化后，变量POKY_BBLAYERS_CONF_VERSION的数字会随着增加。 

	POKY_BBLAYERS_CONF_VERSION = "2"

	BBPATH = "${TOPDIR}"
	BBFILES ?= ""

	BBLAYERS ?= " \
	  /home/hann/poky/meta \
	  /home/hann/poky/meta-poky \
	  /home/hann/poky/meta-yocto-bsp \
	  "

### 2.2 配方文件

下面通过一个简单例子hello.bb介绍配方文件，hello.bb在项目中的位置：

	/poky/documentation/ref-manual/examples/hello-single

hello.bb配方文件的内容如下：

	#配方需编译文件的描述
	DESCRIPTION = "Simple helloworld application"
	SECTION = "examples"
	#遵守的协议
	LICENSE = "MIT"
	#LICENSE文件，该文件对应的MD5
	LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
	#从本地获取源文件
	SRC_URI = "file://helloworld.c"
	#变量S指的是bitbake放置已解压文件目录的完整路径
	S = "${WORKDIR}"
	#构建：通过执行make命令来实现编译和链接
	do_compile() {
	    ${CC} helloworld.c -o helloworld
	}
	#安装：通过执行make install命令将编译好的文件安装到变量${D}${bindir}指定的目录中
	do_install() {
	    install -d ${D}${bindir}
	    install -m 0755 helloworld ${D}${bindir}
	}

### 2.3 机器配置

Yocto项目板级支持包meta-yocto-bsp层的目录结构如下所示，可以看出在/conf/machine中存放机器配置文件*.conf。

	hann@hann-virtual-machine:~/yocto/poky/meta-yocto-bsp$ tree -L 2
	.
	├── conf
	│   ├── layer.conf
	│   └── machine
		├── beaglebone-yocto.conf
		├── edgerouter.conf
		├── genericx86-64.conf
		├── genericx86.conf
		├── include
		└── mpc8315e-rdb.conf
	├── lib
	│   └── oeqa
	├── README.hardware
	├── recipes-bsp
	│   ├── formfactor
	│   └── gma500-gfx-check
	├── recipes-graphics
	│   └── xorg-xserver
	├── recipes-kernel
	│   └── linux
	└── wic
	    ├── beaglebone-yocto.wks
	    ├── edgerouter.wks
	    ├── genericx86.wks
	    └── mpc8315e-rdb.wks

	12 directories, 6 files

下面是一个默认的典型配置文件，机器名为x86_64，统用的PC机和服务器的计算机配置。支持常用的驱动程序，这些驱动程序可以在“典型”的硬件上使用。

	DEFAULTTUNE ?= "core2-64"
	require conf/machine/include/tune-core2.inc
	require conf/machine/include/genericx86-common.inc

	SERIAL_CONSOLES_CHECK = "ttyS0"
	#For runqemu
	QB_SYSTEM_NAME = "qemu-system-x86_64"
	
### 3.4 层配置文件

为特定发行版构建的镜像或SDK提供顶级或常规策略。例如，在Poky中，发行层是 meta-poky层。在发行版层中是一个conf/distro包含发行版配置文件的目录，如poky.conf ，其中包含用于Poky发行版的 许多策略配置。

	meta-poky

	hann@hann-virtual-machine:~/yocto/poky/meta-poky$ tree -L 2
	.
	├── classes
	│   └── poky-sanity.bbclass
	├── conf
	│   ├── bblayers.conf.sample
	│   ├── conf-notes.txt
	│   ├── distro
	│   ├── layer.conf
	│   ├── local.conf.sample
	│   ├── local.conf.sample.extended
	│   └── site.conf.sample
	├── README.poky
	└── recipes-core
	    ├── busybox
	    ├── psplash
	    └── tiny-init

	7 directories, 8 files
	layer.conf

	#在conf和classes目录下搜素配置文件和类文件
	BBPATH =. "${LAYERDIR}:"

	#在recipes-*文件中查找配方文件和追加文件
	BBFILES += "${LAYERDIR}/recipes-*/*/*.bb \
		    ${LAYERDIR}/recipes-*/*/*.bbappend"
	#下面定义 搜索的方式和等级
	BBFILE_COLLECTIONS += "yocto"
	BBFILE_PATTERN_yocto = "^${LAYERDIR}/"
	BBFILE_PRIORITY_yocto = "5"

	LAYERSERIES_COMPAT_yocto = "zeus"

	#版本号
	LAYERVERSION_yocto = "3"

	LAYERDEPENDS_yocto = "core"

	REQUIRED_POKY_BBLAYERS_CONF_VERSION = "2"
