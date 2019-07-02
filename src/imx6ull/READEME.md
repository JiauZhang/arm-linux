#### 查看串口
```shell
# 可以拔插一次串口，根据变动的信息判断
dmesg | grep tty
```
#### minicom配置
```shell
sudo minicom -s
```
#### U盘
- 系统自动挂在在**/mnt**目录下
#### 模块信息
```shell
# 安装模块
insmod module_name
# 查看模块信息
cat /proc/modules
# 卸载模块
rmmod /PATH_TO/module_name
```
#### 通过TFTP从主机获取文件
```shell
wr tftp -r bcmdhd.ko -g YOUR_IP_ADDR
```

