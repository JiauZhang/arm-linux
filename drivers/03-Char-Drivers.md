### 主设备号和次设备号
对字符设备的访问是通过文件系统内的设备名称进行的，这些名称被称为特殊文件、设备文件，或者简单的称之为文件系统树的节点，
它们通常位于**//dev**目录下。通常而言，主设备号标示设备对应的驱动程序。现代的Linux内核允许多个驱动程序共享主设备号，
但大多数设备仍然按照**一个主设备号对应一个驱动程序**的原则组织。

次设备号由内核使用，用于正确确定设备文件所指的设备。
### 设备编号的内部表达
要获得dev_t的主设备号或次设备号，应使用：
```c
MAJOR(dev_t dev);
MINOR(dev_t dev);
```
相反，如果需要将主设备号和次设备号转换成dev_t类型，则使用：
```c
MKDEV(int major, int minor);
```
### 分配和释放设备编号
获取指定设备号
```c
int register_chrdev_region(dev_t first, unsigned int count, char *name);
```
动态分配设备号
```c
int alloc_chrdev_region(dev_t *dev, unsigned int firstminor, \
						unsigned int count, char *name);
```
释放设备编号
```c
void unregister_chrdev_region(dev_t first, unsigned int count);
```
驱动程序应尽量使用alloc_chrdev_region而不是register_chrdev_region函数

**动态分配的缺点：**由于分配的主设备号不能保证始终一致，所以无法预先创建设备节点。对于驱动程序的一般用法，
这倒不是问题，因为一旦分配了设备号，就可以从/proc/devices中读取得到。因此，为了加载一个使用动态主设备好的设备驱动程序，
对insmod的调用可替换为一个简单的脚本，该脚本在调用insmod之后读取/proc/devices以获得新分配的主设备号，然后创建对应的
设备文件。
### 重要的数据结构
**file_operations**结构：用来建立设备编号与驱动程序之间的连接。最重要的设备方法：
```c
struct file_operations fops = {
	.owner = xxx,
	.llseek = xxx,
	.read = xxx,
	.write = xxx,
	.ioctl = xxx,
	.open = xxx,
	.release = xxx,
};
```
该声明采用了标准C的标记化结构初始化语法，使得代码更加紧凑且易读，允许对结构成员进行重新排列。在某些场合下，
将频繁访问的成员放在相同的硬件缓存上，将大大提高性能。

**file**结构：

**inode**结构：用来获得主设备号和次设备号的宏：
```c
unsigned int iminor(struct inode *inode);
unsigned int imajor(struct inode *inode);
```