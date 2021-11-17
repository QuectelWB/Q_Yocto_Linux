#QT5.5移植全攻略

From https://www.cnblogs.com/tracing/p/4982296.html

https://doc.qt.io/qt-5/embedded-linux.html

##编译

> 到www.qt.io下载源码，qt-everywhere-opensource-src-5.5.0
设置编译器或者说平台。编译器是通过xplatform参数指定的，xplatform后面跟的参数实际上就时qtbase/mkspec下的文件夹的名字，这里以linux-arm-gnueabi-g++为模板，拷贝这个文件夹，重命名为arm-linux，然后修改这个文件夹里的qmake.conf，修改后的内容如下：

        #  modifications to g++.conf
        QMAKE_CC                = arm-none-linux-gnueabi-gcc
        QMAKE_CXX               = arm-none-linux-gnueabi-g++
        QMAKE_LINK              = arm-none-linux-gnueabi-g++
        QMAKE_LINK_SHLIB        = arm-none-linux-gnueabi-g++
 
        # modifications to linux.conf
        QMAKE_AR                = arm-none-linux-gnueabi-ar cqs
        QMAKE_OBJCOPY           = arm-none-linux-gnueabi-objcopy
        QMAKE_NM                = arm-none-linux-gnueabi-nm -P
        QMAKE_STRIP             = arm-none-linux-gnueabi-strip
		
这里修改的就是交叉编译器的名字，根据个人情况而定，我使用的是友善的编译器。

> 配置

	./configure --prefix=/QT5.5 -no-c++11 -no-qml-debug -xplatform arm-linux \
		 -qt-freetype -no-opengl -qt-pcre -nomake tests -nomake examples \
		-sysroot /opt/FriendlyARM/toolschain/4.4.3/arm-none-linux-gnueabi/sys-root/  
		-v

这里的arm-linux就上上面拷贝的文件夹的名字。这里需要说明的是sysroot，指定了sysroot，编译器就会到sysroot目录去找头文件和库，否则就会去本机找，网上的移植过程基本都没设置这个路径，在qt的官网上可以找到这个说明http://doc.qt.io/qt-5/embedded-linux.html,设置了sysroot之后，这个目录就是根目录了，prefix是相对于这个目录来安装的，也就是说make install后，qt的库安装到了/opt/FriendlyARM/toolschain/4.4.3/arm-none-linux-gnueabi/sys-root/QT5.5目录下。这个配置可能会提示跟xcb相关的东西，按照提示安装相关软件即可。配置之后，qt会提示你加上-qreal float选项，这个是为了跟以前的兼容，这里没有加，如果加上也没问题，加上之后会碰到编译上的一些问题，这个在编译小节里说明。

> 编译。编译过程中，碰到了几个问题。
- 跟clock_gettime相关的错误。根据提示在源文件所在目录的Makefile里，找到LIBS加上-lrt
- 因为加了-qreal float引起的编译问题，如果不加这个选项，就不存在这两个问题。都在qpdf.cpp中，一个是qBound(0., stops.at(i).first, 1.);其中qBound定义如下:

        Q_DECL_CONSTEXPR inline const T &qBound(const T &min, const T &val, const T &max)
        { return qMax(min, qMin(max, val)); }
		
qBound的３个参数是一样的，而stops.at(i).first是qreal类型的，0.和1.是double类型的，而qreal类型被指定为了float类型，所以就造成了这种编译问题，可以将stops.at(i).first强制转换为double类型。
另一个问题也在这里文件里，是<<符号的重载问题，代码如下：

        QPdf::ByteStream s(&shader);
        s << "<<\n"
        "/ShadingType 2\n"
        "/ColorSpace " << (alpha ? "/DeviceGray\n" : "/DeviceRGB\n") <<
        "/AntiAlias true\n"
        "/Coords [" << start.x() << start.y() << stop.x() << stop.y() << "]\n"
        "/Extend [true true]\n"
        "/Function " << function << "0 R\n"
        ">>\n"
        "endobj\n";
		
