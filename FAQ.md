常见问题
======


## root 编译

	root@660a248560c4:/home/SC20/tmp/SC20_Linux_R09_r00320.1/poky/build# rebake lk
	ERROR:  OE-core's config sanity checker detected a potential misconfiguration.
	    Either fix the cause of this error or at your own risk disable the checker (see sanity.conf).
	    Following is the list of potential problems / advisories:
	
	    Do not use Bitbake as root.
	
	
	poky/meta/classes/sanity.bbclass 注释掉
	
	    if 0 == os.getuid():
        raise_sanity_error("Do not use Bitbake as root.", d) 

## 单独编译

	
	rebake lk
	rebake virtual/kernel

## 导出toolchain
	
	bitbake -s | grep -i toolchain     // 支持哪些toolchain
	bitbake meta-toolchain-qt5	 // qt toolchain
	bitbake machine-image -f -c populate_sdk  // all toolchain
	

## 


	
