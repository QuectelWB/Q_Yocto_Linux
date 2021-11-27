#开机



## grub & cmdline

	
	
	quectel@q-OptiPlex-7070:~/Yocto/poky/build$ cat /etc/default/grub
	# If you change this file, run 'update-grub' afterwards to update
	# /boot/grub/grub.cfg.
	# For full documentation of the options in this file, see:
	#   info -f grub -n 'Simple configuration'

	GRUB_DEFAULT=0
	GRUB_TIMEOUT_STYLE=hidden
	GRUB_TIMEOUT=10
	GRUB_DISTRIBUTOR=`lsb_release -i -s 2> /dev/null || echo Debian`
	GRUB_CMDLINE_LINUX_DEFAULT="quiet splash"
	GRUB_CMDLINE_LINUX="net.ifnames=0 biosdevname=0"

	# Uncomment to enable BadRAM filtering, modify to suit your needs
	# This works with Linux (no patch required) and with any kernel that obtains
	# the memory map information from GRUB (GNU Mach, kernel of FreeBSD ...)
	#GRUB_BADRAM="0x01234567,0xfefefefe,0x89abcdef,0xefefefef"

	# Uncomment to disable graphical terminal (grub-pc only)
	#GRUB_TERMINAL=console

	# The resolution used on graphical terminal
	# note that you can use only modes which your graphic card supports via VBE
	# you can see them in real GRUB with the command `vbeinfo'
	#GRUB_GFXMODE=640x480

	# Uncomment if you don't want GRUB to pass "root=UUID=xxx" parameter to Linux
	#GRUB_DISABLE_LINUX_UUID=true

	# Uncomment to disable generation of recovery mode menu entries
	#GRUB_DISABLE_RECOVERY="true"

	# Uncomment to get a beep at grub start
	#GRUB_INIT_TUNE="480 440 1"

	quectel@q-OptiPlex-7070:~/Yocto/poky/build$ cat /proc/cmdline
	BOOT_IMAGE=/boot/vmlinuz-5.4.0-90-generic root=UUID=a72acd25-09f1-47df-af04-0ddd4ba599f3 ro net.ifnames=0 biosdevname=0 quiet splash vt.handoff=7
	


## 