这个地方没弄明白，`ByteStream`已经重载了`qreal`类型，提示是模糊的重载，有int或者char可以备选,将`start.y()`等强制转换为`int`后编译通过。
- 因为配置的时候加了`-no-opengl`选项，在编译qt3d的时候，需要用到opengl的模块，编译不过。因为用不到opengl，所以将qt3d屏蔽掉。找到源码目录下的`qt.pro`，屏蔽掉下面两句话:

			addModule(qt3d, qtdeclarative qtimageformats)
			addModule(qtcanvas3d, qtdeclarative)
		
用#号将这两句话注释掉就可以了。

>安装。make install


## 在目标板上运行
先编译一个qt程序，在qtcreator的管理构建套件里设置好QT Versions和编译器，这里QT Versions要设置成安装之后QT5.5的qmake路径,本文里是/opt/FriendlyARM/toolschain/4.4.3/arm-none-linux-gnueabi/sys-root/QT5.5/bin/qmake这个路径,编译器就选/opt/FriendlyARM/toolschain/4.4.3/bin/arm-linux-g++

将编译好的QT5.5拷贝到目标板上，这里放到/opt目录下。

指定QT的运行平台，这里使用linuxfb，QT有几种运行平台，在官网上有说明http://doc.qt.io/qt-5/embedded-linux.html，这里使用linuxfb，新建一个文件setqtenv.sh，写下如下内容：

		export QTDIR=/opt/QT5.5
		export QT_QPA_FONTDIR=$QTDIR/lib/fonts
		export QT_QPA_PLATFORM_PLUGIN_PATH=$QTDIR/plugins/
		export LD_LIBRARY_PATH=$QTDIR/lib:$LD_LIBRARY_PATH
		export QT_QPA_PLATFORM=linuxfb:fb=/dev/fb0:size=800x600:mmSize=800x600:offset=0x0:tty=/dev/tty1
	
如果不指定`QT_QPA_FONTDIR`QT就会到`/QT5.5`的路径下去找字体，结果就找不到了。如果不指定`QT_QPA_PLATFORM_PLUGIN_PATH`，应用程序启动的时候就会提示找不到`linuxfb`，`linuxfb`是QT的一个插件，可以在`/opt/QT5.5/plugins/platforms/`文件夹下看到一个`libqlinuxfb.so`。`LD_LIBRARY_PATH`是指定库的路径，这个就不说了，最熟悉的东西了。`QT_QPA_PLATFORM`是指定平台，`fb`指定`framebuffer`的设备地址，具体的参数在官网上有说明参见上面给出的链接。然后运行`source setqtenv.sh`添加这些环境变量，至此，界面就可以显示出来了。

## 支持触摸屏键盘
首先应用程序要链接ts库，在qtcreator的工程文件里加上LIBS += -lts
设置触摸屏相关的环境变量，如下：

	export QT_QPA_GENERIC_PLUGINS=tslib,evdevkeyboard:/dev/input/event0
	export TSLIB_TSDEVICE=/dev/touchscreen-1wire
	export TSLIB_CALIBFILE=/etc/pointercal
	export TSLIB_CONFFILE=/etc/ts.conf
	export TSLIB_PLUGINDIR=/usr/lib/ts

这个QT_QPA_GENERIC_PLUGINS参数在官网上没找到说明，百度到的，evdevkeyboard是键盘插件，冒号后面是键盘设备的路径，在/opt/QT5.5/plugins/generic目录下可以看到一个libqtslibplugin.so，剩下的就是tslib的配置了，这里使用的是友善触摸屏，tslib已经移植好了，如果没有tslib的话，还需要移植tslib。至此，触摸屏就可以使用了，直接执行./ArmTest就可以了，不需要加其他参数，这里ArmTest是我的测试程序。
