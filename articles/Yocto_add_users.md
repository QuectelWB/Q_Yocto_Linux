在local.conf文件下修改：

1.修改root用户的密码，密码为biao123：

INHERIT += "extrausers"
EXTRA_USERS_PARAMS += "usermod -P biao1234 root;"
1
2
2.添加用户名为biao，密码为biaobiao的账户：

INHERIT += "extrausers"
EXTRA_USERS_PARAMS += "useradd -P biaobiao biao;"