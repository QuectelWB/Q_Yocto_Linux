

安装

- apt-get

	sudo apt install -y libgstreamer1.0-0 gstreamer1.0-plugins-base gstreamer1.0-plugins-good \
		gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-doc \
		 gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 \
		 gstreamer1.0-qt5 gstreamer1.0-pulseaudio

- 编译

	git clone https://gitlab.freedesktop.org/gstreamer/gst-build.git -b 1.18

	sudo pip3 install meson
	sudo pip3 install ninja

	cd gst-build
	meson --prefix=/usr/lib/gstreamer-1.0  build
	ninja -C build
	meson install -C build

编译遇到的问题

	关闭http ssl校验 git config --global http.sslverify false

	pip3 install --user meson  //先更新下pip3

	$ git config --global http.postBuffer 524288000
	$ git config --global core.compression -1   
	$ git config --global http.maxRequestBuffer 100M
	$ git config --global http.lowSpeedLimit 0
	$ git config --global http.lowSpeedTime 999999


	 export LD_LIBRARY_PATH=/wk/gst-build/gst185_xz/gst-build/build_esw/out/lib/x86_64-linux-gnu/gstreamer-1.0/





