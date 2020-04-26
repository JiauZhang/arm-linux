## 引导 ARM Linux
> 本文翻译自：https://www.kernel.org/doc/html/latest/arm/booting.html

引导 ARM Linux 需要一个引导加载程序，它是一个先于内核运行的小段程序。
引导加载程序被用于初始化各种设备，并最终调用 Linux 内核，同时给内核传递参数。

总之，引导加载程序需要提供一下几种功能：
1. 设置和初始化内存
2. 初始化一个串口
3. 检测机器类型
4. 设置内核标签列表
5. 加载 initramfs
6. 调用内核镜像

### 1. 设置和初始化内存
- Existing boot loaders: MANDATORY
- New boot loaders: MANDATORY

引导加载程序用来查询和初始化所有内存，该过程是体系结构相关的。

### 2. 初始化一个串口
- Existing boot loaders: OPTIONAL, RECOMMENDED
- New boot loaders: OPTIONAL, RECOMMENDED

引导加载程序应该在目标办上初始化和使能一个串口，
这样可以使内核串口驱动自动地检测哪个串口可用于内核控制台。

另外，引导加载程序可以通过标记列表传递 'console=' 选项给内核指定端口，
串口参数格式选项详见：Documentation/admin-guide/kernel-parameters.rst

### 3. 检测机器类型
- Existing boot loaders: OPTIONAL
- New boot loaders: MANDATORY except for DT-only platforms

引导加载程序需要通过一些方法来检测目标板类型，最终提供给内核一个 MACH_TYPE_xxx
的值给内核，该值详见：linux/arch/arm/tools/mach-types。
该值需要放在寄存器 r1 中。

### 4. 设置引导数据
- Existing boot loaders: OPTIONAL, HIGHLY RECOMMENDED
- New boot loaders: MANDATORY

引导加载程序必须一个标记列表或者 dtb 镜像给内核，该引导数据的物理地址需要放置在寄存器 r2 中。

引导家在程序必须创建和初始化内核标记列表，一个有效的标记列表开始于ATAG_CORE，
结束于ATAG_NONE。引导加载程序最少要传递系统内存的大小和位置以及文件系统位置。
因此最小的标记列表类似于：

```
              +-----------+
base ->       | ATAG_CORE |  |
              +-----------+  |
              | ATAG_MEM  |  | increasing address
              +-----------+  |
              | ATAG_NONE |  |
              +-----------+  v
```

标记列表应该放置在系统 RAM 中，推荐放置在前 16KiB 处，以防止被内核解压缩程序覆盖掉。

### 5. 加载 initramfs
- Existing boot loaders: OPTIONAL
- New boot loaders: OPTIONAL

### 6. 调用内核镜像
- Existing boot loaders: MANDATORY
- New boot loaders: MANDATORY

调用内核镜像 zImage 有两种可能，如果 zImage 位于 flash 中，并且被正确链接，
那么就可以直接在 flash 中调用 zImage。同时 zImage 也可能在 RAM 中调用，
内核应该放置在内存的前 128MiB 中，推荐加载到 32MIB 之上来避免内核在解压缩之前重定位，
这可以加快引导速度。

如果引导的是原生的内核，那么内核的加载位置就必须严格等于 TEXT_OFFSET - PAGE_OFFSET。

无论什么情况，以下条件必须得到满足：
- 关闭所有 DMA 相关的设备
- CPU 寄存器设置
    - r0 = 0
    - r1 = 机器类型码
    - r3 = 标记列表在系统内存中的物理地址
- CPU 模式
    - 关闭所有中断
    - CPU 必须处于 SVC 模式
- 缓存和 MMU
    - MMU 必须关闭
    - 指令缓存可以打开可以关闭
    - 数据缓存必须关闭
- 引导加载程序应该直接跳转到内核镜像处执行第一条指令