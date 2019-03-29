### 虚拟机环境配置
- 安装VirtualBox
- 配置虚拟机可使用的宿主机硬件配置，这里选择虚拟机可以物理内存为1024M，网络连接方式为桥接模式，
如果打开虚拟机后不好用，在网络中重新建立一个有线网，然后在不关闭虚拟机的情况下重新选择一个其他的设置，
最后重新设置回来，这步是为了刷新虚拟机网络~
- 在VirtualBox中安装Fedora9，由于其默认安装在C盘，所以需要先对安装位置进行设置
- 使用lokkit命令打开防火墙设置界面，关闭防火墙
### 设置板子IP
```shell
# 根据Fedora的IP地址进行设置
ifconfig eth0 xxx.xxx.xxx.xxx
# 测试网络是否互通
ping xxx.xxx.xxx.xxx
```
### 使用FTP向板子传送文件
```shell
# 板子IP
ftp xxx.xxx.xxx.xxx
# 登录板子
username
password
#设置传送格式
bin
# 发送指定文件
put filename
# 上一步可能出现：Permission denied
# 这是因为所登录账户没有写权限
# 使用串口登录板子root账户，提升用户权限
chmod 777 /home/username/
# 退出登录
by
```