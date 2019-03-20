### 安装交叉编译工具
```
1.更换下载源，将source.list复制到/etc/apt下
2.执行命令
	sudo apt-get update
	sudo apt-get upgrade
	sudo apt-get install gcc-arm-linux-gnueabi
```
### 开启SSH服务
```
1.安装软件
	sudo apt-get install openssh-server
2.备份sshd配置文件
	sudo cp /etc/ssh/sshd_config /etc/ssh/sshd_config.backup
3.修改sshd_config文件
	Port 23 #取消注释并把端口改为23
	ListenAddress 0.0.0.0 #取消注释
	#StrictModes yes #注释
	PasswordAuthentication yes #允许密码登录
4.启动ssh服务
	sudo service ssh start
```
