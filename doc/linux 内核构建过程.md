### 构建内核
```shell
# shell 执行如下指令
make zImage
```

### 全局变量
```makefile
srctree		:= $(if $(KBUILD_SRC),$(KBUILD_SRC),$(CURDIR))
objtree		:= $(CURDIR)
src		:= $(srctree)
obj		:= $(objtree)

VPATH		:= $(srctree)$(if $(KBUILD_EXTMOD),:$(KBUILD_EXTMOD))

export srctree objtree VPATH
```

### 体系架构相关变量
```makefile
ARCH		?= arm
CROSS_COMPILE	?= arm-linux-gnueabi-
# Architecture as present in compile.h
UTS_MACHINE 	:= $(ARCH)
SRCARCH 	:= $(ARCH)
```

### 编译会用到的通用定义
```makefile
# Look for make include files relative to root of kernel src
MAKEFLAGS += --include-dir=$(srctree)

# We need some generic definitions (do not try to remake the file).
$(srctree)/scripts/Kbuild.include: ;
include $(srctree)/scripts/Kbuild.include
```

### 引入具体体系结构相关 Makefile
```makefile
# Read arch specific Makefile to set KBUILD_DEFCONFIG as needed.
# KBUILD_DEFCONFIG may point out an alternative default configuration
# used for 'make defconfig'
include $(srctree)/arch/$(SRCARCH)/Makefile
export KBUILD_DEFCONFIG KBUILD_KCONFIG
```

### 内核 vmlinux 镜像布局
```makefile
# 用于处理用户直接在 shell 中输入 make 这类操作
# 真正的目标是 all，此时仅生成 vmlinux
# That's our default target when none is given on the command line
PHONY := _all
_all:

# If building an external module we do not care about the all: rule
# but instead _all depend on modules
PHONY += all
ifeq ($(KBUILD_EXTMOD),)
_all: all
else
_all: modules
endif
# 真正的目标
all: vmlinux

vmlinux: $(vmlinux-lds) $(vmlinux-init) $(vmlinux-main) vmlinux.o $(kallsyms.o) FORCE

# head-y 对应的值 arch/arm/kernel/head.o arch/arm/kernel/init_task.o
vmlinux-init := $(head-y) $(init-y)
vmlinux-main := $(core-y) $(libs-y) $(drivers-y) $(net-y)
vmlinux-all  := $(vmlinux-init) $(vmlinux-main)
vmlinux-lds  := arch/$(SRCARCH)/kernel/vmlinux.lds
export KBUILD_VMLINUX_OBJS := $(vmlinux-all)

core-y		+= kernel/ mm/ fs/ ipc/ security/ crypto/ block/
init-y		:= $(patsubst %/, %/built-in.o, $(init-y))
core-y		:= $(patsubst %/, %/built-in.o, $(core-y))
drivers-y	:= $(patsubst %/, %/built-in.o, $(drivers-y))
net-y		:= $(patsubst %/, %/built-in.o, $(net-y))
libs-y1		:= $(patsubst %/, %/lib.a, $(libs-y))
libs-y2		:= $(patsubst %/, %/built-in.o, $(libs-y))
libs-y		:= $(libs-y1) $(libs-y2)

vmlinux-dirs	:= $(patsubst %/,%,$(filter %/, $(init-y) $(init-m) \
		     $(core-y) $(core-m) $(drivers-y) $(drivers-m) \
		     $(net-y) $(net-m) $(libs-y) $(libs-m)))

vmlinux-alldirs	:= $(sort $(vmlinux-dirs) $(patsubst %/,%,$(filter %/, \
		     $(init-n) $(init-) \
		     $(core-n) $(core-) $(drivers-n) $(drivers-) \
		     $(net-n)  $(net-)  $(libs-n)    $(libs-))))

# Build vmlinux
# ---------------------------------------------------------------------------
# vmlinux is built from the objects selected by $(vmlinux-init) and
# $(vmlinux-main). Most are built-in.o files from top-level directories
# in the kernel tree, others are specified in arch/$(ARCH)/Makefile.
# Ordering when linking is important, and $(vmlinux-init) must be first.
#
# vmlinux
#   ^
#   |
#   +-< $(vmlinux-init)
#   |   +--< init/version.o + more
#   |
#   +--< $(vmlinux-main)
#   |    +--< driver/built-in.o mm/built-in.o + more
#   |
#   +-< kallsyms.o (see description in CONFIG_KALLSYMS section)
#
# vmlinux version (uname -v) cannot be updated during normal
# descending-into-subdirs phase since we do not yet know if we need to
# update vmlinux.
# Therefore this step is delayed until just before final link of vmlinux -
# except in the kallsyms case where it is done just before adding the
# symbols to the kernel.
#
# System.map is generated to document addresses of all kernel symbols
```

### 生成 zImage
```makefile
# 这其中给出了 make zImage 目标及其依赖
zImage Image xipImage bootpImage uImage: vmlinux
	$(Q)$(MAKE) $(build)=$(boot) MACHINE=$(MACHINE) $(boot)/$@
# 其作用是执行 arch/$(SRCARCH)/Makefile，传入 MACHINE 参数
# 执行目标是 $(boot)/$@ --> arch/$(SRCARCH)/zImage

# 查看 arch/arm/boot/Makefile
$(obj)/zImage:	$(obj)/compressed/vmlinux FORCE
	$(call if_changed,objcopy)
	@echo '  Kernel: $@ is ready'

$(obj)/Image: vmlinux FORCE
	$(call if_changed,objcopy)
	@echo '  Kernel: $@ is ready'

$(obj)/compressed/vmlinux: $(obj)/Image FORCE
	$(Q)$(MAKE) $(build)=$(obj)/compressed $@

# 该目标生成需要进入到 arch/arm/boot/compressed 下执行 make
# 使用 $(obj)/vmlinux.lds 链接脚本链接
# head.o, piggy.o misc.o
$(obj)/vmlinux: $(obj)/vmlinux.lds $(obj)/$(HEAD) $(obj)/piggy.o \
	 	$(addprefix $(obj)/, $(OBJS)) FORCE
	$(call if_changed,ld)
	@:
# (obj)/vmlinux 的依赖目标
# 压缩 boot 目录下的 vmlinux 文件生成 piggy.gz 
# if_changed 执行的命令为 cmd_* 形式，定义在 Makefile.lib 中
$(obj)/piggy.gz: $(obj)/../Image FORCE
	$(call if_changed,gzip)

$(obj)/piggy.o:  $(obj)/piggy.gz FORCE

CFLAGS_font.o := -Dstatic=

$(obj)/font.c: $(FONTC)
	$(call cmd,shipped)
# 生成链接脚本
$(obj)/vmlinux.lds: $(obj)/vmlinux.lds.in arch/arm/boot/Makefile .config
	@sed "$(SEDFLAGS)" < $< > $@

########################################################################
############# 最终，arch/arm/boot 目录下生成 zImage...... ##############
########################################################################
```