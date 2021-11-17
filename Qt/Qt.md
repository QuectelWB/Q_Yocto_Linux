# Qt5

##参考文档

https://developer.toradex.com/knowledge-base/how-to-set-up-qt-creator-to-cross-compile-for-embedded-linux

https://www.ics.com/blog/configuring-qt-creator-yocto-development

https://www.kynetics.com/docs/2018/Yocto-SDK-Qt/


## 准备工作

- 准备Qt Creator

	wget https://download.qt.io/official_releases/qt/5.12/5.12.10/qt-opensource-linux-x64-5.12.10.run

- 交叉编译工具链
	
	bitbake machine-image -f -c populate_sdk
	
	output: poky/tmp-glibc/deploy/sdk
	
	cd poky/tmp-glibc/deploy/sdk
	./oecore-x86_64-armv7ahf-neon-toolchain-nodistro.0.sh  // install the toolchain to /usr/local/oecore-x86_64
	same with qt5 toolchain	
	bitbake meta-toolchain-qt5
	
	source /usr/local/oecore-x86_64/environment-setup-armv7ahf-neon-oe-linux-gnueabi
	
- Qt Creator Settings

	Tools-> Options
	C Compiler:
	"/usr/local/oecorex86_64/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi/arm-oe-linux-gnueabigcc"
	C++ Compiler:
	“/usr/local/oecorex86_64/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi/arm-oe-linux-gnueabig++
	
	Debugger:
	“/usr/local/oecorex86_64/sysroots/x86_64-oesdk-linux/usr/bin/arm-oe-linux-gnueabi/arm-oe-linux-gnueabigdb”
	
	QT Version and qmake:
	“/usr/local/oecore-x86_64/sysroots/x86_64-oesdk-linux/usr/bin/qt5/qmake”
	
	Kits for msm8909:
	including C/C++ compiler , Debugger , qmake

## 写测试程序


	




