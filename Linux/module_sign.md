#关于内核模块签名

	https://www.cnblogs.com/uestc-mm/p/7644966.html

- ***CONFIG_MODULE_SIG=y*** 表示开启了签名机制，但是这时候模块签名或不签名都可以使用。
- ***CONFIG_MODULE_SIG_FORCE=y*** 如果上述配置项使能，则模块必须有正确的签名才能正常使用。
- ***CONFIG_MODULE_SIG_ALL=y*** 内核在编译的时候，并不会主动去给模块签名，除非你把上述配置项打开。 
	
	
	/usr/src/linux-headers-4.4.0-96-generic/scripts/sign-file 	\
		sha512 		\
		/usr/src/linux-headers-4.4.0-96-generic/certs/signing_key.pem 	\
		/usr/src/linux-headers-4.4.0-96-generic/certs/signing_key.x509 
		hello.ko

	SC20_Linux_R09_r00320.1/poky/build/tmp-glibc/work$ find -name "signing_key*"
	./msm8909-oe-linux-gnueabi/linux-msm/3.18-r5_quec-smart/build/signing_key.priv
	./msm8909-oe-linux-gnueabi/linux-msm/3.18-r5_quec-smart/build/signing_key.x509
	./x86_64-nativesdk-oesdk-linux/nativesdk-libdnf/0.9.3-r0/git/data/tests/gpgkey/signing_key.pub


