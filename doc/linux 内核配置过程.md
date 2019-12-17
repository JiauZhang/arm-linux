### 内核版本
- linux 2.6.32.2

### 配置内核的过程
配置内核可以通过执行 make menuconfig 来进行，下面分析该命令的执行流程
- 执行该目标
```
%config: scripts_basic outputmakefile FORCE
	$(Q)mkdir -p include/linux include/config
	$(Q)$(MAKE) $(build)=scripts/kconfig $@
```

- 依赖目标
```
scripts_basic:
	$(Q)$(MAKE) $(build)=scripts/basic
```

- \$\(build\) 的作用
```
### 
# Shorthand for $(Q)$(MAKE) -f scripts/Makefile.build obj=
# Usage:
# $(Q)$(MAKE) $(build)=dir
build := -f $(if $(KBUILD_SRC),$(srctree)/)scripts/Makefile.build obj
# 原文件中的注释已经说得很清楚了，这是一个执行特定 Makefile 的缩写
# 因此 $(Q)$(MAKE) $(build)=scripts/basic 可以转换为
# $(Q)$(MAKE) -f $(if $(KBUILD_SRC),$(srctree)/)scripts/Makefile.build obj=scripts/basic
# 即 make 执行 Makefile.build 这个文件
```

- Makefile.build 关键语句
这几句的目的是把 obj 记录的目录下的 Makefile include 进来，从而编译一些主机上运行的程序
```
# src := $(obj)
# The filename Kbuild has precedence over Makefile
# kbuild-dir := $(if $(filter /%,$(src)),$(src),$(srctree)/$(src))
# kbuild-file := $(if $(wildcard $(kbuild-dir)/Kbuild),$(kbuild-dir)/Kbuild,$(kbuild-dir)/Makefile)
# include $(kbuild-file)
```

### make menuconfig
现在回到原始目标上，make menuconfig 执行的指令如下
```
%config: scripts_basic outputmakefile FORCE
	$(Q)mkdir -p include/linux include/config
	$(Q)$(MAKE) $(build)=scripts/kconfig $@

# 即主要是执行
$(Q)$(MAKE) $(build)=scripts/kconfig menuconfig
```
同样的，它还会再次进入到 Makfile.build 文件中，并把 scripts/kconfig 目录下的 Makefile 包含进来，
同时执行的目标变成了 menuconfig

### scripts/kconfig/Makefile
对应的目标如下
```
ifdef KBUILD_KCONFIG
Kconfig := $(KBUILD_KCONFIG)
else
# 实际是这个值，比如 arch/arm/Kconfig
Kconfig := arch/$(SRCARCH)/Kconfig
endif

menuconfig: $(obj)/mconf
	$< $(Kconfig)
```
其依赖于 scripts/kconfig/mconf 这个程序，而此程序并不存在，在该 Makefile 以下的内容中给出了其生成规则，
并且还会检查生成该程序所以来的程序是否存在，执行的脚本是 scripts/kconfig/lxdialog/check-lxdialog.sh,
如果没有安装依赖库，则出现错误提示
```
# Check if we can link to ncurses
check() {
        $cc -xc - -o $tmp 2>/dev/null <<'EOF'
#include CURSES_LOC
main() {}
EOF
	if [ $? != 0 ]; then
	    echo " *** Unable to find the ncurses libraries or the"       1>&2
	    echo " *** required header files."                            1>&2
	    echo " *** 'make menuconfig' requires the ncurses libraries." 1>&2
	    echo " *** "                                                  1>&2
	    echo " *** Install ncurses (ncurses-devel) and try again."    1>&2
	    echo " *** "                                                  1>&2
	    exit 1
	fi
}
```
这就是为什么执行 make menuconfig 之前需要先安装该程序的原因，因为需要用到它~

以上步骤顺利完成之后，scripts/kconfig/mconf 程序就顺利生成了，接下来就是真正的开始启动配置了
```
scripts/kconfig/mconf $(Kconfig)
# 以 arm 为例，以下路径其实已经被替换成了绝对路径，这里省略
scripts/kconfig/mconf arch/arm/Kconfig
```
至此，真正的配置工作开始了，mconf 开始读取 Kconfig 并进行解析，其解析规则可以查看
mconf.c 文件，当然了，没必要去看它如何实现的，只需要了解 Kconfig 的语法即可，其语法
可以参考[https://www.kernel.org/doc/html/latest/kbuild/kconfig-language.html](https://www.kernel.org/doc/html/latest/kbuild/kconfig-language.html)

如果不出意外，现在终端上会显示一个图形界面，然后就可以对各个选项进行配置了，退出之后
程序会把配置项写入到 .config 文件中，并生成相应的头文件供源码使用。