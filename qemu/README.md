### 依赖工具
```shell
# 编译工具
sudo apt-get install -y make gcc

# ARM 模拟器
# linux@DESKTOP-JUL35C8:~$ qemu-system-arm --version
# QEMU emulator version 2.11.1(Debian 1:2.11+dfsg-1ubuntu7.26)
# Copyright (c) 2003-2017 Fabrice Bellard and the QEMU Project developers
sudo apt-get install -y qemu-system-arm

# dos 格式文件转化为 unix 格式
sudo apt-get install -y dos2unix

# 解压 zip 文件
sudo apt-get install -y zip

# menuconfig 依赖工具
sudo apt-get install -y libncurses5-dev
```

### 编译错误
```shell
drivers/net/phy/phy.c:41:27: fatal error: mach/platform.h: No such file or directory
 #include <mach/platform.h>
     ^
compilation terminated.
scripts/Makefile.build:307: recipe for target 'drivers/net/phy/phy.o' failed

# 解决办法
使用 make menuconfig 重新配置内核，将网络功能关闭
```

### 启动仿真
```shell
qemu-system-arm -machine vexpress-a9 -m 1024M -kernel ~/linux-3.4.y-nanopi2-lollipop-mr1/arch/arm/boot/zImage -nographic -append "console=ttyAMA0"
# 由于没有根文件系统，进入仿真后终端无法在退出，这时候就需要再开一个终端 kill 仿真程序
# 查看系统所有进程
ps -e
# 找到 qemu-system-arm 进程 PID
kill XXX_PID
```

### 运行警告信息
```shell
shared memfd open() failed: Function not implemented
pulseaudio: pa_context_connect() failed
pulseaudio: Reason: Connection refused
pulseaudio: Failed to initialize PA contextaudio: Could not init `pa' audio driver
ALSA lib confmisc.c:767:(parse_card) cannot find card '0'
ALSA lib conf.c:4528:(_snd_config_evaluate) function snd_func_card_driver returned error: No such file or directory
ALSA lib confmisc.c:392:(snd_func_concat) error evaluating strings
ALSA lib conf.c:4528:(_snd_config_evaluate) function snd_func_concat returned error: No such file or directory
ALSA lib confmisc.c:1246:(snd_func_refer) error evaluating name
ALSA lib conf.c:4528:(_snd_config_evaluate) function snd_func_refer returned error: No such file or directory
ALSA lib conf.c:5007:(snd_config_expand) Evaluate error: No such file or directory
ALSA lib pcm.c:2495:(snd_pcm_open_noupdate) Unknown PCM default
alsa: Could not initialize DAC
alsa: Failed to open `default':
alsa: Reason: No such file or directory
ALSA lib confmisc.c:767:(parse_card) cannot find card '0'
ALSA lib conf.c:4528:(_snd_config_evaluate) function snd_func_card_driver returned error: No such file or directory
ALSA lib confmisc.c:392:(snd_func_concat) error evaluating strings
ALSA lib conf.c:4528:(_snd_config_evaluate) function snd_func_concat returned error: No such file or directory
ALSA lib confmisc.c:1246:(snd_func_refer) error evaluating name
ALSA lib conf.c:4528:(_snd_config_evaluate) function snd_func_refer returned error: No such file or directory
ALSA lib conf.c:5007:(snd_config_expand) Evaluate error: No such file or directory
ALSA lib pcm.c:2495:(snd_pcm_open_noupdate) Unknown PCM default
alsa: Could not initialize DAC
alsa: Failed to open `default':
alsa: Reason: No such file or directory
audio: Failed to create voice `lm4549.out'

# 清除：并不管用
sudo apt-get autoremove --purge pulseaudio-utils

# 重新配置内核：内核启动过程中的可以清除，qemu 启动时的无法解除
Device Drivers -->
	Sound Card support --> 取消编译
	HID Devices --> 顺便把不需要的都给裁剪掉
	MMC/SD/SDIO Card support -->
	XXXX --> 还有一些其他的杂项，我关注的点是内核，外设相关的有没有无所谓

# 配置全局环境变量关闭 qemu 音频：该方法有效
export QEMU_AUDIO_DRV=none
```

### 终端重复输出打印信息
```shell
# 引导参数中的 console 设置为空
qemu-system-arm -machine vexpress-a9 -m 1024M -kernel ~/linux-3.4.y-nanopi2-lollipop-mr1/arch/arm/boot/zImage -nographic -append "console="
```